#ifndef _CONNECT_WIFI_H_
#define _CONNECT_WIFI_H_

#include <WiFi.h> 
#include <ArduinoOTA.h>
#include "wifiConfig.h"

inline bool connectToWiFiRouter = true;
// Uncomment the next 2 lines and enter your WiFi SSID & password
// inline const char* ssid = "SSID";
// inline const char* password = "PASSWORD";

/**
 * Connect to WiFi. 
 * 
 * @param wifi_mode STA(0), AP(1), AP_STA(2)
 */ 
inline void connectWiFi(uint8_t wifi_mode=0)
{
  drawText(1, 0, "Connecting...");
  drawText(2, 0, String(ssid));

  WiFi.disconnect();
  delay(1000); 
    
  if (wifi_mode == 0) {
    WiFi.mode(WIFI_STA); // Mode 0 (default)
    Serial.println("WIFI_STA mode");
  } else if(wifi_mode == 1) {
    WiFi.mode(WIFI_AP); // Mode 1
    Serial.println("WIFI_AP mode");
  } else {
    WiFi.mode(WIFI_AP_STA); // Mode 2
    Serial.println("WIFI_AP_STA mode");
  }
    
  WiFi.setAutoConnect(true);      
  WiFi.setAutoReconnect(true); 
     
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  if (WiFi.status() == WL_CONNECTED) {    
    delay(1000);
    Serial.println("WiFi connected");  
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
    
    clearDisplayBelowHeader();
    drawText(1, 0, "IP");
    drawIPAddress(1, 20, WiFi.localIP());

    // Add the WiFi icon
    drawWiFi();
  } 
  else {
    drawText(1, 0, "WiFi connect failed!");
    Serial.println("WiFi connect failed!");
  }  
}

/**
 * Setup for Over-the-Air updates.
 * This allows you to update the code without being
 * connected via the USB cable. 
 * 
 */ 
inline void setupOTA()
{
  
  ArduinoOTA.onStart([](){
    clearDisplay();
    drawText(0, 0, "Started updating...");
  });

  ArduinoOTA.onEnd([]()
  {
    clearDisplay();
    drawText(0, 0, "Update Complete!");

    ESP.restart();
  });

  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {});

  ArduinoOTA.onError([](ota_error_t error)
  {
    // Serial.printf("Error[%u]: ", error);
    String info = "Error Info:";
    switch(error)
    {
      case OTA_AUTH_ERROR:
        info += "Auth Failed";
        Serial.println("Auth Failed");
        break;

      case OTA_BEGIN_ERROR:
        info += "Begin Failed";
        Serial.println("Begin Failed");        
        break;

      case OTA_CONNECT_ERROR:
        info += "Connect Failed";
        Serial.println("Connect Failed");        
        break;

      case OTA_RECEIVE_ERROR:
        info += "Receive Failed";
        Serial.println("Receive Failed");        
        break;

      case OTA_END_ERROR:
        info += "End Failed";
        Serial.println("End Failed");        
        break;
    }
    ESP.restart();
  });

  ArduinoOTA.begin();
  // drawText(3, 0, "Started OTA");
}


#endif // _CONNECT_WIFI_H_