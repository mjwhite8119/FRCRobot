// Import required libraries
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <SPIFFS.h>

String sliderValueLeft = "0";
String sliderValueRight = "0";

const char* PARAM_INPUT = "value";

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

/**
 * Replaces placeholder with slider value in your web page
 * 
 * @param var - Requested string value from web page
 */ 
String processor(const String& var){
  if (var == "SLIDERVALUE1"){
    return sliderValueLeft;
  } else if (var == "SLIDERVALUE2"){
    return sliderValueRight;
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
  } else {
    Serial.println("SPIFFS Loaded successfully");
  }

  File file2 = SPIFFS.open("/index3.html");
  if(!file2){
      Serial.println("Failed to open file for reading");
      return;
  } else {
    Serial.println("Index file ...");
    while(file2.available()){
      Serial.write(file2.read());
    }
    file2.close();
  }

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    // request->send(200, "text/plain", "Robot controller found.");
    request->send(SPIFFS, "/index3.html", String(), false, processor);
  });
  
  // Route to load style.css file
  server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/style.css", "text/css");
  });

  server.onNotFound([](AsyncWebServerRequest *request){
    request->send(404, "text/plain", "The robot controller was not found.");
  });

  // Route to forward motor
  server.on("/move", HTTP_GET, [](AsyncWebServerRequest *request){
    String periodStr = request->arg("value");  
    command.direction = request->arg("direction");
    command.period = periodStr.toInt();
    command.schedule();
    request->send(SPIFFS, "/index3.html", String(), false, processor);
  });

  // Send a GET request to <ESP_IP>/slider?value=<inputMessage>
  server.on("/slider1", HTTP_GET, [] (AsyncWebServerRequest *request) {
    String inputMessage;
    // GET input1 value on <ESP_IP>/slider?value=<inputMessage>
    if (request->hasParam(PARAM_INPUT)) {
      inputMessage = request->getParam(PARAM_INPUT)->value();
      sliderValueLeft = inputMessage;
      command.leftPWM = sliderValueLeft.toInt();
    }
    else {
      inputMessage = "No message sent";
    }
    Serial.println(inputMessage);
    request->send(200, "text/plain", "OK");
  });

  // Send a GET request to <ESP_IP>/slider?value=<inputMessage>
  server.on("/slider2", HTTP_GET, [] (AsyncWebServerRequest *request) {
    String inputMessage;
    // GET input1 value on <ESP_IP>/slider?value=<inputMessage>
    if (request->hasParam(PARAM_INPUT)) {
      inputMessage = request->getParam(PARAM_INPUT)->value();
      sliderValueRight = inputMessage;
      command.rightPWM = sliderValueRight.toInt();
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