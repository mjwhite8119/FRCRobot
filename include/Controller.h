#ifndef _CONTROLLER_H_
#define _CONTROLLER_H_

#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "Command.h"

const char* PARAM_INPUT_1 = "period";
const char* PARAM_INPUT_2 = "direction";
const char* PARAM_INPUT = "value";

String sliderValueLeft = "0";
String sliderValueRight = "0";

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <title>FRC BabyBot</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <link rel="icon" href="data:,">
  <style>
    html {font-family: Arial; display: inline-block; text-align: center;}
    input[type=button], input[type=submit], input[type=reset] {
      background-color: #4CAF50;
      border: none;
      color: white;
      padding: 16px 30px;
      text-decoration: none;
      -webkit-appearance: none;
      margin: 4px 2px;
      cursor: pointer;
    }
    h2 {font-size: 2.3rem;}
    h3 {font-size: 1.3rem;}
    p {font-size: 1.9rem;}
    .label {
      display: inline-block; 
      text-align: left; 
      width: 90px;
    }
    body {
      max-width: 430px; 
      margin:0px auto; 
      padding-bottom: 25px;
    }
    .container { padding: 5px; 
      position: relative; 
      border: none; 
      margin: 5px;
    }
    .slidercontainer { 
      padding: 1px; 
      position: relative; 
      border: none; 
      margin: 1px; 
    }
    .slider { 
      -webkit-appearance: none; 
      margin: 14px; 
      width: 300px; 
      height: 20px; 
      background: #4CAF50;
      outline: none; 
      -webkit-transition: .2s; 
      transition: opacity .2s;
    }
    .slider::-webkit-slider-thumb {
      -webkit-appearance: none; 
      appearance: none; 
      width: 30px; 
      height: 30px; 
      background: #000000; 
      cursor: pointer;
    }
    .slider::-moz-range-thumb { 
      width: 30px; 
      height: 30px; 
      background: #000000; 
      cursor: pointer; 
    } 
  </style>
</head>
<body>
  <h2>FRC BabyBot</h2>
  <h3>Lesson 1</h3>
  <div class='container'>
    <h4 class='label'>Forward</h4>
    %BUTTONPLACEHOLDER1%
  </div> 
  <div class='container'>
    <h4 class='label'>Backward</h4>
    %BUTTONPLACEHOLDER2%
  </div> 
  <div class='container'>
    <h4 class='label'>Left</h4>
    %BUTTONPLACEHOLDER3%
  </div> 
  <div class='container'>
    <h4 class='label'>Right</h4>
    %BUTTONPLACEHOLDER4%
  </div> 
<script>
function move(element, dir) {
  var xhr = new XMLHttpRequest();
  xhr.open("GET", "/update?period="+element.value+"&direction="+dir, true); 
  xhr.send();
}
</script>
</body>
</html>
)rawliteral";

/**
 * Replaces placeholder with buttons and sliders in the web page
 * 
 * @param var - Requested string value from web page
 */ 
String processor(const String& var){

  if(var == "BUTTONPLACEHOLDER1"){
    String buttons = "";
    buttons += "<input type=\"button\" value=\"1\" onclick=\"move(this, 'F')\">";
    buttons += "<input type=\"button\" value=\"2\" onclick=\"move(this, 'F')\">";
    buttons += "<input type=\"button\" value=\"3\" onclick=\"move(this, 'F')\">";
    return buttons;
  }
  if(var == "BUTTONPLACEHOLDER2"){
    String buttons = "";
    buttons += "<input type=\"button\" value=\"1\" onclick=\"move(this, 'B')\">";
    buttons += "<input type=\"button\" value=\"2\" onclick=\"move(this, 'B')\">";
    buttons += "<input type=\"button\" value=\"3\" onclick=\"move(this, 'B')\">";
    return buttons;
  }
  if(var == "BUTTONPLACEHOLDER3"){
    String buttons = "";
    buttons += "<input type=\"button\" value=\"1\" onclick=\"move(this, 'L')\">";
    buttons += "<input type=\"button\" value=\"2\" onclick=\"move(this, 'L')\">";
    buttons += "<input type=\"button\" value=\"3\" onclick=\"move(this, 'L')\">";
    return buttons;
  }
  if(var == "BUTTONPLACEHOLDER4"){
    String buttons = "";
    buttons += "<input type=\"button\" value=\"1\" onclick=\"move(this, 'R')\">";
    buttons += "<input type=\"button\" value=\"2\" onclick=\"move(this, 'R')\">";
    buttons += "<input type=\"button\" value=\"3\" onclick=\"move(this, 'R')\">";
    return buttons;
  }
  
  return String();
}

/**
 * Handle the move request
 * 
 * @param request - The async webserver request
 * @param command - Command object
 */ 
void handleMove(AsyncWebServerRequest *request, Command & command)
{
  String periodStr = "0";
  String direction = "0";

  // GET input1 value on <ESP_IP>/update?period=<periodStr>&direction=<direction>
  if (request->hasParam(PARAM_INPUT_1) && request->hasParam(PARAM_INPUT_2)) {
    periodStr = request->getParam(PARAM_INPUT_1)->value();
    direction = request->getParam(PARAM_INPUT_2)->value();     
  }

  // Log value to the console for debugging
  log_d("period %s", periodStr);
  log_d("direction %s", direction);

  // Assign the parameters and schedule the command
  command.period = periodStr.toInt();
  command.direction = direction;
  command.schedule();

  request->send(200, "text/plain", "OK");
}

void setupController(Command & command){
  
  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html, processor);
  });

  // Send a GET request to <ESP_IP>/update?period=<periodStr>&direction=<direction>
  server.on("/update", HTTP_GET, [&command] (AsyncWebServerRequest *request) { handleMove(request, command); });

  // Display the page not found message
  server.onNotFound([](AsyncWebServerRequest *request){
    request->send(404, "text/plain", "The robot controller was not found.");
  });

  // Start server
  server.begin();
}

#endif // _CONTROLLER_H_