// Import required libraries
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <SPIFFS.h>

String sliderValue = "190";
String direction = "";
int period = 0;
int PWM = 190;

const char* PARAM_INPUT = "value";

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

/**
 * Replaces placeholder with slider value in your web page
 * 
 * @param var - Requested string value from web page
 */ 
String processor(const String& var){
  if (var == "SLIDERVALUE"){
    return sliderValue;
  }
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
    request->send(SPIFFS, "/index2.html", String(), false, processor);
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
    request->send(SPIFFS, "/index2.html", String(), false, processor);
  });

  // Send a GET request to <ESP_IP>/slider?value=<inputMessage>
  server.on("/slider", HTTP_GET, [] (AsyncWebServerRequest *request) {
    String inputMessage;
    // GET input1 value on <ESP_IP>/slider?value=<inputMessage>
    if (request->hasParam(PARAM_INPUT)) {
      inputMessage = request->getParam(PARAM_INPUT)->value();
      sliderValue = inputMessage;
      PWM = sliderValue.toInt();
    }
    else {
      inputMessage = "No message sent";
    }
    Serial.println(inputMessage);
    request->send(200, "text/plain", "OK");
  });

  // Begin the WebServer
  server.begin();

}