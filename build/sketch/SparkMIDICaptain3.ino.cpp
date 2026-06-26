#include <Arduino.h>
#line 1 "/Users/leko/Downloads/SparkMIDICaptain-main/SparkMIDICaptain3/SparkMIDICaptain3.ino"
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_NeoPixel.h>

#include "SparkIO.h"
#include "Spark.h"

// --------------------------------------------------
// Pins
// --------------------------------------------------
#define BTN_UP     12
#define BTN_DOWN   14
#define BTN_TUN    27

#define EXP_PIN    34
#define TOE_PIN    25
#define LED_PIN     26
#define NUM_LEDS    8


  //#define OLED_SDA 4
  //#define OLED_SCL 15


// --------------------------------------------------
// Forward declarations
// --------------------------------------------------
//void updateScreen();
void animateTunerLEDs();

// --------------------------------------------------
// Tuner State
// --------------------------------------------------
bool tunerActive = false;
String tunerNote = "";
int tunerCents = 0;

// --------------------
//EXP 
// ---------------

int expRaw = 0;
int expFiltered = 0;
int expLastSent = -1;

#line 47 "/Users/leko/Downloads/SparkMIDICaptain-main/SparkMIDICaptain3/SparkMIDICaptain3.ino"
int smooth(int current, int previous);
#line 51 "/Users/leko/Downloads/SparkMIDICaptain-main/SparkMIDICaptain3/SparkMIDICaptain3.ino"
void updateExpressionPedal();
#line 102 "/Users/leko/Downloads/SparkMIDICaptain-main/SparkMIDICaptain3/SparkMIDICaptain3.ino"
int getGlobalPreset();
#line 114 "/Users/leko/Downloads/SparkMIDICaptain-main/SparkMIDICaptain3/SparkMIDICaptain3.ino"
void updateScreen();
#line 184 "/Users/leko/Downloads/SparkMIDICaptain-main/SparkMIDICaptain3/SparkMIDICaptain3.ino"
void routeInput_Spark(int event);
#line 215 "/Users/leko/Downloads/SparkMIDICaptain-main/SparkMIDICaptain3/SparkMIDICaptain3.ino"
void routeInput_DAW(int event);
#line 219 "/Users/leko/Downloads/SparkMIDICaptain-main/SparkMIDICaptain3/SparkMIDICaptain3.ino"
void routeInput_BOTH(int event);
#line 227 "/Users/leko/Downloads/SparkMIDICaptain-main/SparkMIDICaptain3/SparkMIDICaptain3.ino"
void handleButtons();
#line 261 "/Users/leko/Downloads/SparkMIDICaptain-main/SparkMIDICaptain3/SparkMIDICaptain3.ino"
void setup();
#line 292 "/Users/leko/Downloads/SparkMIDICaptain-main/SparkMIDICaptain3/SparkMIDICaptain3.ino"
void loop();
#line 236 "/Users/leko/Downloads/SparkMIDICaptain-main/SparkMIDICaptain3/MIDI.ino"
void setup_midi();
#line 265 "/Users/leko/Downloads/SparkMIDICaptain-main/SparkMIDICaptain3/MIDI.ino"
bool update_midi(byte *mid);
#line 5 "/Users/leko/Downloads/SparkMIDICaptain-main/SparkMIDICaptain3/Screen.ino"
void splash_screen();
#line 25 "/Users/leko/Downloads/SparkMIDICaptain-main/SparkMIDICaptain3/Screen.ino"
void show_connected();
#line 37 "/Users/leko/Downloads/SparkMIDICaptain-main/SparkMIDICaptain3/Screen.ino"
void show_message(char *msg, int preset);
#line 4 "/Users/leko/Downloads/SparkMIDICaptain-main/SparkMIDICaptain3/Spark.ino"
void dump_preset(SparkPreset preset);
#line 32 "/Users/leko/Downloads/SparkMIDICaptain-main/SparkMIDICaptain3/Spark.ino"
int get_effect_index(char *str);
#line 44 "/Users/leko/Downloads/SparkMIDICaptain-main/SparkMIDICaptain3/Spark.ino"
bool wait_for_spark(int command_expected);
#line 60 "/Users/leko/Downloads/SparkMIDICaptain-main/SparkMIDICaptain3/Spark.ino"
bool wait_for_app(int command_expected);
#line 78 "/Users/leko/Downloads/SparkMIDICaptain-main/SparkMIDICaptain3/Spark.ino"
bool spark_state_tracker_start();
#line 235 "/Users/leko/Downloads/SparkMIDICaptain-main/SparkMIDICaptain3/Spark.ino"
bool update_spark_state();
#line 379 "/Users/leko/Downloads/SparkMIDICaptain-main/SparkMIDICaptain3/Spark.ino"
void update_ui();
#line 410 "/Users/leko/Downloads/SparkMIDICaptain-main/SparkMIDICaptain3/Spark.ino"
void update_ui_hardware();
#line 449 "/Users/leko/Downloads/SparkMIDICaptain-main/SparkMIDICaptain3/Spark.ino"
void set_input1();
#line 463 "/Users/leko/Downloads/SparkMIDICaptain-main/SparkMIDICaptain3/Spark.ino"
void change_generic_model(char *new_eff, int slot);
#line 473 "/Users/leko/Downloads/SparkMIDICaptain-main/SparkMIDICaptain3/Spark.ino"
void change_comp_model(char *new_eff);
#line 477 "/Users/leko/Downloads/SparkMIDICaptain-main/SparkMIDICaptain3/Spark.ino"
void change_drive_model(char *new_eff);
#line 481 "/Users/leko/Downloads/SparkMIDICaptain-main/SparkMIDICaptain3/Spark.ino"
void change_amp_model(char *new_eff);
#line 492 "/Users/leko/Downloads/SparkMIDICaptain-main/SparkMIDICaptain3/Spark.ino"
void change_mod_model(char *new_eff);
#line 496 "/Users/leko/Downloads/SparkMIDICaptain-main/SparkMIDICaptain3/Spark.ino"
void change_delay_model(char *new_eff);
#line 502 "/Users/leko/Downloads/SparkMIDICaptain-main/SparkMIDICaptain3/Spark.ino"
void change_generic_onoff(int slot,bool onoff);
#line 511 "/Users/leko/Downloads/SparkMIDICaptain-main/SparkMIDICaptain3/Spark.ino"
void change_noisegate_onoff(bool onoff);
#line 515 "/Users/leko/Downloads/SparkMIDICaptain-main/SparkMIDICaptain3/Spark.ino"
void change_comp_onoff(bool onoff);
#line 519 "/Users/leko/Downloads/SparkMIDICaptain-main/SparkMIDICaptain3/Spark.ino"
void change_drive_onoff(bool onoff);
#line 523 "/Users/leko/Downloads/SparkMIDICaptain-main/SparkMIDICaptain3/Spark.ino"
void change_amp_onoff(bool onoff);
#line 527 "/Users/leko/Downloads/SparkMIDICaptain-main/SparkMIDICaptain3/Spark.ino"
void change_mod_onoff(bool onoff);
#line 531 "/Users/leko/Downloads/SparkMIDICaptain-main/SparkMIDICaptain3/Spark.ino"
void change_delay_onoff(bool onoff);
#line 535 "/Users/leko/Downloads/SparkMIDICaptain-main/SparkMIDICaptain3/Spark.ino"
void change_reverb_onoff(bool onoff);
#line 540 "/Users/leko/Downloads/SparkMIDICaptain-main/SparkMIDICaptain3/Spark.ino"
void change_generic_toggle(int slot);
#line 552 "/Users/leko/Downloads/SparkMIDICaptain-main/SparkMIDICaptain3/Spark.ino"
void change_noisegate_toggle();
#line 556 "/Users/leko/Downloads/SparkMIDICaptain-main/SparkMIDICaptain3/Spark.ino"
void change_comp_toggle();
#line 560 "/Users/leko/Downloads/SparkMIDICaptain-main/SparkMIDICaptain3/Spark.ino"
void change_drive_toggle();
#line 564 "/Users/leko/Downloads/SparkMIDICaptain-main/SparkMIDICaptain3/Spark.ino"
void change_amp_toggle();
#line 568 "/Users/leko/Downloads/SparkMIDICaptain-main/SparkMIDICaptain3/Spark.ino"
void change_mod_toggle();
#line 572 "/Users/leko/Downloads/SparkMIDICaptain-main/SparkMIDICaptain3/Spark.ino"
void change_delay_toggle();
#line 576 "/Users/leko/Downloads/SparkMIDICaptain-main/SparkMIDICaptain3/Spark.ino"
void change_reverb_toggle();
#line 581 "/Users/leko/Downloads/SparkMIDICaptain-main/SparkMIDICaptain3/Spark.ino"
void change_generic_param(int slot, int param, float val);
#line 596 "/Users/leko/Downloads/SparkMIDICaptain-main/SparkMIDICaptain3/Spark.ino"
void change_noisegate_param(int param, float val);
#line 600 "/Users/leko/Downloads/SparkMIDICaptain-main/SparkMIDICaptain3/Spark.ino"
void change_comp_param(int param, float val);
#line 604 "/Users/leko/Downloads/SparkMIDICaptain-main/SparkMIDICaptain3/Spark.ino"
void change_drive_param(int param, float val);
#line 608 "/Users/leko/Downloads/SparkMIDICaptain-main/SparkMIDICaptain3/Spark.ino"
void change_amp_param(int param, float val);
#line 612 "/Users/leko/Downloads/SparkMIDICaptain-main/SparkMIDICaptain3/Spark.ino"
void change_mod_param(int param, float val);
#line 616 "/Users/leko/Downloads/SparkMIDICaptain-main/SparkMIDICaptain3/Spark.ino"
void change_delay_param(int param, float val);
#line 620 "/Users/leko/Downloads/SparkMIDICaptain-main/SparkMIDICaptain3/Spark.ino"
void change_reverb_param(int param, float val);
#line 625 "/Users/leko/Downloads/SparkMIDICaptain-main/SparkMIDICaptain3/Spark.ino"
void change_hardware_preset(int pres_num);
#line 637 "/Users/leko/Downloads/SparkMIDICaptain-main/SparkMIDICaptain3/Spark.ino"
void change_custom_preset(SparkPreset *preset, int pres_num);
#line 650 "/Users/leko/Downloads/SparkMIDICaptain-main/SparkMIDICaptain3/Spark.ino"
void tuner_on_off(bool on_off);
#line 656 "/Users/leko/Downloads/SparkMIDICaptain-main/SparkMIDICaptain3/Spark.ino"
void send_tap_tempo(float tempo);
#line 12 "/Users/leko/Downloads/SparkMIDICaptain-main/SparkMIDICaptain3/SparkComms.ino"
void setup_comms_queues();
#line 92 "/Users/leko/Downloads/SparkMIDICaptain-main/SparkMIDICaptain3/SparkComms.ino"
void notifyCB_sp(NimBLERemoteCharacteristic* pRemoteCharacteristic, uint8_t* pData, size_t length, bool isNotify);
#line 120 "/Users/leko/Downloads/SparkMIDICaptain-main/SparkMIDICaptain3/SparkComms.ino"
void notifyCB_pedal(NimBLERemoteCharacteristic* pRemoteCharacteristic, uint8_t* pData, size_t length, bool isNotify);
#line 158 "/Users/leko/Downloads/SparkMIDICaptain-main/SparkMIDICaptain3/SparkComms.ino"
void data_callback(const uint8_t *buffer, size_t size);
#line 192 "/Users/leko/Downloads/SparkMIDICaptain-main/SparkMIDICaptain3/SparkComms.ino"
void connect_spark();
#line 229 "/Users/leko/Downloads/SparkMIDICaptain-main/SparkMIDICaptain3/SparkComms.ino"
void connect_pedal();
#line 264 "/Users/leko/Downloads/SparkMIDICaptain-main/SparkMIDICaptain3/SparkComms.ino"
bool connect_to_all();
#line 459 "/Users/leko/Downloads/SparkMIDICaptain-main/SparkMIDICaptain3/SparkComms.ino"
void send_to_spark(byte *buf, int len);
#line 464 "/Users/leko/Downloads/SparkMIDICaptain-main/SparkMIDICaptain3/SparkComms.ino"
void send_to_app(byte *buf, int len);
#line 478 "/Users/leko/Downloads/SparkMIDICaptain-main/SparkMIDICaptain3/SparkComms.ino"
int ble_getRSSI();
#line 88 "/Users/leko/Downloads/SparkMIDICaptain-main/SparkMIDICaptain3/SparkIO.ino"
void dump_raw_block(byte *block, int block_length);
#line 116 "/Users/leko/Downloads/SparkMIDICaptain-main/SparkMIDICaptain3/SparkIO.ino"
void dump_processed_block(byte *block, int block_length);
#line 149 "/Users/leko/Downloads/SparkMIDICaptain-main/SparkMIDICaptain3/SparkIO.ino"
void uint_to_bytes(unsigned int i, uint8_t *h, uint8_t *l);
#line 154 "/Users/leko/Downloads/SparkMIDICaptain-main/SparkMIDICaptain3/SparkIO.ino"
void bytes_to_uint(uint8_t h, uint8_t l,unsigned int *i);
#line 169 "/Users/leko/Downloads/SparkMIDICaptain-main/SparkMIDICaptain3/SparkIO.ino"
void show_heap();
#line 176 "/Users/leko/Downloads/SparkMIDICaptain-main/SparkMIDICaptain3/SparkIO.ino"
int memrnd(int mem);
#line 189 "/Users/leko/Downloads/SparkMIDICaptain-main/SparkMIDICaptain3/SparkIO.ino"
uint8_t* malloc_check(int size);
#line 203 "/Users/leko/Downloads/SparkMIDICaptain-main/SparkMIDICaptain3/SparkIO.ino"
uint8_t* realloc_check(uint8_t *ptr, int new_size);
#line 217 "/Users/leko/Downloads/SparkMIDICaptain-main/SparkMIDICaptain3/SparkIO.ino"
void free_check(uint8_t *ptr);
#line 227 "/Users/leko/Downloads/SparkMIDICaptain-main/SparkMIDICaptain3/SparkIO.ino"
void new_packet(struct packet_data *pd, int length);
#line 232 "/Users/leko/Downloads/SparkMIDICaptain-main/SparkMIDICaptain3/SparkIO.ino"
void new_packet_from_data(struct packet_data *pd, uint8_t *data, int length);
#line 239 "/Users/leko/Downloads/SparkMIDICaptain-main/SparkMIDICaptain3/SparkIO.ino"
void clear_packet(struct packet_data *pd);
#line 256 "/Users/leko/Downloads/SparkMIDICaptain-main/SparkMIDICaptain3/SparkIO.ino"
bool scan_packet(CircularArray &buf, int *start, int *this_end, int end);
#line 357 "/Users/leko/Downloads/SparkMIDICaptain-main/SparkMIDICaptain3/SparkIO.ino"
void handle_spark_packet();
#line 412 "/Users/leko/Downloads/SparkMIDICaptain-main/SparkMIDICaptain3/SparkIO.ino"
void handle_app_packet();
#line 468 "/Users/leko/Downloads/SparkMIDICaptain-main/SparkMIDICaptain3/SparkIO.ino"
void app_callback(uint8_t *buf, int size);
#line 475 "/Users/leko/Downloads/SparkMIDICaptain-main/SparkMIDICaptain3/SparkIO.ino"
void spark_callback(uint8_t *buf, int size);
#line 508 "/Users/leko/Downloads/SparkMIDICaptain-main/SparkMIDICaptain3/SparkIO.ino"
int remove_headers(CircularArray &out_block, CircularArray &in_block, int in_len);
#line 568 "/Users/leko/Downloads/SparkMIDICaptain-main/SparkMIDICaptain3/SparkIO.ino"
void fix_bit_eight(CircularArray &in_block, int in_len);
#line 611 "/Users/leko/Downloads/SparkMIDICaptain-main/SparkMIDICaptain3/SparkIO.ino"
int compact(CircularArray &out_block, CircularArray &in_block, int in_len);
#line 672 "/Users/leko/Downloads/SparkMIDICaptain-main/SparkMIDICaptain3/SparkIO.ino"
void process_sparkIO();
#line 1735 "/Users/leko/Downloads/SparkMIDICaptain-main/SparkMIDICaptain3/SparkIO.ino"
int expand(byte *out_block, byte *in_block, int in_len);
#line 1823 "/Users/leko/Downloads/SparkMIDICaptain-main/SparkMIDICaptain3/SparkIO.ino"
void add_bit_eight(byte *in_block, int in_len);
#line 1904 "/Users/leko/Downloads/SparkMIDICaptain-main/SparkMIDICaptain3/SparkIO.ino"
int add_headers(byte *out_block, byte *in_block, int in_len);
#line 1964 "/Users/leko/Downloads/SparkMIDICaptain-main/SparkMIDICaptain3/SparkIO.ino"
void spark_send();
#line 2013 "/Users/leko/Downloads/SparkMIDICaptain-main/SparkMIDICaptain3/SparkIO.ino"
void app_send();
#line 47 "/Users/leko/Downloads/SparkMIDICaptain-main/SparkMIDICaptain3/SparkMIDICaptain3.ino"
int smooth(int current, int previous) {
  return (previous * 0.8) + (current * 0.2);
}

void updateExpressionPedal() {
  expRaw = analogRead(EXP_PIN);  // 0–4095
  expFiltered = smooth(expRaw, expFiltered);

  int expPercent = map(expFiltered, 0, 4095, 0, 100);

  if (abs(expPercent - expLastSent) > 1) {
    expLastSent = expPercent;

    // Example: control Delay Mix (parameter 0)
    float val = expPercent / 100.0f;

    spark_msg_out.change_effect_parameter("Wah", 0, val);
    spark_send();

    Serial.print("EXP: ");
    Serial.println(expPercent);
  }
}



// --------------------------------------------------
// OLED
// --------------------------------------------------
Adafruit_SSD1306 display(128, 64, &Wire);

// --------------------------------------------------
// NeoPixels
// --------------------------------------------------
Adafruit_NeoPixel strip(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);

// --------------------------------------------------
// State Machine
// --------------------------------------------------
enum UIMode {
  MODE_SPARK,
  MODE_TUNER,
  MODE_DAW,
  MODE_BOTH
};

UIMode uiMode = MODE_SPARK;

// --------------------------------------------------
// Spark Preset System
// --------------------------------------------------
int bank = 0;          // 0–7
int presetInBank = 0;  // 0–3
bool modState = false;

int getGlobalPreset() {
  return bank * 4 + presetInBank;
}

// --------------------------------------------------
// Button Timing
// --------------------------------------------------
unsigned long lastModPress = 0;

// --------------------------------------------------
// OLED Update
// --------------------------------------------------
void updateScreen() {
  display.clearDisplay();
  display.setCursor(0,0);

  if (tunerActive) {
    display.setTextSize(2);
    display.print("Tuner\n");

    display.setTextSize(3);
    display.print(tunerNote);

    display.setTextSize(2);
    display.setCursor(0, 40);
    display.print("Cents: ");
    display.println(tunerCents);

    display.display();
    return;
  }

  // Normal UI
  display.setTextSize(2);
  display.print("Mode: ");
  switch (uiMode) {
    case MODE_SPARK: display.println("SPARK"); break;
    case MODE_TUNER: display.println("TUNER"); break;
    case MODE_DAW:   display.println("DAW");   break;
    case MODE_BOTH:  display.println("BOTH");  break;
  }

  display.print("Bank: ");
  display.println(bank);

  display.print("Preset: ");
  display.println(presetInBank);

  display.print("Mod: ");
  display.println(modState ? "ON" : "OFF");

  display.display();
}

// --------------------------------------------------
// NeoPixel Tuner Animation
// --------------------------------------------------
void animateTunerLEDs() {
  strip.clear();

  int center = NUM_LEDS / 2;
  int offset = map(tunerCents, -50, 50, -center, center);
  int lit = center + offset;

  for (int i = 0; i < NUM_LEDS; i++) {
    if (i == lit) {
      if (abs(tunerCents) < 3) {
        strip.setPixelColor(i, strip.Color(0, 255, 0));      // in tune
      } else if (tunerCents < 0) {
        strip.setPixelColor(i, strip.Color(255, 0, 0));      // flat
      } else {
        strip.setPixelColor(i, strip.Color(255, 255, 0));    // sharp
      }
    }
  }

  strip.show();
}

// --------------------------------------------------
// Input Router
// --------------------------------------------------
void routeInput_Spark(int event) {
  switch (event) {
    case 1: // preset up
      presetInBank++;
      if (presetInBank > 3) presetInBank = 0;
      change_hardware_preset(getGlobalPreset());
      break;

    case 2: // preset down
      presetInBank--;
      if (presetInBank < 0) presetInBank = 3;
      change_hardware_preset(getGlobalPreset());
      break;

    case 3: // mod toggle
      modState = !modState;
      change_mod_toggle();
      break;

    case 4: // bank up
      bank++;
      if (bank > 7) bank = 0;
      change_hardware_preset(getGlobalPreset());
      break;
    case 5: // bank up
      tunerActive = !tunerActive;
      tuner_on_off(tunerActive);
      break;
  }
}

void routeInput_DAW(int event) {
  // Placeholder for DAW/MIDI control
}

void routeInput_BOTH(int event) {
  routeInput_Spark(event);
  routeInput_DAW(event);
}

// --------------------------------------------------
// Button Handling
// --------------------------------------------------
void handleButtons() {

  // BTN_UP → preset up
  if (!digitalRead(BTN_UP)) {
    if (uiMode == MODE_SPARK) routeInput_Spark(1);
    if (uiMode == MODE_DAW)   routeInput_DAW(1);
    if (uiMode == MODE_BOTH)  routeInput_BOTH(1);
    updateScreen();
    delay(250);
  }

  // BTN_DOWN → preset down
  if (!digitalRead(BTN_DOWN)) {
    if (uiMode == MODE_SPARK) routeInput_Spark(2);
    if (uiMode == MODE_DAW)   routeInput_DAW(2);
    if (uiMode == MODE_BOTH)  routeInput_BOTH(2);
    updateScreen();
    delay(250);
  }
  // BTN_DOWN → preset down
  if (!digitalRead(BTN_TUN)) {
    if (uiMode == MODE_SPARK) routeInput_Spark(5);
    if (uiMode == MODE_DAW)   routeInput_DAW(5);
    if (uiMode == MODE_BOTH)  routeInput_BOTH(5);
    updateScreen();
    delay(250);
  }

  
}

// --------------------------------------------------
// Setup
// --------------------------------------------------
void setup() {
  Serial.begin(115200);

  pinMode(BTN_UP,   INPUT_PULLUP);
  pinMode(BTN_DOWN, INPUT_PULLUP);
  pinMode(BTN_TUN,  INPUT_PULLUP);
  Wire.begin();   // 21 SDA, 22 SCL

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0,0);
  display.println("Spark Pedal");
  display.display();

  strip.begin();
  strip.clear();
  strip.show();

  delay(500);

  spark_state_tracker_start();
  Serial.println("Spark BLE Ready");

  updateScreen();
}

// --------------------------------------------------
// Main Loop
// --------------------------------------------------
void loop() {

  if (update_spark_state()) {

    Serial.print("Spark msg: ");
    Serial.println(cmdsub, HEX);
  }
  // Expression Pedal
  updateExpressionPedal();  
  // Buttons
  handleButtons();
}

#line 1 "/Users/leko/Downloads/SparkMIDICaptain-main/SparkMIDICaptain3/CircularArray.ino"
#include "CircularArray.h"

#define DEBUG_ARRAY(...)  {char _b[100]; sprintf(_b, __VA_ARGS__); Serial.println(_b);}
#define DEB_ARRAY(...)  {char _b[100]; sprintf(_b, __VA_ARGS__); Serial.print(_b);}
//define DEBUG_ARRAY(...) {}
//define DEB_ARRAY(...) {}

CircularArray::CircularArray() {
    start = 0;
    end = 0;
    size = sizeof(buf);
  }

int CircularArray::length() {
    return end >= start ? end - start : end - start + size;
  }

  uint8_t& CircularArray::operator [](int index) {
    int ind;

    if (index > length ()) {
      DEBUG_ARRAY("Index out of range: %d", index);
    }

    ind = (index + start) % size;
    return buf[ind];
  }

  int CircularArray::expand (int len) {
    int le;
    le = len;
    if (length() + le > size) {
      le = size - length();
      DEBUG_ARRAY("Expand past buffer size: requested %d limted to %d", len, le);
    }

    end += le;
    if (end > size) end -= size; 
    return le;
  }

  int CircularArray::shrink(int len) {
    int le;
    le = len;
    if (le > length()) {
      DEBUG_ARRAY("Shrink past buffer size: requested %d limited to %d", len, length());
      le = length();
    }
    start += le;
    if (start > size) start -= size; 
    return le;
  }

/*
  int CircularArray::copy_in(uint8_t *data, int index, int len) {
    int le; 
    int i;
    le = len;
    if (index + len > length()) {
      le = length() - index;
      DEBUG_ARRAY("Trying to copy_in too much: %d limited to %d", len, le);
    }
    for (i = 0; i < le; i++) {
      buf[(start + index + i) % size] = data[i];
    }
    return le;
  }

  int CircularArray::copy_out(uint8_t *data, int index, int len) {
    int le;
    int i;

    le = len;
    if (index + len > length()) {
      le = length() - index;
      DEBUG_ARRAY("Trying to copy_out too much: %d limited to %d", len, le);
    }
    for (i = 0; i < le; i++) {
      data[i] = buf[(start + index + i) % size];
    }
    return le;
  }
*/

  int CircularArray::append(uint8_t *data, int len) {
    int le;
    int i;

    // check length
    le = len;
    if (length() + le > size) {
      le = size - length();
      DEBUG_ARRAY("Append past buffer size: requested %d limted to %d", len, le);
    }
    // copy
    for (i = 0; i < le; i++) {
      buf[(end + i) % size] = data[i];
    }
    // expand
    end += le;
    if (end > size) end -= size; 
    return le;
  }

  int CircularArray::extract(uint8_t *data, int len_to_copy, int len_to_shrink) {
    int i;

    // check length
    if (len_to_copy > length()) {
      len_to_copy = length();
      DEBUG_ARRAY("Trying to extract too much, limited to %d", len_to_copy);
    }
    // copy
    for (i = 0; i < len_to_copy; i++) {
      data[i] = buf[(start + i) % size];
    }
    // shrink
    if (len_to_shrink > length()) {
      len_to_shrink = length();
      DEBUG_ARRAY("Trying to shink too much, limited to %d", len_to_shrink);
    }
    start += len_to_shrink;
    if (start > size) start -= size; 

    return len_to_copy;
  }

 int CircularArray::extract_append(CircularArray &to, int len_to_copy, int len_to_shrink) {
    // check length to copy out
    if (len_to_copy > length()) {
      len_to_copy = length();
      DEBUG_ARRAY("Trying to extract too much, limited to %d", len_to_copy);
    }
    // check length to copy in destination
    if (to.length() + len_to_copy > to.size) {
      len_to_copy = to.size - to.length();
      DEBUG_ARRAY("Trying to append too much, limited to %d", len_to_copy);
    }
    // copy
    for (int i = 0; i < len_to_copy; i++) {
      to.buf[(to.end + i) % to.size] = buf[(start + i) % size];
    }
    // expand destination
    to.end += len_to_copy;
    if (to.end > to.size) to.end -= to.size;

    // shrink source
    if (len_to_shrink > length()) {
      len_to_shrink = length();
      DEBUG_ARRAY("Trying to shink too much, limited to %d", len_to_shrink);
    }
    start += len_to_shrink;
    if (start > size) start -= size; 

    return len_to_copy;
 }

  void CircularArray::show() {
    int i;
    for (i = 0; i < length(); i++) 
      DEB_ARRAY("%d ", buf[(start + i) % size]);
    DEBUG_ARRAY("");
  }

  void CircularArray::clear() {
    start = 0;
    end = 0;
  }

#line 1 "/Users/leko/Downloads/SparkMIDICaptain-main/SparkMIDICaptain3/MIDI.ino"
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

#line 1 "/Users/leko/Downloads/SparkMIDICaptain-main/SparkMIDICaptain3/RingBuffer.ino"
#include "RingBuffer.h" 

//
// RingBuffer class
//

/* Implementation of a ring buffer - with a difference
 * New data is written to a temp area with add_to_buffer() and committed into the buffer once done - commit()
 * Commited data is read from the head of the buffer with get_from_buffer()
 * Data in this area can be read and updated by index - get_at_index() and set_at_index()
 * 
 * If the temp data is not required it can be ignored using drop() rather than commit()
 * 
 *    +----------------------------------++----------------------------------------------------------------------+
 *    |  0 |  1 |  2 |  3 |  4 |  5 |  6 ||  7 |  8 |  9 | 10 | 11 | 12 | 13 || 14 | 15 | 16 | 17 | 18 | 19 | 20 |                                              |
 *    +----------------------------------++----------------------------------------------------------------------+  
 *       ^                             ^     ^                             ^
 *       st ---------- len ------------+     +----------- t_len --------- en
 * 
 *           committed data                      temporary data                                   empty
 */

RingBuffer::RingBuffer() {
  st = 0;
  en = 0;
  len = 0;
  t_len = 0;
}

bool RingBuffer::add(uint8_t b) {
  if (len + t_len < RB_BUFF_MAX) {
    rb[en] = b;
    t_len++;
    en++; 
    if (en >= RB_BUFF_MAX) en = 0; 
    return true;
  }
  else
    return false;
}

bool RingBuffer::get(uint8_t *b) {
  if (len > 0) {
    *b = rb[st];
    len--;
    st++; 
    if (st >= RB_BUFF_MAX) st = 0; 
    return true;
  }
  else
    return false;  
}

// set a value at a location in the temp area
bool RingBuffer::set_at_index(int ind, uint8_t b) {
  if (ind >= 0 && ind < t_len) {
    rb[(st+len+ind) % RB_BUFF_MAX] = b;
    return true;
  }
  else
    return false;
}

// get a value from a location in the temp area
bool RingBuffer::get_at_index(int ind, uint8_t *b) {
  if (ind >= 0 && ind < t_len) {
    *b = rb[(st+len+ind) % RB_BUFF_MAX];
    return true;
  }
  else
    return false;
}

bool RingBuffer::set_bit_at_index(int ind, uint8_t b) {
  if (ind >= 0 && ind < t_len) {
    rb[(st+len+ind) % RB_BUFF_MAX] |= b;    
    return true;
  }
  else
    return false; 
}

int RingBuffer::get_len() { // total temp len
  return t_len;
}

int RingBuffer::get_pos() { // current position
  return t_len;
}

void RingBuffer::commit() {
  len += t_len;
  t_len = 0;
}

int RingBuffer::get_data_len() { // total  len
  return len;
}

void RingBuffer::drop() {
  en = st + len;
  t_len = 0;
}

void RingBuffer::clear() {
  en = st;
  len = 0;
}

bool RingBuffer::is_empty() {
  return (len == 0);
}

void RingBuffer::dump() {
  int i;

  for (i=0; i<len; i++) {
    Serial.print("S ");
    Serial.print(st+i);
    Serial.print(" ");
    Serial.print((st+i) % RB_BUFF_MAX);
    Serial.print(" ");    
    Serial.println(rb[(st+i) % RB_BUFF_MAX], HEX);
  };
  for (i=0; i<t_len; i++) {
    Serial.print("T ");
    Serial.print(st+len+i);
    Serial.print(" ");
    Serial.print((st+len+i) % RB_BUFF_MAX);
    Serial.print(" ");    
    Serial.println(rb[(st+len+i) % RB_BUFF_MAX], HEX);
  };
}

void RingBuffer::dump2() {
  int i;
  uint8_t v;

//  Serial.println();
  for (i=0; i<len; i++) {
    v=rb[(st+i) % RB_BUFF_MAX];
    if (v < 16) Serial.print("0");
    Serial.print(v, HEX);
    Serial.print(" ");
  };
  for (i=0; i<t_len; i++) {
    v=rb[(st+len+i) % RB_BUFF_MAX];
    if (v < 16) Serial.print("0");
    Serial.print(v, HEX);
    Serial.print(" ");
  };
  Serial.println();
}

void RingBuffer::dump3() {
  int i;
  uint8_t v;

  Serial.print("               ");
  for (i=0; i<len; i++) {
    v=rb[(st+i) % RB_BUFF_MAX];
    if (v < 16) Serial.print("0");
    Serial.print(v, HEX);
    Serial.print(" ");
    if (i % 16 == 15) {
      Serial.println();
      Serial.print("               ");
    };
  };
  Serial.println();
}

#line 1 "/Users/leko/Downloads/SparkMIDICaptain-main/SparkMIDICaptain3/Screen.ino"
#include "Screen.h"



void splash_screen() {
#ifdef OLED_ON
  Wire.begin(OLED_SDA, OLED_SCL);  // set my SDA, SCL

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { 
    Serial.println("SSD1306 allocation failed");
    while (true); 
  }

  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(2);
  display.setCursor(40, 10);
  display.print("MIDI");
  display.setCursor(20, 40);
  display.print("CAPTAIN");
  display.display();
#endif
}

void show_connected() {
#ifdef OLED_ON
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(2);
  display.setCursor(0, 10);
  display.print("CONNECTED");
  display.display();
#endif
}


void show_message(char *msg, int preset) {
#ifdef OLED_ON
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(2);
  display.setCursor(0, 10);
  display.print("MIDI:");
  display.setCursor(0, 40);
  display.print(msg);

  display.setTextSize(3);
  display.setCursor(100, 00);
  display.print(preset);

  display.display();
#endif
}


#line 1 "/Users/leko/Downloads/SparkMIDICaptain-main/SparkMIDICaptain3/Spark.ino"
#include "Spark.h"


void dump_preset(SparkPreset preset)
{
  Serial.print("Preset: ");
  Serial.println(preset.Name);
  Serial.print("Current : preset ");
  Serial.print(preset.curr_preset);
  Serial.print(" ");
  Serial.println(preset.preset_num);  
  for (int i = 0; i < 7; i++) {
    Serial.print("  ");
    Serial.println(preset.effects[i].EffectName);
  }
}


///// ROUTINES TO SYNC TO AMP SETTINGS

int selected_preset;
int preset_requested;
bool preset_received;
unsigned long sync_timer;

// SparkBox specific
int hw_preset_requested; // for UI sync of bank of presets
bool hw_preset_received;
unsigned long hw_preset_timer;


int get_effect_index(char *str) {
  int ind, i;

  ind = -1;
  for (i = 0; ind == -1 && i <= 6; i++) {
    if (strcmp(presets[CUR_EDITING][current_input].effects[i].EffectName, str) == 0) {
      ind  = i;
    }
  }
  return ind;
}

bool wait_for_spark(int command_expected) {
  bool got_it = false;
  unsigned long time_now;

  time_now = millis();

  while (!got_it && millis() - time_now < 2000) {
    process_sparkIO();
    if (spark_msg_in.get_message(&cmdsub, &msg, &preset)) {
      got_it = (cmdsub == command_expected);    
    }
  }
  // if got_it is false, this is a timeout
  return got_it;
};

bool wait_for_app(int command_expected) {
  bool got_it = false;
  unsigned long time_now;

  time_now = millis();

  while (!got_it && millis() - time_now < 2000) {
    process_sparkIO();
    if (app_msg_in.get_message(&cmdsub, &msg, &preset)) {
      DEBUG(cmdsub, HEX);
      got_it = (cmdsub == command_expected);    
    }
  }
  // if got_it is false, this is a timeout
  return got_it;
};


bool spark_state_tracker_start() {
  bool got;
  int pres;

  int preset_to_get;
  bool got_all_presets;

  spark_state = SPARK_DISCONNECTED;
  ble_passthru = true;
  // try to find and connect to Spark - returns false if failed to find Spark
  if (!connect_to_all()) return false;

  #ifdef CLASSIC
  DEBUG("Bluetooth library is classic Bluedroid");
  #else
  DEBUG("Bluetooth library is NimBLE");
  #endif

  DEBUG("SparkIO version: CircularArray and malloc");
  
  // handle different spark types
  DEB("SPARK TYPE: ");
  switch (spark_type) {
    case S40:
      DEBUG("Spark 40");
      break;
    case GO:
      DEBUG("Spark GO");
      break;
    case MINI:
      DEBUG("Spark MINI");
      break;
    case LIVE:
      DEBUG("Spark LIVE");
      break;      
    case SPARK2:
      DEBUG("Spark 2");
      break;    
    case NONE:
      DEBUG("Unknown");
      break; 
  }

  if (spark_type == LIVE || spark_type == SPARK2) { 
    num_inputs = 2;
    num_presets = 8;          
    max_preset = 7;
  }  
  else {
    num_inputs = 1;
    num_presets = 4;                 
    max_preset = 3;
  }

  spark_state = SPARK_CONNECTED;     // it has to be to have reached here
  spark_ping_timer = millis();
  selected_preset = 0;

  // Get serial number
  spark_msg_out.get_serial();
  spark_send();
  got = wait_for_spark(0x0323);
  if (got) DEBUG("Got serial number");
  else DEBUG("Failed to get serial number");

  // Get firmware version
  spark_msg_out.get_firmware();
  spark_send();
  got = wait_for_spark(0x032f);
  if (got) DEBUG("Got firmware version");
  else DEBUG("Failed to get firmware version");

  // Get
  spark_msg_out.get_checksum_info();
  spark_send();
  got = wait_for_spark(0x032a);
  if (got) DEBUG("Got checksum");
  else DEBUG("Failed to get checksum");

  // Get serial number
  spark_msg_out.get_serial();
  spark_send();
  got = wait_for_spark(0x0323);
  if (got) DEBUG("Got serial number");
  else DEBUG("Failed to get serial number");

  // Get the presets
  preset_to_get = 0x0000;
  got_all_presets = false;
  while (!got_all_presets) {
    spark_msg_out.get_preset_details(preset_to_get);
    spark_send();
    got = wait_for_spark(0x0301);

    if (got) {
      pres = preset.preset_num; // won't get an 0x7f
      if (preset.curr_preset == 0x01) {
        pres = CUR_EDITING;
        got_all_presets = true;
      }
      presets[pres][0] = preset;

      DEB("Got preset: "); 
      DEBUG(pres);
      //dump_preset(presets[pres][0]);
      
      preset_to_get++;
      if (preset_to_get > max_preset + 0x0000) preset_to_get = 0x0100;
    }
    else {
      DEB("Missed preset: "); 
      DEBUG(preset_to_get);
    };
  }

  if (spark_type == LIVE) got_all_presets = false;

  // Get the presets from INPUT 2 on LIVE
  preset_to_get = 0x0300;    // The LIVE presets
  while (!got_all_presets) {
    spark_msg_out.get_preset_details(preset_to_get);
    spark_send();
    got = wait_for_spark(0x0301);

    if (got) {
      pres = preset.preset_num; // won't get an 0x7f
      if (preset.curr_preset == 0x04) {
        pres = CUR_EDITING;
        got_all_presets = true;
      }
      presets[pres][1] = preset;

      DEB("Got preset: "); 
      DEBUG(pres);
      //dump_preset(presets[pres][1]);
      
      preset_to_get++;
      if (preset_to_get > max_preset + 0x0300) preset_to_get = 0x0400;
    }
    else {
      DEB("Missed preset: "); 
      DEBUG(preset_to_get);
    };
  }


  spark_state = SPARK_SYNCED;
  DEBUG("End of setup");

  spark_ping_timer = millis();
  ble_passthru = true;
  return true;
}

// get changes from app or Spark and update internal state to reflect this
// this function has the side-effect of loading cmdsub, msg and preset which can be used later

bool  update_spark_state() {
  int pres, ind;
  int input;
  
  // sort out connection and sync progress
  if (!ble_spark_connected) {
    spark_state = SPARK_DISCONNECTED;
    
    if (millis() - spark_ping_timer > 500) {
      DEBUG("Spark disconnected, try to reconnect...");
      spark_ping_timer = millis();
      connect_spark();  // reconnects if any disconnects happen    
      if (ble_spark_connected)
        spark_state = SPARK_SYNCED; 
    }
  }


  process_sparkIO();
  
  // K&R: Expressions connected by && or || are evaluated left to right, 
  // and it is guaranteed that evaluation will stop as soon as the truth or falsehood is known.
  
  if (spark_msg_in.get_message(&cmdsub, &msg, &preset) || app_msg_in.get_message(&cmdsub, &msg, &preset)) {
    DEB("Message: ");
    DEBUG(cmdsub, HEX);

    // all the processing for sync
    switch (cmdsub) {
      // full preset details

      case 0x0301:  
      case 0x0101:
        pres = (preset.preset_num == 0x7f) ? TMP_PRESET : preset.preset_num;
        if (preset.curr_preset == 0x01 || preset.curr_preset == 0x04)
          pres = CUR_EDITING;
        input = (preset.curr_preset > 2);     // this makes input either 0 for 0x00 and 0x01 or 1 for 0x03 and 0x04
        
        DEB("Got preset ");
        DEB(input);
        DEB(" : ");
        DEB(pres);
        DEB(" = ");
        DEB(preset.curr_preset);
        DEB(" : ");
        DEBUG(preset.preset_num);

        presets[pres][input] = preset;          // don't use current input to store
        
        dump_preset(presets[pres][input]);
        
        break;
      // change of amp model
      case 0x0306:
        strcpy(presets[CUR_EDITING][current_input].effects[3].EffectName, msg.str2);
        break;
      // change of effect
      case 0x0106:
        ind = get_effect_index(msg.str1);
        if (ind >= 0) 
          strcpy(presets[CUR_EDITING][current_input].effects[ind].EffectName, msg.str2);
          setting_modified = true;
        break;
      // effect on/off  
      case 0x0315:
      case 0x0115:
        ind = get_effect_index(msg.str1);
        if (ind >= 0) 
          presets[CUR_EDITING][current_input].effects[ind].OnOff = msg.onoff;
          setting_modified = true;
        break;
      // change parameter value  
      case 0x0337:
      case 0x0104:
        ind = get_effect_index(msg.str1);
        if (ind >= 0)
          presets[CUR_EDITING][current_input].effects[ind].Parameters[msg.param1] = msg.val;
        setting_modified = true;  
        // SparkBox specific
        strcpy(param_str, msg.str1);
        param = msg.param1;
        break;  
      // change to preset  
      case 0x0338:
      case 0x0138:
        selected_preset = (msg.param2 == 0x7f) ? TMP_PRESET : msg.param2;
        presets[CUR_EDITING][current_input] = presets[selected_preset][current_input];
        setting_modified = false;
        // SparkBox specific
        // Only update the displayed preset number for HW presets
        if (selected_preset < num_presets) {
          display_preset_num = selected_preset; 
        }                        
        break;
      // Send licence key  
      case 0x0170:
        break; 
      // store to preset  
      case 0x0327:
        selected_preset = (msg.param2 == 0x7f) ? TMP_PRESET : msg.param2;
        presets[selected_preset][current_input] = presets[CUR_EDITING][current_input];
        setting_modified = false;
        // SparkBox specific
        // Only update the displayed preset number for HW presets
        if (selected_preset < num_presets) {
          display_preset_num = selected_preset; 
        }  
        break;
      // current selected preset
      case 0x0310:
        selected_preset = (msg.param2 == 0x7f) ? TMP_PRESET : msg.param2;
        if (msg.param1 == 0x01 || msg.param1 == 0x04) 
          selected_preset = CUR_EDITING;
        presets[CUR_EDITING][current_input] = presets[selected_preset][current_input];
        // SparkBox specific
        // Only update the displayed preset number for HW presets
        if (selected_preset < num_presets) {
          display_preset_num = selected_preset; 
        }
        break;
      case 0x0364:
        // SparkBox specific
        isTunerMode = true;
        break;    
         
      case 0x0365:
      case 0x0465:
        // SparkBox specific
        isTunerMode = false;
        break;       
      
      case 0x0438:
        setting_modified = false;
        break;

      default:
        break;
    }
    return true;
  }
  else
    return false;
}

void update_ui() {
  bool got;

  if (ble_app_connected) {
    ble_passthru = false;
    app_msg_out.save_hardware_preset(0x00, 0x03);
    app_send();

    DEBUG("Updating UI");
    got = wait_for_app(0x0201);
    if (got) {
      strcpy(presets[CUR_EDITING][current_input].Name, "SyncPreset");
      strcpy(presets[CUR_EDITING][current_input].UUID, "F00DF00D-FEED-0123-4567-987654321000");  
      presets[CUR_EDITING][current_input].curr_preset = 0x00;
      presets[CUR_EDITING][current_input].preset_num = 0x03;
      app_msg_out.create_preset(&presets[CUR_EDITING][current_input]);
      app_send();
      delay(100);
      app_msg_out.change_hardware_preset(0x00, 0x00);
      app_send();
      app_msg_out.change_hardware_preset(0x00, 0x03);     
      app_send();
    }
    else {
      DEBUG("Didn't capture the new preset");
    }
    ble_passthru = true;
  }
}

// SparkBox specific
void update_ui_hardware() {
  bool got;
  int p;
  int i;
  bool done;

  if (ble_app_connected) {
    done = false;
    ble_passthru = false;

    DEBUG("Updating UI for hardware");

    i = 0;

    while (i < num_presets) {
      app_msg_out.save_hardware_preset(0x00, i);
      app_send();

      got = wait_for_app(0x0201);
      if (got) {
        DEB("Got hardware preset request ");
        DEB(msg.param2);
        DEB(" Looking for: ");
        DEBUG(i);
        presets[i][current_input].curr_preset = 0x00;
        presets[i][current_input].preset_num = i;
        app_msg_out.create_preset(&presets[i][current_input]);
        app_send();
        delay(1000);
        i++;
      }
      else {
        DEBUG("Didn't capture the new preset");
      }
    }
  }
}


void set_input1() {
  /*
  // DOn't think I ever implemented select_live_input_1() !!

  if (spark_type == LIVE || spark_type == SPARK2) {
    spark_msg_out.select_live_input_1();
    spark_send();
  }
  */
}


///// ROUTINES TO CHANGE AMP SETTINGS

void change_generic_model(char *new_eff, int slot) {
  if (strcmp(presets[CUR_EDITING][current_input].effects[slot].EffectName, new_eff) != 0) {
    set_input1();
    spark_msg_out.change_effect_input(presets[CUR_EDITING][current_input].effects[slot].EffectName, new_eff, current_input);
    strcpy(presets[CUR_EDITING][current_input].effects[slot].EffectName, new_eff);
    spark_send();
    delay(100);
  }
}

void change_comp_model(char *new_eff) {
  change_generic_model(new_eff, 1);
}

void change_drive_model(char *new_eff) {
  change_generic_model(new_eff, 2);
}

void change_amp_model(char *new_eff) {
  if (strcmp(presets[CUR_EDITING][current_input].effects[3].EffectName, new_eff) != 0) {
    spark_msg_out.change_effect_input(presets[CUR_EDITING][current_input].effects[3].EffectName, new_eff, current_input);
    app_msg_out.change_effect_input(presets[CUR_EDITING][current_input].effects[3].EffectName, new_eff, current_input);
    strcpy(presets[CUR_EDITING][current_input].effects[3].EffectName, new_eff);
    spark_send();
    app_send();
    delay(100);
  }
}

void change_mod_model(char *new_eff) {
  change_generic_model(new_eff, 4);
}

void change_delay_model(char *new_eff) {
  change_generic_model(new_eff, 5);
}



void change_generic_onoff(int slot,bool onoff) {
  
  spark_msg_out.turn_effect_onoff_input(presets[CUR_EDITING][current_input].effects[slot].EffectName, onoff, current_input);
  app_msg_out.turn_effect_onoff_input(presets[CUR_EDITING][current_input].effects[slot].EffectName, onoff, current_input);
  presets[CUR_EDITING][current_input].effects[slot].OnOff = onoff;
  spark_send();
  app_send();  
}

void change_noisegate_onoff(bool onoff) {
  change_generic_onoff(0, onoff);  
}

void change_comp_onoff(bool onoff) {
  change_generic_onoff(1, onoff);  
}

void change_drive_onoff(bool onoff) {
  change_generic_onoff(2, onoff);  
}

void change_amp_onoff(bool onoff) {
  change_generic_onoff(3, onoff);  
}

void change_mod_onoff(bool onoff) {
  change_generic_onoff(4, onoff);  
}

void change_delay_onoff(bool onoff) {
  change_generic_onoff(5, onoff);  
}

void change_reverb_onoff(bool onoff) {
  change_generic_onoff(6, onoff);  
}


void change_generic_toggle(int slot) {
  bool new_onoff;

  new_onoff = !presets[CUR_EDITING][current_input].effects[slot].OnOff;
  
  spark_msg_out.turn_effect_onoff_input(presets[CUR_EDITING][current_input].effects[slot].EffectName, new_onoff, current_input);
  app_msg_out.turn_effect_onoff_input(presets[CUR_EDITING][current_input].effects[slot].EffectName, new_onoff, current_input);
  presets[CUR_EDITING][current_input].effects[slot].OnOff = new_onoff;
  spark_send();
  app_send();  
}

void change_noisegate_toggle() {
  change_generic_toggle(0);  
}

void change_comp_toggle() {
  change_generic_toggle(1);  
}

void change_drive_toggle() {
  change_generic_toggle(2);  
}

void change_amp_toggle() {
  change_generic_toggle(3);  
}

void change_mod_toggle() {
  change_generic_toggle(4);  
}

void change_delay_toggle() {
  change_generic_toggle(5);  
}

void change_reverb_toggle() {
  change_generic_toggle(6);  
}


void change_generic_param(int slot, int param, float val) {
  float diff;

  // some code to reduce the number of changes
  diff = presets[CUR_EDITING][current_input].effects[slot].Parameters[param] - val;
  if (diff < 0) diff = -diff;
  if (diff > 0.04) {
    spark_msg_out.change_effect_parameter_input(presets[CUR_EDITING][current_input].effects[slot].EffectName, param, val, current_input);
    app_msg_out.change_effect_parameter_input(presets[CUR_EDITING][current_input].effects[slot].EffectName, param, val, current_input);
    presets[CUR_EDITING][current_input].effects[slot].Parameters[param] = val;
    spark_send();  
    app_send();
  }
}

void change_noisegate_param(int param, float val) {
  change_generic_param(0, param, val);
}

void change_comp_param(int param, float val) {
  change_generic_param(1, param, val);
}

void change_drive_param(int param, float val) {
  change_generic_param(2, param, val);
}

void change_amp_param(int param, float val) {
  change_generic_param(3, param, val);
}

void change_mod_param(int param, float val) {
  change_generic_param(4, param, val);
}

void change_delay_param(int param, float val) {
  change_generic_param(5, param, val);
}

void change_reverb_param(int param, float val){
  change_generic_param(6, param, val);
}


void change_hardware_preset(int pres_num) {
  if (pres_num >= 0 && pres_num <= max_preset) {  
    presets[CUR_EDITING][current_input] = presets[pres_num][current_input];
    display_preset_num = pres_num;
    
    spark_msg_out.change_hardware_preset(0, pres_num);
    app_msg_out.change_hardware_preset(0, pres_num);  
    spark_send();  
    app_send();
  }
}

void change_custom_preset(SparkPreset *preset, int pres_num) {
  if (pres_num >= 0 && pres_num <= max_preset) {
    preset->preset_num = (pres_num < num_presets) ? pres_num : 0x7f;
    presets[CUR_EDITING][current_input] = *preset;
    presets[pres_num][current_input] = *preset;
    
    spark_msg_out.create_preset(preset);
    spark_send();  
    spark_msg_out.change_hardware_preset(0, preset->preset_num);
    spark_send();  
  }
}

void tuner_on_off(bool on_off) {
  spark_msg_out.tuner_on_off(on_off); 
  spark_send();  
}


void send_tap_tempo(float tempo) {
  spark_msg_out.send_tap_tempo(tempo);
  spark_send();    
};
#line 1 "/Users/leko/Downloads/SparkMIDICaptain-main/SparkMIDICaptain3/SparkComms.ino"
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
#line 1 "/Users/leko/Downloads/SparkMIDICaptain-main/SparkMIDICaptain3/SparkIO.ino"
#include "SparkIO.h"

/*  SparkIO
 *  
 *  SparkIO handles communication to and from the Positive Grid Spark amp over bluetooth for ESP32 boards
 *  
 *  From the programmers perspective, you create and read two formats - a Spark Message or a Spark Preset.
 *  The Preset has all the data for a full preset (amps, effects, values) and can be sent or received from the amp.
 *  The Message handles all other changes - change amp, change effect, change value of an effect parameter, change hardware preset and so on
 *  
 *  Conection is handled with the two commands:
 *  
 *    connect_to_all();
 *  
 *  Messages and presets from the amp and the app are then queued and processed.
 *  The essential thing is the have the spark_process() and app_process() function somewhere in loop() - this handles all the processing of the input queues
 *  
 *  loop() {
 *    ...
 *    process_sparkIO();
 *    ...
 *    do something
 *    ...
 *  }
 * 
 * Sending functions:
 *     void create_preset(SparkPreset *preset);   
 *     void get_serial();    
 *     void turn_effect_onoff(char *pedal, bool onoff);    
 *     void change_hardware_preset(uint8_t preset_num);    
 *     void change_effect(char *pedal1, char *pedal2);    
 *     void change_effect_parameter(char *pedal, int param, float val);
 *     
 *     These all create a message or preset which is sent immediately to the app or amp
 *  
 * Receiving functions:
 *     bool get_message(unsigned int *cmdsub, SparkMessage *msg, SparkPreset *preset);
 * 
 *     This receives the front of the 'received' queue - if there is nothing it returns false
 *     
 *     Based on whatever was in the queue, it will populate fields of the msg parameter or the preset parameter.
 *     Eveything apart from a full preset sent from the amp will be a message.
 *     
 *     You can determine which by inspecting cmdsub - this will be 0x0301 for a preset.
 *     
 *     Other values are:
 *     
 *     cmdsub       str1                   str2              val           param1             param2                onoff
 *     0323         amp serial #
 *     0337         effect name                              effect val    effect number
 *     0306         old effect             new effect
 *     0338                                                                0                  new hw preset (0-3)
 * 
 * 
 * 
 */


/* Data sizes for streaming to and from the Spark are as below.
 *
 *                             To Spark                   To App
 * Data                        128   (0x80)               25   (0x19)  
 * 8 bit expanded              150   (0x96)               32   (0x20)
 * With header and trailer     157   (0x9d)               39   (0x29)
 * 
 * Packet size                 173   (0xad)               106  (0x6a) 
 *
 */


// ------------------------------------------------------------------------------------------------------------
// Shared global variables
//
// block_from_spark holds the raw data from the Spark amp and data is processed in-place
// block_from_app holds the raw data from the app and data is processed in-place
// ------------------------------------------------------------------------------------------------------------

#define HEADER_LEN 6
#define CHUNK_HEADER_LEN 6

CircularArray array_spark;
CircularArray array_app;

// ------------------------------------------------------------------------------------------------------------
// Routines to dump full blocks of data
// ------------------------------------------------------------------------------------------------------------

void dump_raw_block(byte *block, int block_length) {
  DEB("Raw block - length: ");
  DEBUG(block_length);

  int lc = 8;
  for (int i = 0; i < block_length; i++) {
    byte b = block[i];
    // 0xf001 chunk header
    if (b == 0xf0) {
      DEBUG();
      lc = 6;
    }
    // 0x01fe block header
    if (b == 0x01 && block[i+1] == 0xfe) {
      lc = 16;
      DEBUG();
    }
    if (b < 16) DEB("0");
    DEB(b, HEX);
    DEB(" ");
    if (--lc == 0) {
      DEBUG();
      lc = 8;
    }
  }
  DEBUG();
}

void dump_processed_block(byte *block, int block_length) {
  DEB("Processed block: length - ");
  DEBUG(block_length);

  int pos = 0;
  int len = 0;
  int lc;
  byte b;

  while (pos < block_length) {
    if (len == 0) {
      len = (block[pos+2] << 8) + block[pos+3];
      lc = HEADER_LEN;
      DEBUG();
    }
    b = block[pos];
    if (b < 16) DEB("0");
    DEB(b, HEX);
    DEB(" ");
    if (--lc == 0) {
      DEBUG();
      lc = 8;
    }
    len--;
    pos++;
  }
  DEBUG();
}

// ------------------------------------------------------------------------------------------------------------ 
// UTILITY FUNCTIONS
// ------------------------------------------------------------------------------------------------------------

void uint_to_bytes(unsigned int i, uint8_t *h, uint8_t *l) {
  *h = (i & 0xff00) / 256;
  *l = i & 0xff;
}

void bytes_to_uint(uint8_t h, uint8_t l,unsigned int *i) {
  *i = (h & 0xff) * 256 + (l & 0xff);
}


// ------------------------------------------------------------------------------------------------------------ 
// MEMORY FUNCTIONS
// ------------------------------------------------------------------------------------------------------------


//#define DEBUG_MEMORY(...)  {char _b[100]; sprintf(_b, __VA_ARGS__); Serial.println(_b);}
//#define DEBUG_HEAP(...)  {char _b[100]; sprintf(_b, __VA_ARGS__); Serial.println(_b);}
#define DEBUG_MEMORY(...) {}
#define DEBUG_HEAP(...) {}

void show_heap() {
  DEBUG_HEAP("Total heap: %d", ESP.getHeapSize());
  DEBUG_HEAP("Free heap: %d", ESP.getFreeHeap());
  DEBUG_HEAP("Total PSRAM: %d", ESP.getPsramSize());
  DEBUG_HEAP("Free PSRAM: %d", ESP.getFreePsram());
}

int memrnd(int mem) {
  int new_mem;
  return mem;

  // can round if we want to!
  if (mem <= 20) new_mem = 20;
  else if (mem <= 100) new_mem = 100;
  else if (mem <= 800) new_mem = 800;
  else new_mem = mem;
  return new_mem;
}


uint8_t *malloc_check(int size) {
#ifdef PSRAM
  uint8_t *p = (uint8_t *) ps_malloc(memrnd(size));
#else
  uint8_t *p = (uint8_t *) malloc(memrnd(size));
#endif
  DEBUG_MEMORY("Malloc: %p %d %d", p, size, memrnd(size));
  if (p == NULL) {
    DEBUG_MEMORY("MALLOC FAILED: %p %d", p, size);
  }
  //show_heap();    
  return p;
}

uint8_t *realloc_check(uint8_t *ptr, int new_size) {
#ifdef PSRAM
  uint8_t *p = (uint8_t *) ps_realloc(ptr, memrnd(new_size));
#else
  uint8_t *p = (uint8_t *) realloc(ptr, memrnd(new_size));
#endif
  DEBUG_MEMORY("Realloc: %p %p %d %d", p, ptr, new_size, memrnd(new_size));
  if (p == NULL) {
    DEBUG_MEMORY("REALLOC FAILED: %p %p %d", p, ptr, new_size);
  }
  //show_heap();    
  return p; 
}

void free_check(uint8_t *ptr) {
  DEBUG_MEMORY("Free: %p", ptr);
  free(ptr);
  //show_heap();     
}

// ------------------------------------------------------------------------------------------------------------
// Packet handling routines
// ------------------------------------------------------------------------------------------------------------

void new_packet(struct packet_data *pd, int length) {
  pd->ptr = (uint8_t *) malloc_check(length) ;
  pd->size = length;
}

void new_packet_from_data(struct packet_data *pd, uint8_t *data, int length) {
  pd->ptr = (uint8_t *) malloc_check(length) ;
  pd->size = length;
  for (int i = 0; i < length; i++)
    pd->ptr[i] = data[i];
}

void clear_packet(struct packet_data *pd) {
  free_check(pd->ptr);
  pd->size = 0; 
}

// ------------------------------------------------------------------------------------------------------------
// Routines to handle validating packets of data from SparkComms before further processing
// Uses the RTOS queues to receive the packets
// ------------------------------------------------------------------------------------------------------------

//#define DEBUG_COMMS(...)  {char _b[100]; sprintf(_b, __VA_ARGS__); Serial.println(_b);}
#define DEBUG_COMMS(...) {}
//#define DEBUG_STATUS(...)  {char _b[100]; sprintf(_b, __VA_ARGS__); Serial.println(_b);}
#define DEBUG_STATUS(...) {}
//#define DUMP_BUFFER(p, s) {for (int _i=0; _i <=  (s); _i++) {Serial.print( (p)[_i], HEX); Serial.print(" ");}; Serial.println();}
#define DUMP_BUFFER(p, s) {}

bool scan_packet (CircularArray &buf, int *start, int *this_end, int end) {
  int cmd; 
  int sub;
  int checksum;
  int multi_total_chunks, multi_this_chunk, multi_last_chunk;
  int st = -1;
  int en = -1;
  int this_checksum = 0;
  bool is_good = true;
  bool is_done = false;
  bool is_multi = false;
  bool is_final_multi = false;
  bool is_first_multi = false;
  bool found_chunk = false;

  int len = buf.length();
  int p = *start;

  while (!is_done) {
    // check to see if past end of buffer
    if (p > end ) {
      is_done = true;
      is_good = false;
      en = p-1;   // is this ok????
    }
 
    // skip a block header if we find one
    else if (end - p >= 2 && buf[p] == 0x01 && buf[p + 1] == 0xfe) {
      p += 16;
    }
    
    // found start of a message - either single or multi-chunk
    else if (end - p >= 6 && buf [p] == 0xf0 && buf[p + 1] == 0x01) {

      //DEBUG_COMMS("Pos %3d: new header", p);
      found_chunk = true;
      checksum = buf[p + 3];
      cmd      = buf[p + 4];
      sub      = buf[p + 5];
      this_checksum = 0;

      if ((cmd == 0x01 || cmd == 0x03) && sub == 0x01)
        is_multi = true;
      else
       is_multi = false;
    
      if (is_multi && end - p >= 9) {
        multi_total_chunks = buf[p + 7] | (buf[p + 6] & 0x01? 0x80 : 0x00);         
        multi_this_chunk   = buf[p + 8] | (buf[p + 6] & 0x02? 0x80 : 0x00);
        is_first_multi = (multi_this_chunk == 0);
        is_final_multi = (multi_this_chunk + 1 == multi_total_chunks);

        //DEBUG_COMMS("Pos %3d: multi-chunk %d of %d", p, multi_this_chunk, multi_total_chunks);
        if (!is_first_multi && (multi_this_chunk != multi_last_chunk + 1)) {
          //DEBUG_COMMS( "Gap in multi chunk numbers");
          is_good = false;
        }
      }
      // only mark start if first multi chunk or not multi at all
      if (!is_multi || (is_multi && is_first_multi)) {
        //DEBUG_COMMS("Mark as start of chunks");
        st = p;
        is_good = true;
      }

      // skip header
      p += 6;
    }

    // if we have an f7, check we found a header and if multi, we are at last chunk
    else if (buf[p] == 0xf7 && found_chunk) {
      //DEBUG_COMMS( "Pos %3d: got f7", p);
      //DEBUG_COMMS("Provided checksum: %2x Calculated checksum: %2x", checksum, this_checksum);
      if (checksum != this_checksum)
        is_good = false;
      if (is_multi)
        multi_last_chunk = multi_this_chunk;
      if (!is_multi| (is_multi && is_final_multi)) {
        en = p;
        is_done = true;
      }
      else
        p++;
    }
    // haven't found a block yet so just scanning
    else if (!found_chunk) {
      p++;
    }

    // must be processing a meaningful block so update checksum calc
    else {
      this_checksum ^= buf[p];
      p++;
    }
  }
  *start = st;
  *this_end = en;
  DEBUG_COMMS("Returning start: %3d end: %3d status: %s", st, en, is_good ? "good" : "bad");
  return is_good;
}

void handle_spark_packet() {
  struct packet_data qe; 
  struct packet_data me;
  
  int start, end;
  bool good_packet;
  int good_end;

  int len, trim_len;

  // process packets queued
  while (uxQueueMessagesWaiting(qFromSpark) > 0) {
    lastSparkPacketTime = millis();
    xQueueReceive(qFromSpark, &qe, (TickType_t) 0);

    // passthru
    if (ble_passthru) {
      send_to_app(qe.ptr, qe.size);
    }

    array_spark.append(qe.ptr, qe.size);
    clear_packet(&qe); // this was created in app_callback, no longer needed
  }
  
  end = array_spark.length() - 1;
  start = 0;
  good_end = 0;

  if (array_spark.length() > 0) {  
    if (scan_packet(array_spark, &start, &end, array_spark.length())) {
      DEBUG_COMMS("Got a good packet %d %d", start, end);
      len = end - start + 1;
      int orig_len = len;

      if (start > 0) {
        array_spark.shrink(start);    // clear out any bad data
        end =- start;
      }

      // process the packet to extract the msgpack data
      trim_len = remove_headers(array_spark, array_spark, len);
      fix_bit_eight(array_spark, trim_len);
      len = compact(array_spark, array_spark, trim_len);

      array_spark.extract_append(spark_msg_in.message_in, len, orig_len);
    }
  }

  // check for timeouts and delete the packet, it took too long to get a proper packet
  if ((array_spark.length() > 0) && (millis() - lastSparkPacketTime > SPARK_TIMEOUT)) {
    array_spark.clear();
    Serial.println("CLEARED SPARK");
  }
}

void handle_app_packet() {
  struct packet_data qe; 
  struct packet_data me;
  int start, end;
  bool good_packet;
  int good_end;

  int len, trim_len;

  // process packets queued
  while (uxQueueMessagesWaiting(qFromApp) > 0) {
    lastAppPacketTime = millis();
    xQueueReceive(qFromApp, &qe, (TickType_t) 0);

    if (ble_passthru) {
      send_to_spark(qe.ptr, qe.size);
    }

    array_app.append(qe.ptr, qe.size);
    clear_packet(&qe); // this was created in app_callback, no longer needed
  }

  end = array_app.length() - 1;
  start = 0;
  good_end = 0;

  if (array_app.length() > 0) {  
    if (scan_packet(array_app, &start, &end, array_app.length())) {
      DEBUG_COMMS("Got a good packet %d %d", start, end);
      len = end - start + 1;
      int orig_len = len;

      if (start > 0) {
        array_app.shrink(start);    // clear out any bad data
        end =- start;
      }

      // process the packet to extract the msgpack data
      trim_len = remove_headers(array_app, array_app, len);
      fix_bit_eight(array_app, trim_len);
      len = compact(array_app, array_app, trim_len);

      array_app.extract_append(app_msg_in.message_in, len, orig_len);
    }
  }

  // check for timeouts and delete the packet, it took too long to get a proper packet
  if ((array_app.length() > 0) && (millis() - lastAppPacketTime > APP_TIMEOUT)) {
    array_app.clear();
    Serial.println("CLEARED APP");
  }

}


// simply copy the packet received and put pointer in the queue
void app_callback(uint8_t *buf, int size) {
  struct packet_data qe;

  new_packet_from_data(&qe, buf, size);
  xQueueSend (qFromApp, &qe, (TickType_t) 0);
}

void spark_callback(uint8_t *buf, int size) {
  struct packet_data qe;

  new_packet_from_data(&qe, buf, size);
  xQueueSend (qFromSpark, &qe, (TickType_t) 0);
}

// ------------------------------------------------------------------------------------------------------------
// Global variables
//
// last_sequence_to_spark used for a response to a 0x0201 request for a preset - must have the same sequence in the response
// ------------------------------------------------------------------------------------------------------------
int last_sequence_to_spark;


// ------------------------------------------------------------------------------------------------------------
// Routines to process blocks of data to get to msgpack format
//
// remove_headers() - remove the 01fe and f001 headers, add 6 byte header to packet
// fix_bit_eight() - add the missing eighth bit to each data byte
// compact()       - remove the multi-chunk header and the eighth bit byte to get to msgpack data
// ------------------------------------------------------------------------------------------------------------

// remove_headers())
// Removes any headers (0x01fe and 0xf001) from the packets and leaves the rest
// Each new data block starts with a 6 byte SparkIO header
// 0  command
// 1  sub-command
// 2  total block length (inlcuding this header) (msb)
// 3  total block length (including this header) (lsb)
// 4  number of checksum errors in the original block
// 5  sequence number of the original block

int remove_headers(CircularArray &out_block, CircularArray &in_block, int in_len) {
  int new_len  = 0;
  int in_pos   = 0;
  int out_pos  = 0;
  int out_base = 0; 
  int last_sequence = -1;

  byte chk;

  byte sequence;
  byte command;
  byte sub_command;
  byte checksum;

  while (in_pos < in_len) {
    // check for 0xf7 chunk ending
    if (in_block[in_pos] == 0xf7) {
       in_pos++;
       out_block[out_base + 2] = (out_pos - out_base) >> 8;
       out_block[out_base + 3] = (out_pos - out_base) & 0xff;
       out_block[out_base + 4] += (checksum != chk);
       out_block[out_base + 5] = sequence;
    }    
    // check for 0x01fe start of Spark 40 16-byte block header
    else if (in_block[in_pos] == 0x01 && in_block[in_pos + 1] == 0xfe) {
      in_pos += 16;
    }
    // check for 0xf001 chunk header
    else if (in_block[in_pos] == 0xf0 && in_block[in_pos + 1] == 0x01) {
      sequence    = in_block[in_pos + 2];
      checksum    = in_block[in_pos + 3];
      command     = in_block[in_pos + 4];
      sub_command = in_block[in_pos + 5];


      chk = 0;
      in_pos += 6;

      if (sequence != last_sequence) {
        last_sequence = sequence;
        out_base = out_pos;                     // move to end of last data
        out_pos  = out_pos + HEADER_LEN;        // save space for header      
        out_block[out_base]     = command;
        out_block[out_base + 1] = sub_command;
        out_block[out_base + 4] = 0;
      }
    }
    else {
      out_block[out_pos] = in_block[in_pos];
      chk ^= in_block[in_pos];
      in_pos++;
      out_pos++;
    }
  }
  // keep a global record of the sequence number for a response to an 0x0201
  last_sequence_to_spark = sequence;

  return out_pos;
}

void fix_bit_eight(CircularArray &in_block, int in_len) {
  int len = 0;
  int in_pos = 0;
  int counter = 0;
  int command = 0;

  byte bitmask;
  byte bits;
  int cmd_sub = 0;

  while (in_pos < in_len) {
    if (len == 0) {
      command = (in_block[in_pos] << 8) + in_block[in_pos];
      len = (in_block[in_pos + 2] << 8) + in_block[in_pos + 3];
      in_pos += HEADER_LEN;
      len    -= HEADER_LEN;
      counter = 0;
    }
    else {
      if (counter % 8 == 0) {
        bitmask = 1;
        bits = in_block[in_pos];
      }
      else {
        if (bits & bitmask) {
          in_block[in_pos] |= 0x80;
        }
        bitmask <<= 1;
      }
      counter++;
      len--;
      in_pos++;
    }
    if (command == 0x0101 && counter == 150) 
      counter = 0;
    if (command == 0x0301 && counter == 32) 
      counter = 0;    

  }
}

// TODO - this currently can cope with multiple messages in a sequence, but doesn't need to be able to do that any more!!!!

int compact(CircularArray &out_block, CircularArray &in_block, int in_len) {
  int len = 0;
  int in_pos = 0;
  int out_pos = 0;
  int counter = 0;
  int out_base = 0;

  int total_chunks;
  int this_chunk;
  int data_len;

  int command = 0;

  while (in_pos < in_len) {
    if (len == 0) {
      // start of new block so prepare header and new out_base pointer
      out_base = out_pos;
      command = (in_block[in_pos] << 8) +     in_block[in_pos + 1];
      len     = (in_block[in_pos + 2] << 8) + in_block[in_pos + 3];
      // fill in the out header (length will change!)
      memcpy(&out_block[out_base], &in_block[in_pos], HEADER_LEN);      
      in_pos  += HEADER_LEN;
      out_pos += HEADER_LEN;
      len     -= HEADER_LEN;
      counter = 0;

    }
    // if len is not 0
    else {
      // this is the bitmask, so we won't copy it
      if (counter % 8 == 0) {      
        in_pos++;
      }
      // this is the multi-chunk header - perhaps do some checks on this in future
      else if ((command == 0x0301 || command == 0x0101 ) && (counter >= 1 && counter <= 3)) { 
        if (counter == 1) total_chunks = in_block[in_pos++];
        if (counter == 2) this_chunk   = in_block[in_pos++];
        if (counter == 3) data_len     = in_block[in_pos++];         
      }
      // otherwise we can copy it
      else { 
        out_block[out_pos] = in_block[in_pos];
        out_pos++;
        in_pos++;
      }
      counter++;
      len--;
      // if at end of the block, update the header length
      if (len == 0) {
        out_block[out_base + 2] = (out_pos - out_base) >> 8;
        out_block[out_base + 3] = (out_pos - out_base) & 0xff;
      }
      if (command == 0x0101 && counter == 150) 
        counter = 0;
      if (command == 0x0301 && counter == 32) 
        counter = 0;    
    }
  }
  return out_pos;
}

void process_sparkIO() {
  handle_app_packet();
  handle_spark_packet();
}


// ------------------------------------------------------------------------------------------------------------
// MessageIn class
// 
// Message formatting routines to read the msgpack 
// Read messages from the in_message ring buffer and copy to a SparkStructure format
// ------------------------------------------------------------------------------------------------------------

void MessageIn::read_byte(uint8_t *b)
{
  *b = message_in[message_pos++];
}   

void MessageIn::read_uint(uint8_t *b)
{
  uint8_t a;
  read_byte(&a);
  if (a == 0xCC)
    read_byte(&a);
  *b = a;
}
   
void MessageIn::read_string(char *str)
{
  uint8_t a, len;
  int i;

  read_byte(&a);
  if (a == 0xd9) {
    read_byte(&len);
  }
  else if (a >= 0xa0) {
    len = a - 0xa0;
  }
  else {
    read_byte(&a);
    if (a < 0xa0 || a >= 0xc0) DEBUG("Bad read_string");
    len = a - 0xa0;
  }

  if (len > 0) {
    // process whole string but cap it at STR_LEN-1
    for (i = 0; i < len; i++) {
      read_byte(&a);
      if (a<0x20 || a>0x7e) a=0x20; // make sure it is in ASCII range - to cope with get_serial 
      if (i < STR_LEN -1) str[i]=a;
    }
    str[i > STR_LEN-1 ? STR_LEN-1 : i]='\0';
  }
  else {
    str[0]='\0';
  }
}   

void MessageIn::read_prefixed_string(char *str)
{
  uint8_t a, len;
  int i;

  read_byte(&a); 
  read_byte(&a);

  if (a < 0xa0 || a >= 0xc0) DEBUG("Bad read_prefixed_string");
  len = a-0xa0;

  if (len > 0) {
    for (i = 0; i < len; i++) {
      read_byte(&a);
      if (a<0x20 || a>0x7e) a=0x20; // make sure it is in ASCII range - to cope with get_serial 
      if (i < STR_LEN -1) str[i]=a;
    }
    str[i > STR_LEN-1 ? STR_LEN-1 : i]='\0';
  }
  else {
    str[0]='\0';
  }
}   

void MessageIn::read_float(float *f)
{
  union {
    float val;
    byte b[4];
  } conv;   
  uint8_t a;
  int i;

  read_byte(&a);  // should be 0xca
  if (a != 0xca) return;

  // Seems this creates the most significant byte in the last position, so for example
  // 120.0 = 0x42F00000 is stored as 0000F042  
   
  for (i=3; i>=0; i--) {
    read_byte(&a);
    conv.b[i] = a;
  } 
  *f = conv.val;
}

void MessageIn::read_onoff(bool *b)
{
  uint8_t a;
   
  read_byte(&a);
  if (a == 0xc3)
    *b = true;
  else // 0xc2
    *b = false;
}

// The functions to get the message

bool MessageIn::get_message(unsigned int *cmdsub, SparkMessage *msg, SparkPreset *preset)
{
  uint8_t cmd, sub, len_h, len_l;
  uint8_t sequence;
  uint8_t chksum_errors;

  unsigned int len;
  unsigned int cs;
   
  uint8_t junk;
  int i, j;
  uint8_t num, num1, num2;
  uint8_t num_effects;

  if (message_in.length() == 0) return false;

  message_pos = 0;

  read_byte(&cmd);
  read_byte(&sub);
  read_byte(&len_h);
  read_byte(&len_l);
  read_byte(&chksum_errors);
  read_byte(&sequence);

  bytes_to_uint(len_h, len_l, &len);
  bytes_to_uint(cmd, sub, &cs);


  *cmdsub = cs;
  switch (cs) {
    // 0x02 series - requests
    // get preset information
    case 0x0201:
      read_byte(&msg->param1);
      read_byte(&msg->param2);
      for (i=0; i < 30; i++) read_byte(&junk); // 30 bytes of 0x00
      break;            
    // get current hardware preset number - this is a request with no payload
    case 0x0210:
      break;
    // get amp name - no payload
    case 0x0211:
      break;
    // get name - this is a request with no payload
    //case 0x0221:
    //  break;
    // get serial number - this is a request with no payload
    case 0x0223:
      break;

    case 0x022a:
      // Checksum request (40 / GO / MINI)
      // the data is a fixed array of four bytes (0x94 00 01 02 03)
      read_byte(&junk);
      read_uint(&msg->param1);
      read_uint(&msg->param2);
      read_uint(&msg->param3);
      read_uint(&msg->param4);
      break;   

    case 0x032a:
      // Checksum response (40 / GO / MINI)
      // the data is a fixed array of four bytes (0x94 00 01 02 03)
      read_byte(&junk);
      read_uint(&msg->param1);
      read_uint(&msg->param2);
      read_uint(&msg->param3);
      read_uint(&msg->param4);
      break;
    // get firmware version - this is a request with no payload
    case 0x022f:
      break;
    // change effect parameter
    case 0x0104:
      read_string(msg->str1);
      read_byte(&msg->param1);
      read_float(&msg->val);
      read_byte(&msg->param2);
      //in_message.clear();        // temporary fix for added Input byte with LIVE
      break;
    // change of effect model
    case 0x0306:
    case 0x0106:
      read_string(msg->str1);
      read_string(msg->str2);
      read_byte(&msg->param2);     
      break;
    // get current hardware preset number
    case 0x0310:
      read_byte(&msg->param1);
      read_byte(&msg->param2);
      break;
    // get name
    case 0x0311:
      read_string(msg->str1);
      break;
    // enable / disable an effect
    // and 0x0128 amp info command
    case 0x0315:
    case 0x0115:
      read_string(msg->str1);
      read_onoff(&msg->onoff);
      read_byte(&msg->param1);
      //in_message.clear();        // temporary fix for added Input byte with LIVE
      break;
    case 0x0128:
      read_string(msg->str1);
      read_onoff(&msg->onoff);
      break;      
    // get serial number
    case 0x0323:
      read_string(msg->str1);
      break;
    // store into hardware preset
    case 0x0327:
      read_byte(&msg->param1);
      read_byte(&msg->param2);
      break;
    // amp info   
    case 0x0328:
      read_float(&msg->val);
      break;  
    // firmware version number
    case 0x032f:
      // really this is a uint32 but it is just as easy to read into 4 uint8 - a bit of a cheat
      read_byte(&junk);           // this will be 0xce for a uint32
      read_byte(&msg->param1);      
      read_byte(&msg->param2); 
      read_byte(&msg->param3); 
      read_byte(&msg->param4); 
      break;
    // change of effect parameter
    case 0x0337:
      read_string(msg->str1);
      read_byte(&msg->param1);
      read_float(&msg->val);
      read_byte(&msg->param2);   // input - new for LIVE
      break;
    // tuner
    case 0x0364:
      read_byte(&msg->param1);
      read_float(&msg->val);
      break;
    case 0x0365:
      read_onoff(&msg->onoff);
      break;
    // change of preset number selected on the amp via the buttons
    case 0x0338:
    case 0x0138:
      read_byte(&msg->param1);
      read_byte(&msg->param2);
      break;
    // license key
    case 0x0170:
      for (i = 0; i < 64; i++)
        read_uint(&license_key[i]);
      break;
    // response to a request for a full preset
    case 0x0301:
    case 0x0101:
      read_byte(&preset->curr_preset);
      read_byte(&preset->preset_num);
      read_string(preset->UUID); 
      read_string(preset->Name);
      read_string(preset->Version);
      read_string(preset->Description);
      read_string(preset->Icon);
      read_float(&preset->BPM);
      read_byte(&num);
      num_effects = num - 0x90;
      preset->num_effects = num_effects;
      for (j=0; j < num_effects; j++) {
        read_string(preset->effects[j].EffectName);
        read_onoff(&preset->effects[j].OnOff);
        read_byte(&num);
        preset->effects[j].NumParameters = num - 0x90;
        for (i = 0; i < preset->effects[j].NumParameters; i++) {
          read_byte(&junk);
          read_byte(&junk);
          read_float(&preset->effects[j].Parameters[i]);
        }
      }
      read_byte(&preset->chksum);  
      break;
    // tap tempo!
    case 0x0363:
      read_float(&msg->val);  
      break;
    case 0x0470:
    case 0x0428:
      read_byte(&junk);
      break;
    // acks - no payload to read - no ack sent for an 0x0104
    case 0x0401:
    case 0x0501:
    case 0x0406:
    case 0x0415:
    case 0x0438:
    case 0x0465:
    case 0x0474: 
    // Serial.print("Got an ack ");
    // Serial.println(cs, HEX);
      break;

    //
    // LIVE messages  
    //

    // Power Settings - Auto Standby and Auto Shutdown
    //      boolean       ? (0xc2 = false)
    //      byte          auto-shutdown time in minutes (0 = Never, 30, 40, 50, 60)
    //      byte          ?
    //      byte          auto standby time in minute (0 = Never, 5, 10, 15, 30)

    case 0x0172:
      read_onoff(&msg->bool1);
      read_byte(&msg->param1);
      read_byte(&msg->param2);
      read_byte(&msg->param3);

      DEB("LIVE set power setting ");
      if (msg->bool1) DEB("true "); else DEB("false ");
      DEB(msg->param1);
      DEB(" ");
      DEBUG(msg->param3);

      break;

    case 0x0372:
      read_onoff(&msg->bool1);
      read_byte(&msg->param1);
      read_byte(&msg->param2);
      read_byte(&msg->param3);

      DEB("LIVE power setting response ");
      if (msg->bool1) DEB("true "); else DEB("false ");
      DEB(msg->param1);
      DEB(" ");
      DEBUG(msg->param3);

      break;
    case 0x0272:
      DEBUG("LIVE get power setting ");
      break;

    case 0x0472:
      read_byte(&junk);
      break;

    // Impedance
    //      byte          0x91 - fixed array size 1
    //      byte          0 = IN1, 1 = IN2 1/4, 2 = IN2 XLR, 3 = IN3, 4 = IN4
    //      byte          0 = Standard, 1 = Hi-Z, 2 = Line, 3 = Mic

    case 0x0174:
      read_byte(&num);
      read_uint(&msg->param1); // 0 = IN1, 1 = IN2 1/4, 2 = IN2 XLR, 4 = IN3/4
      read_uint(&msg->param2); // 0 = Standard, 1 = Hi-Z, 2 = Line, 3 = Mic  

      DEB("LIVE impedance change ");
      DEB(msg->param1);
      DEB(" ");
      DEBUG(msg->param2);
      break;

    //  Get impedance
    case 0x0274:
      read_byte(&num);
      read_uint(&msg->param1); // 0 = IN1, 1 = IN2 1/4, 2 = IN2 XLR, 4 = IN3/4

      DEB("LIVE get impedance ");
      DEBUG(msg->param1);
      break;
    
    // Impedance response
    case 0x0374:
      read_byte(&num);
      read_uint(&msg->param1); // 0 = IN1, 1 = IN2 1/4, 2 = IN2 XLR, 4 = IN3/4
      read_uint(&msg->param2); // 0 = Standard, 1 = Hi-Z, 2 = Line, 3 = Mic  

      DEB("LIVE impedance response ");
      DEB(msg->param1);
      DEB(" ");
      DEBUG(msg->param2);
      break;


    // Mixer
    //      byte          0 = IN1, 1 = IN2 1/4, 2 = IN2 XLR, 3 = IN3, 4 = IN4, 5= MUSIC, 9 = MASTER
    //      float         value

    case 0x0133:
      read_uint(&msg->param1); 
      read_float(&msg->val);

      DEB("MIXER change channel ");
      DEB(msg->param1);
      DEB(" Value: ");
      DEBUG(msg->val);
      break;

    // LIVE INPUT 1 Guitar Volume
    case 0x036b:
      read_float(&msg->val);

      DEB("LIVE guitar volume ");
      DEBUG(msg->val);
      break;
      
    // LIVE Input 2 Cable Insert  
    case 0x0373:
      read_byte(&num);
      num -= 0x90;  // should be a fixed array
      msg->param5 = num;

      read_byte(&msg->param1);
      read_byte(&msg->param2);
      read_onoff(&msg->bool1);

      if (num ==2)  {
        read_byte(&msg->param3);
        read_byte(&msg->param4);
        read_onoff(&msg->bool2);
      }

      DEB("LIVE Input 2 cable insert ");
      DEB(msg->param1);
      DEB(" ");
      DEB(msg->param2);
      DEB(" ");
      if (msg->bool1) DEB("plugged in "); else DEB ("unplugged");

      if (num == 2) {
        DEB(msg->param3);
        DEB(" ");
        DEB(msg->param4);
        DEB(" ");
        if (msg->bool2) DEB("plugged in "); else DEB ("unplugged"); 
      }
      DEBUG("");
      break;


    case 0x0273:
      read_byte(&num);
      num -= 0x90;  // should be a fixed array
      msg->param5 = num;

      read_byte(&msg->param1);
      read_byte(&msg->param2);

      DEBUG("LIVE get input 2 ");
      DEB(msg->param1);
      DEB(" ");
      DEB(msg->param2);
      DEBUG(" ");      
      break;



    // LIVE request checksums
    //      byte          Input (0 = Input 1, 1 = Input 2)

    case 0x022b:
      read_uint(&msg->param1); 

      DEB("Request LIVE checkums, input ");
      DEBUG(msg->param1);
      break;

    // Checksum response (LIVE)
    case 0x032b:
      // the data is a fixed array of eight bytes (0x98 00 01 02 03)
      read_byte(&junk);
      read_uint(&msg->param1);
      read_uint(&msg->param2);
      read_uint(&msg->param3);
      read_uint(&msg->param4);
      read_uint(&msg->param5);
      read_uint(&msg->param6);
      read_uint(&msg->param7);
      read_uint(&msg->param8);

      DEB("LIVE checksums ");
      DEB(msg->param1, HEX); DEB(" ");
      DEB(msg->param2, HEX); DEB(" ");
      DEB(msg->param3, HEX); DEB(" ");
      DEB(msg->param4, HEX); DEB(" ");   
      DEB(msg->param5, HEX); DEB(" ");
      DEB(msg->param6, HEX); DEB(" ");
      DEB(msg->param7, HEX); DEB(" ");
      DEB(msg->param8, HEX); DEBUG(" ");      
      break;


    // LIVE Get mixer setting
    case 0x0233:
      read_byte(&msg->param1);

      DEB("LIVE Mixer request setting for input ");
      DEBUG(msg->param1);
      break;


    // LIVE Mixer
    case 0x0333:
      read_float(&msg->val);

      DEB("LIVE Mixer setting is ");
      DEBUG(msg->val);
      break;

    // LIVE includes 0x031a with 0x0338 with change to preset via HW Button

    // 0x31a in response to a 0x021a gives:
    // Unprocessed message 21A length 9:92 0 1 
    // LIVE hardware preset change 0x031a   0 2 Unchanged 1 6 Unchanged 

    //
    // But as a hardware generated event (pressed) it gives:
    // LIVE hardware preset change 0x031a   0 2 Unchanged
    // Message: 31A 

    case 0x031a:
      read_byte(&num);
      num -= 0x90;  // should be a fixed array, size 1 or 2
      msg->param5 = num;

      read_byte(&msg->param1);
      read_byte(&msg->param2);
      read_onoff(&msg->bool1);
      if (num == 2) {
        read_byte(&msg->param3);
        read_byte(&msg->param4);
        read_onoff(&msg->bool2);  
      }

      DEB("LIVE hardware preset information response ");      
      DEB(msg->param1);
      DEB(" ");
      DEB(msg->param2);
      if (msg->bool1) DEB(" Unsaved changes "); else DEB(" Unchanged ");
      if (num == 2) {
        DEB(msg->param3);
        DEB(" "); 
        DEB(msg->param4);
        if (msg->bool2) DEB(" Unsaved changes"); else DEB(" Unchanged ");  
      }
      DEBUG(""); 
      break;

    case 0x021a:
      read_byte(&num);
      num -= 0x90;  // should be a fixed array
      msg->param5 = num;

      read_byte(&msg->param1);
      read_byte(&msg->param2);

      DEB("LIVE get hardware preset information ");      
      DEB(msg->param1);
      DEB(" ");
      DEBUG(msg->param2);
      break;


    /*
    case 0x0371:
      DEBUG("undefined Battery?");
      read_byte(&msg->param1);
      read_byte(&msg->param2);
      read_byte(&msg->param3);
      read_byte(&msg->param4);   
      read_byte(&num);  
      read_byte(&num); 
      DEBUG(num, HEX);    // should be 0xCD
      read_byte(&num1);
      read_byte(&num2);
      bytes_to_uint(num1, num2, &msg->param10);
      read_byte(&msg->param6);    
      read_byte(&msg->param7);   
      DEBUG(msg->param10, HEX);   
      in_message.clear();
      break;
    */

    // the UNKNOWN command - 0x0224 00 01 02 03
    //case 0x0224:

    default:
      DEB("Unprocessed message ");
      DEB(cs, HEX);
      DEB(" length ");
      DEB(len);

      DEB(":");
      if (len != 0) {
        for (i = 0; i < len - 6; i++) {
          read_byte(&junk);
          DEB(junk, HEX);
          DEB(" ");
        }
      }
      DEBUG();
  }

  message_in.shrink(len);
  return true;
}


// used when sending a preset to Spark to see if a block has been received, will lose the messages until acknowledgement one
bool MessageIn::check_for_acknowledgement() {
  uint8_t cmd, sub, len_h, len_l;
  uint8_t sequence;
  uint8_t chksum_errors;

  unsigned int len;
  unsigned int cs;
  
  if (message_in.length() == 0) return false;

  message_pos = 0;

  read_byte(&cmd);
  read_byte(&sub);
  read_byte(&len_h);
  read_byte(&len_l);
  read_byte(&chksum_errors);
  read_byte(&sequence);

  bytes_to_uint(len_h, len_l, &len);
  bytes_to_uint(cmd, sub, &cs);

  message_in.shrink(len);

  if (cs == 0x0401 || cs == 0x0501)  
    return true;
  else
    return false;
};


// ------------------------------------------------------------------------------------------------------------
// MessageOut class
// 
// Message formatting routines to create the msgpack 
// ead messages from the SparkStructure format and place into the out_message ring buffer 
// ------------------------------------------------------------------------------------------------------------


void MessageOut::start_message(int cmdsub)
{
  int om_cmd, om_sub;
  
  om_cmd = (cmdsub & 0xff00) >> 8;
  om_sub = cmdsub & 0xff;

  buf_size = sizeof(buffer);
  buf_pos = 0;

  buffer[0] = om_cmd;
  buffer[1] = om_sub;
  buffer[2] = 0;      // placeholder for length
  buffer[3] = 0;      // placeholder for length
  buffer[4] = 0;      // placeholder for checksum errors
  buffer[5] = 0x60;   // placeholder for sequence number - setting to 0 will not work!
  buf_pos = 6;

  out_msg_chksum = 0;
}

void MessageOut::end_message()
{
  unsigned int len;
  uint8_t len_h, len_l;
  
  len = buf_pos;
  uint_to_bytes(len, &len_h, &len_l);
  
  buffer[2] = len_h;   
  buffer[3] = len_l;
}

void MessageOut::write_byte_no_chksum(byte b)
{
  if (buf_pos < buf_size) 
    buffer[buf_pos++] = b;
  else {
    DEBUG("WRITE PAST END OF BUFFER");
  }
}

void MessageOut::write_byte(byte b)
{
  write_byte_no_chksum(b);
  out_msg_chksum += int(b);
}

void MessageOut::write_uint(byte b)
{
  if (b > 127) {
    write_byte_no_chksum(0xCC);
    out_msg_chksum += int(0xCC);  
  }
  write_byte_no_chksum(b);
  out_msg_chksum += int(b);
}

void MessageOut::write_uint32(uint32_t w)
{
  int i;
  uint8_t b;
  uint32_t mask;

  mask = 0xFF000000;
  
  write_byte_no_chksum(0xCE);
  out_msg_chksum += int(0xCE);  

  for (i = 3; i >= 0; i--) {
    b = (w & mask) >> (8*i);
    mask >>= 8;
    write_uint(b);
  }
}


void MessageOut::write_prefixed_string(const char *str)
{
  int len, i;

  len = strnlen(str, STR_LEN);
  write_byte(byte(len));
  write_byte(byte(len + 0xa0));
  for (i=0; i<len; i++)
    write_byte(byte(str[i]));
}

void MessageOut::write_string(const char *str)
{
  int len, i;

  len = strnlen(str, STR_LEN);
  write_byte(byte(len + 0xa0));
  for (i=0; i<len; i++)
    write_byte(byte(str[i]));
}      
  
void MessageOut::write_long_string(const char *str)
{
  int len, i;

  len = strnlen(str, STR_LEN);
  write_byte(byte(0xd9));
  write_byte(byte(len));
  for (i=0; i<len; i++)
    write_byte(byte(str[i]));
}

void MessageOut::write_float (float flt)
{
  union {
    float val;
    byte b[4];
  } conv;
  int i;
   
  conv.val = flt;
  // Seems this creates the most significant byte in the last position, so for example
  // 120.0 = 0x42F00000 is stored as 0000F042  
   
  write_byte(0xca);
  for (i=3; i>=0; i--) {
    write_byte(byte(conv.b[i]));
  }
}

void MessageOut::write_onoff (bool onoff)
{
  byte b;

  if (onoff)
  // true is 'on'
    b = 0xc3;
  else
    b = 0xc2;
  write_byte(b);
}

void MessageOut::change_effect_parameter (char *pedal, int param, float val)
{
   if (cmd_base == 0x0100) 
     start_message (cmd_base + 0x04);
   else
     start_message (cmd_base + 0x37);
   write_prefixed_string (pedal);
   write_byte (byte(param));
   write_float(val);
   // Added with LIVE 
   write_byte(0);  // 0 is Input 1
   end_message();
}

void MessageOut::change_effect_parameter_input (char *pedal, int param, float val, uint8_t input)
{
   if (cmd_base == 0x0100) 
     start_message (cmd_base + 0x04);
   else
     start_message (cmd_base + 0x37);
   write_prefixed_string (pedal);
   write_byte (byte(param));
   write_float(val);
   // Added with LIVE 
   write_byte(input);  // 0 is Input 1
   end_message();
}

void MessageOut::change_effect (char *pedal1, char *pedal2)
{
   start_message (cmd_base + 0x06);
   write_prefixed_string (pedal1);
   write_prefixed_string (pedal2);
   // Added with LIVE 
   write_byte(0);  // 0 is Input 1
   end_message();
}

void MessageOut::change_effect_input(char *pedal1, char *pedal2, uint8_t input)
{
   start_message (cmd_base + 0x06);
   write_prefixed_string (pedal1);
   write_prefixed_string (pedal2);
   // Added with LIVE 
   write_byte(input);  // 0 is Input 1
   end_message();
}

void MessageOut::change_hardware_preset (uint8_t curr_preset, uint8_t preset_num)
{
   // preset_num is 0 to 3

   start_message (cmd_base + 0x38);
   write_byte (curr_preset);
   write_byte (preset_num) ;      
   end_message();  
}


void MessageOut::turn_effect_onoff (char *pedal, bool onoff)
{
   start_message (cmd_base + 0x15);
   write_prefixed_string (pedal);
   write_onoff (onoff);
   // Added with LIVE 
   write_byte(0);  // 0 is Input 1
   end_message();
}

void MessageOut::turn_effect_onoff_input (char *pedal, bool onoff, uint8_t input)
{
   start_message (cmd_base + 0x15);
   write_prefixed_string (pedal);
   write_onoff (onoff);
   // Added with LIVE 
   write_byte(input);  // 0 is Input 1
   end_message();
}



void MessageOut::get_serial()
{
   start_message (0x0223);
   end_message();  
}

void MessageOut::get_name()
{
   start_message (0x0211);
   end_message();  
}

void MessageOut::get_hardware_preset_number()
{
   start_message (0x0210);
   end_message();  
}

void MessageOut::get_checksum_info() {
   start_message (0x022a);
   write_byte(0x94);
   write_uint(0);
   write_uint(1);
   write_uint(2);
   write_uint(3);   
   end_message();   
}

void MessageOut::get_firmware() {
   start_message (0x022f);
   end_message(); 
}

void MessageOut::save_hardware_preset(uint8_t curr_preset, uint8_t preset_num)
{
   start_message (cmd_base + 0x27);
//   start_message (0x0327);
   write_byte (curr_preset);
   write_byte (preset_num);  
   end_message();
}

void MessageOut::send_firmware_version(uint32_t firmware)
{
   start_message (0x032f);
   write_uint32(firmware);  
   end_message();
}

void MessageOut::send_serial_number(char *serial)
{
   start_message (0x0323);
   write_prefixed_string(serial);
   end_message();
}

void MessageOut::send_ack(unsigned int cmdsub) {
   start_message (cmdsub);
   end_message();
}

/*
void MessageOut::send_0x022a_info(byte v1, byte v2, byte v3, byte v4)
{
   start_message (0x022a);
   write_byte(0x94);
   write_uint(v1);
   write_uint(v2);
   write_uint(v3);
   write_uint(v4);      
   end_message();
}
*/

void MessageOut::send_key_ack()
{
   start_message (0x0470);
   write_byte(0x00);
   end_message();
}

void MessageOut::send_preset_number(uint8_t preset_h, uint8_t preset_l)
{
   start_message (0x0310);
   write_byte(preset_h);
   write_byte(preset_l);
   end_message();
}

void MessageOut::send_tap_tempo(float val)
{
   if (cmd_base == 0x0100) 
     start_message (cmd_base + 0x62);
   else
     start_message (cmd_base + 0x63);   // is this right??
   write_float(val);
   write_byte(0x3f);
   write_byte(0x3f);
   end_message();
}


void MessageOut::tuner_on_off(bool onoff)
{
   start_message (0x0165);
   write_onoff (onoff);
   end_message();
}

void MessageOut::get_preset_details(unsigned int preset)
{
   int i;
   uint8_t h, l;

   uint_to_bytes(preset, &h, &l);
   
   start_message (0x0201);
   write_byte(h);
   write_byte(l);

   for (i=0; i<30; i++) {
     write_byte(0);
   }
   
   end_message(); 
}

void MessageOut::create_preset(SparkPreset *preset)
{
  int i, j, siz;

  start_message (cmd_base + 0x01);

  write_byte_no_chksum (0x00);
  write_byte_no_chksum (preset->preset_num);   
  write_long_string (preset->UUID);
  //write_string (preset->Name);
  if (strnlen (preset->Name, STR_LEN) > 31)
    write_long_string (preset->Name);
  else
    write_string (preset->Name);
    
  write_string (preset->Version);
  if (strnlen (preset->Description, STR_LEN) > 31)
    write_long_string (preset->Description);
  else
    write_string (preset->Description);
  write_string(preset->Icon);
  write_float (preset->BPM);
   
  write_byte (byte(0x90 + preset->num_effects));       // always 7 pedals
  for (i = 0; i < preset->num_effects; i++) {
    write_string (preset->effects[i].EffectName);
    write_onoff(preset->effects[i].OnOff);

    siz = preset->effects[i].NumParameters;
    write_byte ( 0x90 + siz); 
      
    for (j = 0; j < siz; j++) {
      write_byte (j);
      write_byte (byte(0x91));
      write_float (preset->effects[i].Parameters[j]);
    }
  }
  write_byte_no_chksum (uint8_t(out_msg_chksum % 256));  
  end_message();
}


// ------------------------------------------------------------------------------------------------------------
// Routines to process the msgpack format into Spark blocks
// ------------------------------------------------------------------------------------------------------------

int expand(byte *out_block, byte *in_block, int in_len) {
  int len = 0;
  int in_pos = 0;
  int out_pos = 0;
  int counter = 0;

  int total_chunks;
  int this_chunk;
  int this_len;
  int chunk_size;
  int last_chunk_size;

  bool multi;

  int command = 0;
  uint8_t cmd = 0;
  uint8_t sub = 0;
  uint8_t sequence = 0;

  int i;

  cmd = in_block[in_pos];
  sub = in_block[in_pos + 1];
  command = (cmd << 8) + sub;
  len = (in_block[in_pos + 2] << 8) + in_block[in_pos + 3];
  len -= HEADER_LEN;
  sequence = in_block[in_pos + 5];
  
  in_pos += HEADER_LEN;
  chunk_size = len;
  multi = false;
  total_chunks = 1;
  last_chunk_size = len;

  if (command == 0x0101) {
    chunk_size = 128;
    multi = true;
  }
  if (command == 0x0301) {
    chunk_size = 25;
    multi = true;
  }
  // using the global as this should be a response to a 0x201
  // TODO - think of a better way to do this
  if (command == 0x0301) 
    sequence = last_sequence_to_spark;

  if (multi)
    total_chunks = int((len - 1) / chunk_size) + 1;

  if (chunk_size != 0) 
    last_chunk_size = len % chunk_size;
  else
    last_chunk_size = 0;

  if (last_chunk_size == 0) last_chunk_size = chunk_size;   // an exact number of bytes into the chunks

  for (this_chunk = 0; this_chunk < total_chunks; this_chunk++) {
    this_len = (this_chunk == total_chunks - 1) ? last_chunk_size : chunk_size;     // how big is the last chunk
    
    out_block[out_pos++] = 0xf0;
    out_block[out_pos++] = 0x01;
    out_block[out_pos++] = sequence;
    out_block[out_pos++] = 0;            // space for checksum
    out_block[out_pos++] = cmd;
    out_block[out_pos++] = sub;

    counter = 0;
    // do each data byte
    for (i = 0; i < this_len; i++) {
      if (counter % 8 == 0) {
        out_block[out_pos++] = 0;        // space for bitmap
        counter++;
      }
      if (multi && counter == 1) {       // counter == 1 because we have a first bitmap space
        out_block[out_pos++] = total_chunks;
        out_block[out_pos++] = this_chunk;
        out_block[out_pos++] = this_len;
        counter += 3;
      }
      out_block[out_pos++] = in_block[in_pos++];
      counter++;
    }
    out_block[out_pos++] = 0xf7;
  }
  return out_pos;
}

void add_bit_eight(byte *in_block, int in_len) {
  int in_pos = 0;
  int bit_pos;
  int counter = 0;

  int total_chunks;
  int this_chunk;
  int this_len;
  int chunk_size;
  int last_chunk_size;

  int checksum_pos;
  int checksum;

  bool multi;

  int command = 0;
  uint8_t cmd = 0;
  uint8_t sub = 0;

  byte bitmask;

  int i;

  cmd = in_block[in_pos + 4];
  sub = in_block[in_pos + 5];
  command = (cmd << 8) + sub;

  in_pos = 0;
  chunk_size = in_len;
  multi = false;
  total_chunks = 1;

  if (command == 0x0101) {
    chunk_size = 157;
    multi = true;
  }
  if (command == 0x0301) {
    chunk_size = 39;
    multi = true;
  }
  if (multi)
    total_chunks = int((in_len - 1) / chunk_size) + 1;

  last_chunk_size = in_len % chunk_size;
  if (last_chunk_size == 0) last_chunk_size = chunk_size;   // an exact number of bytes into the chunks


  for (this_chunk = 0; this_chunk < total_chunks; this_chunk++) {
    this_len = (this_chunk == total_chunks - 1) ? last_chunk_size : chunk_size;     
    counter = 0;
    checksum = 0;
    checksum_pos = in_pos + 3;
    in_pos += CHUNK_HEADER_LEN;
    // do each data byte
    for (i = CHUNK_HEADER_LEN; i < this_len - 1; i++) {   // skip header and trailing f7
      if (counter % 8 == 0) {
        in_block[in_pos] = 0;        // space for bitmap
        bitmask = 1;
        bit_pos = in_pos;
      }
      else {
        checksum ^= in_block[in_pos] & 0x7f;
        if (in_block[in_pos] & 0x80) {
          in_block[in_pos] &= 0x7f;
          in_block[bit_pos] |= bitmask;
          checksum ^= bitmask;
        };
        bitmask <<= 1;
      };
      counter++;
      in_pos++;
    }
    in_block[checksum_pos] = checksum;
    in_pos++;    // skip the trailing f7
  }
}  
 
uint8_t header_to_app[]    {0x01, 0xfe, 0x00, 0x00, 0x41, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
uint8_t header_to_spark[]  {0x01, 0xfe, 0x00, 0x00, 0x53, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

int add_headers(byte *out_block, byte *in_block, int in_len) {
  int in_pos;
  int out_pos;

  int total_chunks;
  int this_chunk;
  int this_len;
  int chunk_size;
  int last_chunk_size;

  int command;
  uint8_t cmd;
  uint8_t sub;

  int i;

  in_pos = 0;
  out_pos = 0;

  cmd = in_block[in_pos + 4];
  sub = in_block[in_pos + 5];
  command = (cmd << 8) + sub;

  chunk_size = in_len;            // default if not a multi-chunk message
  total_chunks = 1;

  if (command == 0x0101) chunk_size = 157;
  if (command == 0x0301) chunk_size = 90;

  total_chunks = int((in_len - 1) / chunk_size) + 1;

  last_chunk_size = in_len % chunk_size;
  if (last_chunk_size == 0) last_chunk_size = chunk_size;   // an exact number of bytes into the chunks

  for (this_chunk = 0; this_chunk < total_chunks; this_chunk++) {
    this_len = (this_chunk == total_chunks - 1) ? last_chunk_size : chunk_size;   
    if (cmd == 0x01 || cmd == 0x02) {
      // sending to the amp
      memcpy(&out_block[out_pos], header_to_spark, 16);
    }
    else {
      memcpy(&out_block[out_pos], header_to_app, 16);
    };
    out_block[out_pos + 6] = this_len + 16;
    out_pos += 16;
    memcpy(&out_block[out_pos], &in_block[in_pos], this_len);
    out_pos += this_len;
    in_pos += this_len;
  }
  return out_pos;
}

// ------------------------------------------------------------------------------------------------------------
// Routines to send to the app and the amp
// ------------------------------------------------------------------------------------------------------------

// only need one temp block out as we won't send to app and amp at same time (not thread safe!)

byte block_out_temp[OUT_BLOCK_SIZE];

void spark_send() {
  int len;
  byte direction;

  int this_block;
  int num_blocks;
  int block_size;

  int last_block_len;
  int this_len;

  uint8_t *block_out;

  block_out = spark_msg_out.buffer;
  len = spark_msg_out.buf_pos;

  //if (spark_msg_out.has_message()) {
  if (spark_msg_out.buf_pos > 0) {
    len = expand(block_out_temp, block_out, len);
    add_bit_eight(block_out_temp, len);
    len = add_headers(block_out, block_out_temp, len);

    // with the 16 byte header, position 4 is 0x53fe for data being sent to Spark, and 0x41ff for data going to the app
    // although should be onvious from the call used eg spark_send() sends to spark
    direction = block_out[4];
    if (direction == 0x53)      block_size = 173;
    else if (direction == 0x41) block_size = 106;

    num_blocks = int ((len - 1) / block_size) + 1;
    last_block_len = len % block_size;
    for (this_block = 0; this_block < num_blocks; this_block++) {
      this_len = (this_block == num_blocks - 1) ? last_block_len : block_size;
      send_to_spark(&block_out[this_block * block_size], this_len);
      //Serial.println("Sent a block");

      if (num_blocks != 1) {   // only do this for the multi blocks
        bool done = false;
        unsigned long t;
        t = millis();
        while (!done && (millis() - t) < 400) {  // add timeout just in case of no acknowledgement
          //spark_process();
          process_sparkIO();
          done = spark_msg_in.check_for_acknowledgement();
        };
      } 
    }
  }
}

void app_send() {
  int len;
  byte direction;

  int this_block;
  int num_blocks;
  int block_size;

  int last_block_len;
  int this_len;

  uint8_t *block_out;

  block_out = app_msg_out.buffer;
  len = app_msg_out.buf_pos;

  if (app_msg_out.buf_pos > 0) {
    len = expand(block_out_temp, block_out, len);
    add_bit_eight(block_out_temp, len);
    len = add_headers(block_out, block_out_temp, len);

    // with the 16 byte header, position 4 is 0x53fe for data being sent to Spark, and 0x41ff for data going to the app
    // although should be onvious from the call used eg app_send() sends to app
    direction = block_out[4];
    if (direction == 0x53)      block_size = 173;
    else if (direction == 0x41) block_size = 106;

    num_blocks = int ((len - 1) / block_size) + 1;
    last_block_len = len % block_size;
    for (this_block = 0; this_block < num_blocks; this_block++) {
      this_len = (this_block == num_blocks - 1) ? last_block_len : block_size;
      send_to_app(&block_out[this_block * block_size], this_len);
    }
  }
}

