#include "MIDI.h"

//
// MIDI State processing for BLE streams
//

#ifdef BLE_MIDI
void MIDIState::initialise(RingBuffer *rb)
{
  midi_stream = rb;
  midi_data_count = 0;
  midi_cmd_count = 0;
  midi_status = 0;
}

bool MIDIState::process(byte mi_data[3]) 
{
  byte b;
  bool got_data;

  got_data = false;
  
  if (!midi_stream->is_empty()) {
    midi_stream->get(&b);
    
    if (b <= 0x7F) {
      midi_data[midi_data_count] = b;
      midi_data_count++;
      midi_cmd_count = 0;
      if ((midi_status == 0xC0 || midi_status == 0xD0) || midi_data_count >= 2) {
        mi_data[0] = midi_status;
        mi_data[1] = midi_data[0];
        if (midi_data_count == 2)
          mi_data[2] = midi_data[1];
        else
          mi_data[2] = 0;
        midi_data_count = 0;
        got_data = true;
      }
    } 
    else {
      midi_cmd_count++;
      if (midi_cmd_count > 1) {
        midi_status = b;            // the last byte before a non-cmd byte will always be status unless it was a single timestamp
      }
    }
  } 
  return got_data;
}

MIDIState ble_midi;
#endif


//
// USB for ESP S3
//
#ifdef USB_S3

bool isMIDI = false;
bool isMIDIReady = false;

const size_t MIDI_IN_BUFFERS = 8;
const size_t MIDI_OUT_BUFFERS = 8;
usb_transfer_t *MIDIOut = NULL;
usb_transfer_t *MIDIIn[MIDI_IN_BUFFERS] = {NULL};

// USB MIDI Event Packet Format (always 4 bytes)
//
// Byte 0 |Byte 1 |Byte 2 |Byte 3
// -------|-------|-------|------
// CN+CIN |MIDI_0 |MIDI_1 |MIDI_2
//
// CN = Cable Number (0x0..0xf) specifies virtual MIDI jack/cable
// CIN = Code Index Number (0x0..0xf) classifies the 3 MIDI bytes.
// See Table 4-1 in the MIDI 1.0 spec at usb.org.
//

static void midi_transfer_cb(usb_transfer_t *transfer)
{
  ESP_LOGI("", "midi_transfer_cb context: %d", transfer->context);
  if (Device_Handle == transfer->device_handle) {
    int in_xfer = transfer->bEndpointAddress & USB_B_ENDPOINT_ADDRESS_EP_DIR_MASK;
    if ((transfer->status == 0) && in_xfer) {
      uint8_t *const p = transfer->data_buffer;
      for (int i = 0; i < transfer->actual_num_bytes; i += 4) {
        if ((p[i] + p[i+1] + p[i+2] + p[i+3]) == 0) break;
        ESP_LOGI("", "midi: %02x %02x %02x %02x",
            p[i], p[i+1], p[i+2], p[i+3]);

        USBHostBuf.add(p[i+1]);
        USBHostBuf.add(p[i+2]);
        USBHostBuf.add(p[i+3]);
        USBHostBuf.commit();
      }
      esp_err_t err = usb_host_transfer_submit(transfer);
      if (err != ESP_OK) {
        ESP_LOGI("", "usb_host_transfer_submit In fail: %x", err);
      }
    }
    else {
      ESP_LOGI("", "transfer->status %d", transfer->status);
    }
  }
}

void check_interface_desc_MIDI(const void *p)
{
  const usb_intf_desc_t *intf = (const usb_intf_desc_t *)p;

  // USB MIDI
  if ((intf->bInterfaceClass == USB_CLASS_AUDIO) &&
      (intf->bInterfaceSubClass == 3) &&
      (intf->bInterfaceProtocol == 0))
  {
    isMIDI = true;
    ESP_LOGI("", "Claiming a MIDI device!");
    esp_err_t err = usb_host_interface_claim(Client_Handle, Device_Handle,
        intf->bInterfaceNumber, intf->bAlternateSetting);
    if (err != ESP_OK) ESP_LOGI("", "usb_host_interface_claim failed: %x", err);
  }
}

void prepare_endpoints(const void *p)
{
  const usb_ep_desc_t *endpoint = (const usb_ep_desc_t *)p;
  esp_err_t err;

  // must be bulk for MIDI
  if ((endpoint->bmAttributes & USB_BM_ATTRIBUTES_XFERTYPE_MASK) != USB_BM_ATTRIBUTES_XFER_BULK) {
    ESP_LOGI("", "Not bulk endpoint: 0x%02x", endpoint->bmAttributes);
    return;
  }
  if (endpoint->bEndpointAddress & USB_B_ENDPOINT_ADDRESS_EP_DIR_MASK) {
    for (int i = 0; i < MIDI_IN_BUFFERS; i++) {
      err = usb_host_transfer_alloc(endpoint->wMaxPacketSize, 0, &MIDIIn[i]);
      if (err != ESP_OK) {
        MIDIIn[i] = NULL;
        ESP_LOGI("", "usb_host_transfer_alloc In fail: %x", err);
      }
      else {
        MIDIIn[i]->device_handle = Device_Handle;
        MIDIIn[i]->bEndpointAddress = endpoint->bEndpointAddress;
        MIDIIn[i]->callback = midi_transfer_cb;
        MIDIIn[i]->context = (void *)i;
        MIDIIn[i]->num_bytes = endpoint->wMaxPacketSize;
        esp_err_t err = usb_host_transfer_submit(MIDIIn[i]);
        if (err != ESP_OK) {
          ESP_LOGI("", "usb_host_transfer_submit In fail: %x", err);
        }
      }
    }
  }
  else {
    err = usb_host_transfer_alloc(endpoint->wMaxPacketSize, 0, &MIDIOut);
    if (err != ESP_OK) {
      MIDIOut = NULL;
      ESP_LOGI("", "usb_host_transfer_alloc Out fail: %x", err);
      return;
    }
    ESP_LOGI("", "Out data_buffer_size: %d", MIDIOut->data_buffer_size);
    MIDIOut->device_handle = Device_Handle;
    MIDIOut->bEndpointAddress = endpoint->bEndpointAddress;
    MIDIOut->callback = midi_transfer_cb;
    MIDIOut->context = NULL;
//    MIDIOut->flags |= USB_TRANSFER_FLAG_ZERO_PACK;
  }
  isMIDIReady = ((MIDIOut != NULL) && (MIDIIn[0] != NULL));
}

void show_config_desc_full(const usb_config_desc_t *config_desc)
{
  // Full decode of config desc.
  const uint8_t *p = &config_desc->val[0];
  uint8_t bLength;
  for (int i = 0; i < config_desc->wTotalLength; i+=bLength, p+=bLength) {
    bLength = *p;
    if ((i + bLength) <= config_desc->wTotalLength) {
      const uint8_t bDescriptorType = *(p + 1);
      switch (bDescriptorType) {
        case USB_B_DESCRIPTOR_TYPE_DEVICE:
          ESP_LOGI("", "USB Device Descriptor should not appear in config");
          break;
        case USB_B_DESCRIPTOR_TYPE_CONFIGURATION:
          show_config_desc(p);
          break;
        case USB_B_DESCRIPTOR_TYPE_STRING:
          ESP_LOGI("", "USB string desc TBD");
          break;
        case USB_B_DESCRIPTOR_TYPE_INTERFACE:
          show_interface_desc(p);
          if (!isMIDI) check_interface_desc_MIDI(p);
          break;
        case USB_B_DESCRIPTOR_TYPE_ENDPOINT:
          show_endpoint_desc(p);
          if (isMIDI && !isMIDIReady) {
            prepare_endpoints(p);
          }
          break;
        case USB_B_DESCRIPTOR_TYPE_DEVICE_QUALIFIER:
          // Should not be in config?
          ESP_LOGI("", "USB device qual desc TBD");
          break;
        case USB_B_DESCRIPTOR_TYPE_OTHER_SPEED_CONFIGURATION:
          // Should not be in config?
          ESP_LOGI("", "USB Other Speed TBD");
          break;
        case USB_B_DESCRIPTOR_TYPE_INTERFACE_POWER:
          // Should not be in config?
          ESP_LOGI("", "USB Interface Power TBD");
          break;
        default:
          ESP_LOGI("", "Unknown USB Descriptor Type: 0x%x", *p);
          break;
      }
    }
    else {
      ESP_LOGI("", "USB Descriptor invalid");
      return;
    }
  }
}


#endif





//
// Main MIDI setup and loop
//


void setup_midi() {
  byte b;

#ifdef USB_HOST
  if (Usb.Init() == -1) {
    DEBUG("USB host init failed");
    usb_connected = false;
  }
  else {
    DEBUG("USB host running");
    usb_connected = true;   
  }
#endif

#ifdef USB_S3
  usbh_setup(show_config_desc_full);
#endif


#ifdef BLE_MIDI
  ble_midi.initialise(&midi_in);
#endif

  ser1 = new HardwareSerial(1); 
  ser1->begin(31250, SERIAL_8N1, SER_RX, -1);
  while (ser1->available())
    b = ser1->read();
}

bool update_midi(byte *mid) {
  bool got_midi;
  byte b;
  char *midi_source;
  
  midi_source = "";
  got_midi = false;

#ifdef BLE_MIDI
  // BLE MIDI controller
  if (ble_midi.process(mid)) got_midi = true;
#endif


#ifdef USB_HOST
  // USB MIDI  
  if (usb_connected) {
    Usb.Task();

    if (Midi) {                                                  // USB Midi
      rcvd = Midi.RecvData(midi_buf, false);
      if (rcvd > 0) Serial.println("Got some USB midi data");
      if (rcvd > 0 && !(midi_buf[0] == 0 && midi_buf[1] == 0 && midi_buf[2] == 0)) {
        mid[0] = midi_buf[0];
        mid[1] = midi_buf[1];
        mid[2] = midi_buf[2];
        midi_source = "USB HOST";
        got_midi = true;
      }
    }
  }
#endif

#ifdef USB_S3
  usbh_task();

  if (!USBHostBuf.is_empty()) {
    if (USBHostBuf.get_data_len() >= 3) {
      USBHostBuf.get(&mid[0]);
      USBHostBuf.get(&mid[1]);
      USBHostBuf.get(&mid[2]);
      midi_source = "USB S3";
      got_midi = true;
    }
    else
    { // not at least three bytes in the buffer - this can't be right, should always be a multiple of three
      USBHostBuf.clear();
      Serial.println("Cleared S3 USB Host circular buffer!");
    }
    got_midi = true;
  }
  

#endif

  // Serial 1 - DIN MIDI
  if (ser1->available()) {
    mid[0] = ser1->read();
    mid[1] = ser1->read();
    
    if (mid[0] == 0xC0 || mid[0] == 0xD0)
      mid[2] = 0;
    else 
      mid[2] = ser1->read();
      
    if (mid[0] != 0xFE) {
      midi_source = "SERIAL DIN MIDI";
      got_midi = true;
    }
  }


  if (got_midi) {
    Serial.print("MIDI (");
    Serial.print(midi_source);
    Serial.print(") 0x");
    Serial.print(mid[0], HEX);
    Serial.print(" ");
    Serial.print(mid[1]);
    Serial.print(" ");   
    Serial.println(mid[2]);
  }
  
  return got_midi;
}
