#ifndef Screen_h
  #define Screen_h

  #ifdef OLED_ON
  #include <SPI.h>
  #include <Wire.h>
  #include <Adafruit_GFX.h>
  #include <Adafruit_SSD1306.h>
  Adafruit_SSD1306 display(128, 64, &Wire, -1);


  #define OLED_SDA 4
  #define OLED_SCL 15

  #endif
  
  void splash_screen();
  void show_message(char *msg, int preset);  
  void show_connected();

#endif
