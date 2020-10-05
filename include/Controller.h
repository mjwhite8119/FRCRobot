#ifndef _CONTROLLER_H_
#define _CONTROLLER_H_

#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <SPIFFS.h>
#include "Command.h"

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

class Controller
{
  public:

    /**
     * Constructor
     * 
     * @param command - The command object
     */ 
    Controller(Command & command) 
      :command_(command) {}

    const char* PARAM_INPUT = "value";

    /**
     * Setup the web page used as the controller
     */ 
    void init() {

      // Start the SPIFFS filesystem to serve the HTML pages
      if(!SPIFFS.begin(true)){
        Serial.println("An Error has occurred while mounting SPIFFS");
        return;
      }

      // Route to display the root index page
      server.on("/", HTTP_GET, [this](AsyncWebServerRequest *request){
        request->send(SPIFFS, "/index2.html", String(), false, [this](const String& var) { return processor(var); });
      });
      
      // Route to load style.css file
      server.on("/style.css", HTTP_GET, [this](AsyncWebServerRequest *request) { 
        request->send(SPIFFS, "/style.css", "text/css"); 
      });

      // Route to move the motors
      server.on("/move", HTTP_GET, [this](AsyncWebServerRequest *request) { handleMove(request); });

      // Send a GET request to /slider1?value=<inputMessage>
      server.on("/slider1", HTTP_GET, [this] (AsyncWebServerRequest *request) { handleSlider1(request); });

      // Send a GET request to /slider2?value=<inputMessage>
      server.on("/slider2", HTTP_GET, [this] (AsyncWebServerRequest *request) { handleSlider2(request); });

      // Display the page not found message
      server.onNotFound([this](AsyncWebServerRequest *request){
        request->send(404, "text/plain", "The robot controller was not found.");
      });

      // Begin the WebServer
      server.begin();
    }

    /**
     * Replaces placeholder with slider value in your web page
     * 
     * @param var - Requested string value from web page
     */ 
    String processor(const String& var){
      if (var == "SLIDERVALUE1"){
        return sliderValueLeft_;
      } else if (var == "SLIDERVALUE2"){
        return sliderValueRight_;
      } 
      return String();
    }

    /**
     * Display the page CSS
     * 
     * @param request - The async webserver request
     */ 
    void handleMove(AsyncWebServerRequest *request)
    {
      String periodStr = request->arg("value");
      command_.direction = request->arg("direction");
      command_.period = periodStr.toInt();
      command_.schedule();  
      request->send(SPIFFS, "/index2.html", String(), false, [this](const String& var) { return processor(var); });
    }   

    /**
     * Display the slider
     * 
     * @param request - The async webserver request
     */ 
    void handleSlider1(AsyncWebServerRequest *request)
    {
      String inputMessage;
      // GET input1 value on <ESP_IP>/slider?value=<inputMessage>
      if (request->hasParam(PARAM_INPUT)) {
        inputMessage = request->getParam(PARAM_INPUT)->value();
        sliderValueLeft_ = inputMessage;
        command_.leftPWM = sliderValueLeft_.toInt();
      }
      else {
        inputMessage = "No message sent";
      }
      Serial.println(inputMessage);
      request->send(200, "text/plain", "OK");
    }

    /**
     * Display the slider
     * 
     * @param request - The async webserver request
     */ 
    void handleSlider2(AsyncWebServerRequest *request)
    {
      String inputMessage;
      // GET input1 value on <ESP_IP>/slider?value=<inputMessage>
      if (request->hasParam(PARAM_INPUT)) {
        inputMessage = request->getParam(PARAM_INPUT)->value();
        sliderValueRight_ = inputMessage;
        command_.rightPWM = sliderValueRight_.toInt();
      }
      else {
        inputMessage = "No message sent";
      }
      Serial.println(inputMessage);
      request->send(200, "text/plain", "OK");
    }  

  private:
    Command & command_;

    String sliderValueLeft_ = "0";
    String sliderValueRight_ = "0";
    String direction_ = "";
    int period_ = 0;
    int leftPWM_ = 0;
    int rightPWM_ = 0;  

};

#endif // _CONTROLLER_H_