#ifndef _OLED_H_
#define _OLED_H_

// ---- Use the SSD1306 library for OLED ----------- //
#if USE_OLED 
  #include <U8g2lib.h>

  #define SCREEN_WIDTH 128 // OLED display width, in pixels
  #define SCREEN_HEIGHT 32 // OLED display height, in pixels
  #define OLED_SDA   21
  #define OLED_SCL   22

  // NodeMCU-32S: SCL 21, SDA 22, No reset
  U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C u8g2(U8G2_R0, OLED_SCL, OLED_SDA, U8X8_PIN_NONE);

  // Adds some images for the
  #include "images.h"
#endif // USE_OLED

/* ------------------- Drawing functions -----------------------------*/
// Clear a rectangled area of the screen
inline void clearRect(const uint16_t x, const uint16_t y, const uint16_t width, const uint16_t height)
{
#if USE_OLED    
  u8g2.setDrawColor(0);
  u8g2.drawBox(x, y, width, height);
  u8g2.sendBuffer();  
  u8g2.setDrawColor(1);
  delay(1);
#endif  
}

inline void clearLinePart(const uint16_t lineNumber, const uint16_t startPosition, const uint16_t width)
{
#if USE_OLED  
  const uint16_t lineWidth = 10;
  const uint16_t yPos = (lineNumber * lineWidth);
  clearRect(startPosition, yPos, width, lineWidth);
#endif  
}

inline void clearLine(const uint16_t lineNumber)
{
#if USE_OLED  
  clearLinePart(lineNumber, 0, SCREEN_WIDTH);
#endif  
}

inline void drawText(const uint16_t lineNumber, const uint16_t startPosition, String text) {
#if USE_OLED  
  const uint16_t lineWidth = 10;
  uint16_t yPos=lineNumber * lineWidth;

  // U8G2 uses c-string so convert.
  const char * char_text = text.c_str();

  // write something to the internal memory
  yPos = (lineNumber * lineWidth) + 9;
  u8g2.drawStr(startPosition, yPos, char_text); 

  // transfer internal memory to the display
  u8g2.sendBuffer();             
 
#endif   
}

inline void drawBitmap(const uint16_t x, const uint16_t y, const uint16_t width, const uint16_t height, const unsigned char* bm_bits) 
{
#if USE_OLED  
  u8g2.drawXBMP(x, y, width, height, bm_bits);
  u8g2.sendBuffer();  
#endif  
}

// Clear the entire screen
inline void clearDisplay() {
#if USE_OLED  
  u8g2.clear();
#endif  
}

inline void clearDisplayBelowHeader() {
#if USE_OLED  
  clearRect(0, 10, SCREEN_WIDTH, SCREEN_HEIGHT-10);
#endif  
}

inline void drawBattery(const uint16_t x=108, const uint16_t y=0, const uint8_t capacity=3) {
#if USE_OLED 
  clearLinePart(0, x, y);
  if (capacity == 1) {
    drawBitmap(x, y, BAT_width, BAT_height, BATLow_bits); 
  } else if (capacity == 2) {
    drawBitmap(x, y, BAT_width, BAT_height, BATMedium_bits);
  } else {
    drawBitmap(x, y, BAT_width, BAT_height, BAT_bits);
  }  
#endif  
}

inline void drawAP(const uint16_t x=90, const uint16_t y=0) {
#if USE_OLED  
  drawBitmap(x, y, AP_width, AP_height, AP_bits);
#endif  
}

inline void drawSerial(const uint16_t x=45, const uint16_t y=0) {
#if USE_OLED  
  drawBitmap(x, y, SERIAL_width, SERIAL_height, SERIAL_bits);
#endif  
}

inline void drawMQTT(const uint16_t x=48, const uint16_t y=0) {
#if USE_OLED  
  drawBitmap(x, y, MQTT_width, MQTT_height, MQTT_bits);
#endif  
}

inline void drawLORA(const uint16_t x=35, const uint16_t y=0) {
#if USE_OLED  
  drawBitmap(x, y, LORA_width, LORA_height, LORA_bits);
#endif  
}

inline void drawWiFi(const uint16_t x=0, const uint16_t y=0) {
#if USE_OLED  
  clearLinePart(0, 0, 20);
  drawBitmap(x, y, WIFI_width, WIFI_height, WIFI_bits);
#endif  
}

// Remove the WiFi icon and display the disconnect message
inline void clearWiFi(const uint16_t x=0, const uint16_t y=0) {
#if USE_OLED
  clearLinePart(0, 0, 15);
  clearLine(2);
  drawText(2, 0, "WiFi disconnected!");
#endif  
}

inline void drawIPAddress(const uint16_t lineNumber, const uint16_t startPosition, const IPAddress ip) {
#if USE_OLED  
  drawText(lineNumber, startPosition, String(ip[0]) + "." + String(ip[1]) + "." + String(ip[2]) + "." + String(ip[3]));
#endif  
}

//******************************************************//
// setup oled
// *****************************************************//
inline void setupOLED()
{
  // start the display
  u8g2.begin();

  // clear the internal memory
  u8g2.clearBuffer();   

  // setup font
  u8g2.setFontMode(0);
  u8g2.setFont(u8g2_font_t0_11_mr);         
}

#endif // _OLED_H_