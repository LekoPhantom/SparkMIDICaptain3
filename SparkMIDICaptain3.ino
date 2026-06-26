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
