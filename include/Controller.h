#ifndef _CONTROLLER_H_
#define _CONTROLLER_H_

#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "Command.h"

const char* PARAM_INPUT_1 = "period";
const char* PARAM_INPUT = "value";

String sliderValueLeft = "0";
String sliderValueRight = "0";

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <title>FRC Training Bot</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <link rel="icon" href="data:,">
  <style>
    html {font-family: Arial; display: inline-block; text-align: center;}
    input[type=button], input[type=submit], input[type=reset] {
      background-color: #4CAF50;
      border: none;
      color: white;
      padding: 16px 32px;
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
      width: 100px;
    }
    body {
      max-width: 400px; 
      margin:0px auto; 
      padding-bottom: 25px;
    }
    .container { padding: 7px; 
      position: relative; 
      border: none; 
      margin: 7px;
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
  <h2>FRC Training Bot</h2>
  <h3>Lesson 5</h3>
  <div class='container'>
    <h4 class='label'>Move (seconds)</h4>
    %BUTTONPLACEHOLDER1%
  </div> 
  <p></p>
  <span>Adjust Left Speed </span>
    <span id='textSliderValueLeft'>%SLIDERVALUE1%</span>
    <div class='slidercontainer'>  
      <input type='range' onchange='updateSliderLeft(this)' id='pwmSliderLeft' min='-1' max='1' value='%SLIDERVALUE1%' step='0.1' class='slider'>
    </div>
    <span>Adjust Right Speed </span>
    <span id='textSliderValueRight'>%SLIDERVALUE2%</span>
    <div class='slidercontainer'>  
      <input type='range' onchange='updateSliderRight(this)' id='pwmSliderRight' min='-1' max='1' value='%SLIDERVALUE2%' step='0.1' class='slider'>
    </div> 
<script>
function move(element) {
  var xhr = new XMLHttpRequest();
  xhr.open("GET", "/update?period="+element.value, true); 
  xhr.send();
}
function updateSliderLeft(element) {
  var sliderValueLeft = document.getElementById('pwmSliderLeft').value;
  document.getElementById('textSliderValueLeft').innerHTML = sliderValueLeft;
  console.log(sliderValueLeft);
  var xhr = new XMLHttpRequest();
  xhr.open('GET', '/slider1?value='+sliderValueLeft, true);
  xhr.send();
}
function updateSliderRight(element) {
  var sliderValueRight = document.getElementById('pwmSliderRight').value;
  document.getElementById('textSliderValueRight').innerHTML = sliderValueRight;
  console.log(sliderValueRight);
  var xhr = new XMLHttpRequest();
  xhr.open('GET', '/slider2?value='+sliderValueRight, true);
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
    buttons += "<input type=\"button\" value=\"1\" onclick=\"move(this)\">";
    buttons += "<input type=\"button\" value=\"2\" onclick=\"move(this)\">";
    buttons += "<input type=\"button\" value=\"3\" onclick=\"move(this)\">";
    return buttons;
  }
  if (var == "SLIDERVALUE1"){
    return sliderValueLeft;
  } else if (var == "SLIDERVALUE2"){
    return sliderValueRight;
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

  // GET input1 value on <ESP_IP>/update?period=<periodStr>&direction=<direction>
  if (request->hasParam(PARAM_INPUT_1)) {
    periodStr = request->getParam(PARAM_INPUT_1)->value();    
  }

  // Log value to the console for debugging
  log_d("period %s", periodStr);

  // Assign the parameters and schedule the command
  command.period = periodStr.toInt();
  command.schedule();

  request->send(200, "text/plain", "OK");
}

/**
 * Handle the left wheel slider request
 * 
 * @param request - The async webserver request
 * @param command - Command object
 */ 
void handleSlider1(AsyncWebServerRequest *request, Command & command)
{
  // GET input1 value on <ESP_IP>/slider?value=<inputMessage>
  if (request->hasParam(PARAM_INPUT)) {
    sliderValueLeft = request->getParam(PARAM_INPUT)->value();
    command.leftSpeed = sliderValueLeft.toFloat();
  }
  Serial.println(sliderValueLeft);
  request->send(200, "text/plain", "OK");
}

/**
 * Handle the right wheel slider request
 * 
 * @param request - The async webserver request
 * @param command - Command object
 */ 
void handleSlider2(AsyncWebServerRequest *request, Command & command)
{
  // GET input1 value on <ESP_IP>/slider?value=<inputMessage>
  if (request->hasParam(PARAM_INPUT)) {
    sliderValueRight = request->getParam(PARAM_INPUT)->value();
    command.rightSpeed = sliderValueRight.toFloat();
  }
  Serial.println(sliderValueRight);
  request->send(200, "text/plain", "OK");
}  

void setupController(Command & command){
  
  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html, processor);
  });

  // Send a GET request to <ESP_IP>/update?period=<periodStr>&direction=<direction>
  server.on("/update", HTTP_GET, [&command] (AsyncWebServerRequest *request) { handleMove(request, command); });

  // Send a GET request to /slider1?value=<inputMessage>
  server.on("/slider1", HTTP_GET, [&command] (AsyncWebServerRequest *request) { handleSlider1(request, command); });

  // Send a GET request to /slider2?value=<inputMessage>
  server.on("/slider2", HTTP_GET, [&command] (AsyncWebServerRequest *request) { handleSlider2(request, command); });

  // Display the page not found message
  server.onNotFound([](AsyncWebServerRequest *request){
    request->send(404, "text/plain", "The robot controller was not found.");
  });

  // Start server
  server.begin();
}

#endif // _CONTROLLER_H_