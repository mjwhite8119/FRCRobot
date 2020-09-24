#ifndef _HTML_H_
#define _HTML_H_

#include <WiFi.h>
#include "Robot.h"

WiFiServer server(80);
WiFiClient client;

// Variable to store the HTTP request
std::string header;

// Initial PWM value
int PWM = 180; 
std::string pwmStr = "180";

// Current time
unsigned long currentTime = millis();
// Previous time
unsigned long previousTime = 0; 
// Define timeout time in milliseconds (example: 2000ms = 2s)
const long timeoutTime = 2000;

// ----------------------------------------------------------
// Display the CSS
// ----------------------------------------------------------
void writeCSS() {
  client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
  client.println(".button { background-color: #4CAF50; width: 50px; border: none; color: white; padding: 10px;");
  client.println("text-decoration: none; font-size: 30px; margin: auto; text-align: center; cursor: pointer;}");
  client.println(".button2 {background-color: #555555;}");
  client.println(".label {display: inline-block; text-align: left; width: 100px;}");
  client.println(".container { padding: 8px; position: relative; border: 1px; margin: 8px; }");
  client.println(".slidercontainer { padding: 1px; position: relative; border: none; margin: 1px; }");
  client.println(".slider1 {-webkit-appearance: none; width: 80%; height: 25px; background: #d3d3d3; outline: none; opacity: 0.7; -webkit-transition: .2s; transition: opacity .2s;}");
  client.println("</style></head>");
}

// ----------------------------------------------------------
// Display the HTML page header and CSS
// ----------------------------------------------------------
void displayHeader() {

  // Display the HTML web page
  client.println("<!DOCTYPE html><html>");
  client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
  client.println("<link rel=\"icon\" href=\"data:,\">");

  // CSS style
  writeCSS();
  
  // Web Page Title Heading
  String value = pwmStr.c_str();
  client.println("<body onload=\"showValue1(" + value + ")\"><h1>Training Robot</h1>");
}

// ----------------------------------------------------------
// Display a button
// ----------------------------------------------------------
void printButton(const String name, const String url) {
  String s = "<span><a href=\"" + url + "\"><button class=\"button\">" + name + "</button></a></span>";
  client.println(s);
}

// ----------------------------------------------------------
// Display a button group
// ----------------------------------------------------------
void displayButtonGroup(const String name) {
  client.println("<form method=get>");
  client.println("<div class=\"container\">");
  client.println("<span><h3 class=\"label\">" + name + "</h3></span>");
  printButton("1", "/" + name + "/1");
  printButton("2", "/" + name + "/2");
  printButton("3", "/" + name + "/3");
  client.println("</div>");
  client.println("</form>");
}

// ----------------------------------------------------------
// Display a button group
// ----------------------------------------------------------
void displaySlider() {

  String value = pwmStr.c_str();
  client.println("<div class=\"slidercontainer\">");
  client.println("<h4>Adjust PWM <span id='range1'></span></h4>");
  client.println("<input type='range' class='slider1' name=slider1 min='180' max='220' value='" + value + "' onchange='showValue1(this.value)'>");
  client.println("<script type='text/javascript'>");
  client.println("function showValue1(newValue)\r\n{\r\ndocument.getElementById('range1').innerHTML=newValue;\r\n}\r\n</script>\r\n");
  client.println("<script type=!text/javascript!></script\r\n>");
  client.println("</div>");
}

// ----------------------------------------------------------
// Listen for incoming client requests to the Web Server 
// ----------------------------------------------------------
void handleWebServer(Robot robot) 
{
  client = server.available();   

  if (client) {                             // If a new client connects,
    currentTime = millis();
    previousTime = currentTime;
    Serial.println("New Client.");          // print a message out in the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected() && currentTime - previousTime <= timeoutTime) {  // loop while the client's connected
      currentTime = millis();
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        header += c;
        if (c == '\n') {                    // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();

            // Extract the time period value from the url
            int pos = header.find("/", 5);          
            std::string x = header.substr(pos+1, 2);           
            int period = stoi(x); // How long to run the motors

            // Extract the PWM value from the url
            pos = header.find("slider1");          
            pwmStr = header.substr(pos+8, 3);  
            Serial.print("pwm = ");  Serial.println(pwmStr.c_str());  

            try {
              PWM = stoi(pwmStr); // PWM signal to the motors
            }               
            catch (const std::invalid_argument & e) {
              PWM = 180;
            }
            
            // Control the motors
            if (header.find("GET /Forward") == 0) {
              Serial.print("F "); Serial.println(PWM);
              robot.forward(period, PWM);                     
            } else if (header.find("GET /Backward") == 0) {
              Serial.print("B "); Serial.println(PWM);
              robot.backward(period, PWM);
            } else if (header.find("GET /Left") == 0) {
              robot.left(period, PWM);
            } else if (header.find("GET /Right") == 0) {
              robot.right(period, PWM);  
            } 
            
            // Display the HTML page header and CSS
            displayHeader();
            
            // client.println("<form method=get>");

            // Display the buttons
            displayButtonGroup("Forward");
            displayButtonGroup("Backward");
            displayButtonGroup("Left");
            displayButtonGroup("Right");

            // Display the slider to get PWM value
            displaySlider();

            // client.println("</form>");

            // Close html
            client.println("</body></html>");
            
            // The HTTP response ends with another blank line
            client.println();
            
            // Break out of the while loop
            break;
          } else { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }
    }
    // Clear the header variable
    header = "";
    // Close the connection
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println(""); 
  }
}

#endif // _HTML_H_