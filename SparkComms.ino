#include "SparkComms.h"

const uint8_t notifyOn[] = {0x1, 0x0};

struct packet_data packet_spark;
struct packet_data packet_app;

unsigned long lastAppPacketTime;
unsigned long lastSparkPacketTime;

// read from queue, pass-through to amp then check for a complete valid message to send on for processing
void setup_comms_queues() {
  packet_spark.size = 0;
  packet_app.size = 0;

  lastAppPacketTime = millis();
  lastSparkPacketTime = millis();

  qFromApp         = xQueueCreate(20, sizeof (struct packet_data));
  qFromSpark       = xQueueCreate(20, sizeof (struct packet_data));

  qFromAppFilter   = xQueueCreate(20, sizeof (struct packet_data));
  qFromSparkFilter = xQueueCreate(20, sizeof (struct packet_data));

#ifdef PSRAM
  if (psramInit()) {
    Serial.print("PSRAM ok: ");
    Serial.println(ESP.getFreePsram());
  }  
  else {
    Serial.println("PSRAM failure");
    while (true);
  }
#endif
}

// client callback for connection to Spark

class MyClientCallback : public BLEClientCallbacks
{
  void onConnect(BLEClient *pclient) {
    DEBUG("Spark connected");
    ble_spark_connected = true;   
  }
  void onDisconnect(BLEClient *pclient, int reason) {
    connected_sp = false;    
    ble_spark_connected = false;     
    DEBUG("Spark disconnected");   
  }
};

// server callback for connection to BLE app

class MyServerCallback : public BLEServerCallbacks {
  void onConnect(BLEServer *pserver, NimBLEConnInfo& connInfo)  {
     if (pserver->getConnectedCount() == 1) {
      DEBUG("App connection event and is connected"); 
      ble_app_connected = true;
    }
    else {
      DEBUG("App connection event and is not really connected");   
    }
  }
  void onDisconnect(BLEServer *pserver, NimBLEConnInfo& connInfo, int reason) {
    ble_app_connected = false;
    DEBUG("App disconnected");
    #ifdef CLASSIC
      pAdvertising->start(); 
    #endif
  }
};

#ifdef CLASSIC
// server callback for connection to BT classic app

void bt_callback(esp_spp_cb_event_t event, esp_spp_cb_param_t *param){
  if(event == ESP_SPP_SRV_OPEN_EVT){
    DEBUG("callback: Classic BT Spark app connected");
    bt_app_connected = true;
  }
 
  if(event == ESP_SPP_CLOSE_EVT ){
    DEBUG("callback: Classic BT Spark app disconnected");
    bt_app_connected = false;
  }
}
#endif


// From the Spark

void notifyCB_sp(BLERemoteCharacteristic* pRemoteCharacteristic, uint8_t* pData, size_t length, bool isNotify) {

#ifdef BLE_DUMP
  int i;
  byte b;

  DEB("FROM SPARK:        ");

  for (i = 0; i < length; i++) {
    b = pData[i];
    if (b < 16) DEB("0");
    DEB(b, HEX);    
    DEB(" ");
    if (i % 32 == 31) { 
      DEBUG("");
      DEB("                   ");
    }
  }
  DEBUG();
#endif

  struct packet_data qe;
  new_packet_from_data(&qe, pData, length);
  xQueueSend (qFromSpark, &qe, (TickType_t) 0);
}


// This works with IK Multimedia iRig Blueboard and the Akai LPD8 wireless - interestingly they have the same UUIDs
void notifyCB_pedal(BLERemoteCharacteristic* pRemoteCharacteristic, uint8_t* pData, size_t length, bool isNotify){

  int i;
  byte b;

  for (i = 0; i < length; i++) {
    b = pData[i];
    midi_in.add(b);
  }
  midi_in.commit();

}







class CharacteristicCallbacks: public BLECharacteristicCallbacks {
  void onWrite(BLECharacteristic* pCharacteristic, NimBLEConnInfo& connInfo) {
    std::string s = pCharacteristic->getValue(); 
    int size = s.size();
    const char *buf = s.c_str();

    //DEB("Got BLE callback size: ");
    //DEBUG(size);

    struct packet_data qe;
    new_packet_from_data(&qe, (uint8_t *) buf, size);
    xQueueSend (qFromApp, &qe, (TickType_t) 0);
  };
};

static CharacteristicCallbacks chrCallbacks_s, chrCallbacks_r;


// Serial BT callback for data
void data_callback(const uint8_t *buffer, size_t size) {
//  int index = from_app_index;

  //DEB("Got SerialBT callback size: ");
  //DEBUG(size);

#ifdef BLE_DUMP
    int i = 0;
    byte b;
    DEB("FROM APP:          ");
    for (i=0; i < size; i++) {
      b = buffer[i];
      if (b < 16) DEB("0");
      DEB(b, HEX);    
      DEB(" ");
      if (i % 32 == 31) { 
        DEBUG("");
        DEB("                   ");
      }   
    }
    DEBUG();
#endif

    struct packet_data qe;
    new_packet_from_data(&qe, (uint8_t *) buffer, size);
    xQueueSend (qFromApp, &qe, (TickType_t) 0);

}


BLEUUID SpServiceUuid(C_SERVICE);
// BLE pedal
BLEUUID PedalServiceUuid(PEDAL_SERVICE);

void connect_spark() {
  if (found_sp && !connected_sp) {
    if (pClient_sp != nullptr && pClient_sp->isConnected())
       DEBUG("connect_spark() thinks I was already connected");
    
    //if (pClient_sp->connect(sp_device)) {
    if (pClient_sp->connect(sp_address)) {  
#if defined CLASSIC  && !defined HELTEC_WIFI
        pClient_sp->setMTU(517);  
#endif
      connected_sp = true;
      pService_sp = pClient_sp->getService(SpServiceUuid);
      if (pService_sp != nullptr) {
        pSender_sp   = pService_sp->getCharacteristic(C_CHAR1);
        pReceiver_sp = pService_sp->getCharacteristic(C_CHAR2);
        if (pReceiver_sp && pReceiver_sp->canNotify()) {
#ifdef CLASSIC
          pReceiver_sp->registerForNotify(notifyCB_sp);
          p2902_sp = pReceiver_sp->getDescriptor(BLEUUID((uint16_t)0x2902));
          if (p2902_sp != nullptr)
             p2902_sp->writeValue((uint8_t*)notifyOn, 2, true);
#else
          if (!pReceiver_sp->subscribe(true, notifyCB_sp, true)) {
            connected_sp = false;
            DEBUG("Spark disconnected");
            NimBLEDevice::deleteClient(pClient_sp);
          }   
#endif
        } 
      }
      DEBUG("connect_spark(): Spark connected");
      ble_spark_connected = true;
    }
  }
}


void connect_pedal() {
  if (found_pedal && !connected_pedal) {
    //if (pClient_pedal->connect(pedal_device)) {  
    if (pClient_pedal->connect(pedal_address)) {
#if defined CLASSIC && !defined HELTEC_WIFI
      // BUG?
      pClient_sp->setMTU(517);
#endif
      connected_pedal = true;
      pService_pedal = pClient_pedal->getService(PedalServiceUuid);
      if (pService_pedal != nullptr) {
        pReceiver_pedal = pService_pedal->getCharacteristic(PEDAL_CHAR);

        if (pReceiver_pedal && pReceiver_pedal->canNotify()) {
#ifdef CLASSIC
          pReceiver_pedal->registerForNotify(notifyCB_pedal);
          p2902_pedal = pReceiver_pedal->getDescriptor(BLEUUID((uint16_t)0x2902));
          if(p2902_pedal != nullptr)
            p2902_pedal->writeValue((uint8_t*)notifyOn, 2, true);
#else
          if (!pReceiver_pedal->subscribe(true, notifyCB_pedal, true)) {
            connected_pedal = false;
            DEBUG("Pedal disconnected");
            NimBLEDevice::deleteClient(pClient_pedal);
          } 
#endif
        }
      }
      DEBUG("connect_pedal(): pedal connected");
    }
  }
}



bool connect_to_all() {
  int i, j;
  int counts;
  uint8_t b;
  int len;


  // init comms processing
  setup_comms_queues();

  strcpy(spark_ble_name, DEFAULT_SPARK_BLE_NAME);
  ble_spark_connected = false;
  ble_app_connected = false;
  bt_app_connected = false;    // only for Serial Bluetooth

  BLEDevice::init(spark_ble_name);        // put here for CLASSIC code
  BLEDevice::setMTU(517);
  pClient_sp = BLEDevice::createClient();
  pClient_sp->setClientCallbacks(new MyClientCallback());
 
  // BLE pedal
  pClient_pedal = BLEDevice::createClient();


  BLEDevice::getScan()->setInterval(40);
  BLEDevice::getScan()->setWindow(40);
  BLEDevice::getScan()->setActiveScan(true);
  pScan = BLEDevice::getScan();

  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallback());  
  pService = pServer->createService(S_SERVICE);
  pServer->advertiseOnDisconnect(true);
  
#ifdef CLASSIC  
  pCharacteristic_receive = pService->createCharacteristic(S_CHAR1, BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE | BLECharacteristic::PROPERTY_WRITE_NR);
  pCharacteristic_send = pService->createCharacteristic(S_CHAR2, BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_NOTIFY);
#else
  pCharacteristic_receive = pService->createCharacteristic(S_CHAR1, NIMBLE_PROPERTY::READ | NIMBLE_PROPERTY::WRITE | NIMBLE_PROPERTY::WRITE_NR);
  pCharacteristic_send = pService->createCharacteristic(S_CHAR2, NIMBLE_PROPERTY::READ | NIMBLE_PROPERTY::NOTIFY); 
#endif

  pCharacteristic_receive->setCallbacks(&chrCallbacks_r);
  pCharacteristic_send->setCallbacks(&chrCallbacks_s);
#ifdef CLASSIC
  pCharacteristic_send->addDescriptor(new BLE2902());
#endif

  pService->start();
#ifndef CLASSIC
  pServer->start(); 
#endif

  pAdvertising = BLEDevice::getAdvertising(); // create advertising instance
  
  pAdvertising->addServiceUUID(pService->getUUID()); // tell advertising the UUID of our service
  pAdvertising->enableScanResponse(true);  

  // Connect to Spark
  connected_sp = false;
  found_sp = false;

  // BLE pedal
  connected_pedal = false;
  found_pedal = false;

  DEBUG("Scanning for Spark");

  counts = 0;
  while (!found_sp && counts < MAX_SPARK_SCAN_COUNT) {   // assume we only use a pedal if on already and hopefully found at same time as Spark, don't wait for it
    counts++;
    pResults = pScan->getResults(4000);
    
    for(i = 0; i < pResults.getCount()  && !found_sp; i++) {
      device = pResults.getDevice(i);

      if (device->isAdvertisingService(SpServiceUuid)) {
        strncpy(spark_ble_name, device->getName().c_str(), SIZE_BLE_NAME);

        DEB("Found '");
        DEB(spark_ble_name);
        DEBUG("'");

        if (strstr(spark_ble_name, "40") != NULL) 
          spark_type = S40;
        else if (strstr(spark_ble_name, "GO") != NULL)
          spark_type = GO;
        else if (strstr(spark_ble_name, "MINI") != NULL)        
          spark_type = MINI;  
        else if (strstr(spark_ble_name, "LIVE") != NULL)     
          spark_type = LIVE; 
        else if (strstr(spark_ble_name, "Spark 2") != NULL)     
          spark_type = SPARK2; 
        else {
          DEBUG("Couldn't match Spark type");
          spark_type = NONE;
        }

        found_sp = true;
        connected_sp = false;
        sp_address = device->getAddress();
      }
    }
  }

  DEBUG("Scanning for BLE MIDI device");

  counts = 0;
  while (!found_pedal && counts < MAX_PEDAL_SCAN_COUNT) {  
    counts++;
    pResults = pScan->getResults(4000);
    
    for(i = 0; i < pResults.getCount()  && !found_pedal; i++) {
      device = pResults.getDevice(i);
      if (device->isAdvertisingService(PedalServiceUuid) 
         || strcmp(device->getName().c_str(),"iRig BlueBoard") == 0
         || strcmp(device->getName().c_str(),"WIDI Jack") == 0) {
        DEBUG("Found BLE MIDI device");
        found_pedal = true;
        connected_pedal = false;
        pedal_address= device->getAddress();
      }
    }
  }

  if (!found_sp) return false;   // failed to find the Spark within the number of counts allowed (MAX_SCAN_COUNT)
  connect_spark();
  connect_pedal();

#ifdef CLASSIC
  DEBUG("Starting classic bluetooth");
  // now advertise Serial Bluetooth
  bt = new BluetoothSerial();
  bt->register_callback(bt_callback);

  switch (spark_type) {
    case NONE:
    case S40:
      spark_bt_name = "Spark 40 Audio";
      break;
    case MINI:
      spark_bt_name = "Spark MINI Audio";
      break;
    case GO:
      spark_bt_name = "Spark GO Audio";
      break;
    case LIVE:
      spark_bt_name = "Spark LIVE Audio";
      break;
  }


  DEB("Creating classic bluetooth with name '");
  DEB(spark_bt_name);
  DEBUG("'");
  
  if (!bt->begin (spark_bt_name, false)) {
    DEBUG("Classic bluetooth init fail");
    while (true);
  }

  bt->onData(data_callback);

  // flush anything read from App - just in case
  while (bt->available())
    b = bt->read(); 
  DEBUG("Spark serial bluetooth set up");
#endif



  DEBUG("Available for app to connect...");  

  //== Start: try to look like a Spark Go
  //char scan_data[] = {0x0e,0x09,0x53,0x70,0x61,0x72,0x6b,0x20,0x47,0x4f,0x20,0x42,0x4c,0x45,0x00};
  //char adv_data[] =  {0x02,0x01,0x0a,0x03,0x03,0xc0,0xff,0x0b,0xff,0x06,0x10,0x00,0x00,0x08,0xeb,0xed,0x3d,0x5d,0x5a};

  //BLEAdvertisementData oAdvertisementData = BLEAdvertisementData();
  //BLEAdvertisementData oScanAdvertisementData = BLEAdvertisementData();  

  //oScanAdvertisementData.addData(scan_data, sizeof(scan_data));
  //oAdvertisementData.addData(adv_data, sizeof(adv_data));

  //pAdvertising->setAdvertisementData(oAdvertisementData);
  //pAdvertising->setScanResponseData(oScanAdvertisementData);
  //== Stop: that code


  pAdvertising->setName(spark_ble_name);
  //pAdvertising->setManufacturerData(manuf_data);
  pAdvertising->start(); 

  return true;
}

void send_to_spark(byte *buf, int len) {
  pSender_sp->writeValue(buf, len, false);
}


void send_to_app(byte *buf, int len) {
  if (ble_app_connected) {
    //pCharacteristic_send->setValue(buf, len);
    //pCharacteristic_send->notify(true);
    pCharacteristic_send->notify(buf, len, BLE_HS_CONN_HANDLE_NONE);
  }
#if defined CLASSIC
  if (bt_app_connected) {
    bt->write(buf, len);
  }
#endif
}

// for some reason getRssi() crashes with two clients!
int ble_getRSSI() { 
  return pClient_sp->getRssi();
}