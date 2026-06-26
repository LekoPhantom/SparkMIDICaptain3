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

