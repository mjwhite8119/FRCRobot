// Import required libraries
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <SPIFFS.h>

String direction = "";
int period = 0;

const char* PARAM_INPUT = "value";

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

/**
 * Replaces placeholder with value in your web page
 * 
 * @param var - Requested string value from web page
 */ 
String processor(const String& var){
  return String();
}

/**
 * Listen for incoming client requests to the Web Server
 */ 
void handleWebServer() {

  // Start the SPIFFS filesystem to serve the HTML pages
  if(!SPIFFS.begin(true)){
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });
  
  // Route to load style.css file
  server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/style.css", "text/css");
  });

  // Route to forward motor
  server.on("/move", HTTP_GET, [](AsyncWebServerRequest *request){
    String periodStr = request->arg("value");  
    direction = request->arg("direction");
    period = periodStr.toInt();
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });

  // Begin the WebServer
  server.begin();

}