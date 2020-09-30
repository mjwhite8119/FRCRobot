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

      server.on("/", HTTP_GET, [this](AsyncWebServerRequest *request) { handleRoot(request); });
      
      // Route to load style.css file
      server.on("/style.css", HTTP_GET, [this](AsyncWebServerRequest *request) { handleCSS(request); });

      // Route to move the motors
      server.on("/move", HTTP_GET, [this](AsyncWebServerRequest *request) { handleMove(request); });

      // Begin the WebServer
      server.begin();
    }

    /**
     * Replaces placeholder with slider value in your web page
     * 
     * @param var - Requested string value from web page
     */ 
    String processor(const String& var){
      return String();
    }

    /**
     * Display the root index page
     * 
     * @param request - The async webserver request
     */ 
    void handleRoot(AsyncWebServerRequest *request)
    {
      request->send(SPIFFS, "/index1.htm", String(), false, [this](const String& var) { return processor(var); });
    }

    /**
     * Display the page CSS
     * 
     * @param request - The async webserver request
     */ 
    void handleCSS(AsyncWebServerRequest *request)
    {
      request->send(SPIFFS, "/style.css", "text/css");
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
      request->send(SPIFFS, "/index1.html", String(), false, [this](const String& var) { return processor(var); });
    }   

  private:
    Command & command_;

    String direction_ = "";
    int period_ = 0;
};

#endif // _CONTROLLER_H_