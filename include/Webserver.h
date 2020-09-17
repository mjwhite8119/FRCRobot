#ifndef _HTML_H_
#define _HTML_H_

#include <WiFi.h>
#include "Robot.h"

WiFiServer server(80);

int LED_BUILTIN = 2;

// Variable to store the HTTP request
String header;

// Auxiliar variables to store the current output state
String forwardState = "stop";
String backwardState = "stop";
String leftState = "stop";
String rightState = "stop";

// Current time
unsigned long currentTime = millis();
// Previous time
unsigned long previousTime = 0; 
// Define timeout time in milliseconds (example: 2000ms = 2s)
const long timeoutTime = 2000;

// ----------------------------------------------------------
// Display the HTML page header and CSS
// ----------------------------------------------------------
void displayHeader(WiFiClient client) {

  // Display the HTML web page
  client.println("<!DOCTYPE html><html>");
  client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
  client.println("<link rel=\"icon\" href=\"data:,\">");
  // CSS to style the on/off buttons 
  // Feel free to change the background-color and font-size attributes to fit your preferences
  client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
  client.println(".button { background-color: #4CAF50; border: none; color: white; padding: 16px 40px;");
  client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
  client.println(".button2 {background-color: #555555;}</style></head>");
  
  // Web Page Heading
  client.println("<body><h1>ESP32 Robot</h1>");
}

// ----------------------------------------------------------
// Listen for incoming client requests to the Web Server 
// ----------------------------------------------------------
void handleWebServer(Robot robot) 
{
  WiFiClient client = server.available();   

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
            
            // Control the robot action
            if (header.indexOf("GET /forward/start") >= 0) {
              forwardState = "start";
              digitalWrite(LED_BUILTIN, HIGH);
              robot.forward();         
            } else if (header.indexOf("GET /backward/start") >= 0) {
              backwardState = "start";
              digitalWrite(LED_BUILTIN, HIGH);
              robot.backward();
            } else if (header.indexOf("GET /left/start") >= 0) {
              leftState = "start";
              digitalWrite(LED_BUILTIN, HIGH);
              robot.left();
            } else if (header.indexOf("GET /right/start") >= 0) {
              rightState = "start";
              digitalWrite(LED_BUILTIN, HIGH);
              robot.right();  
            } else if (header.indexOf("GET /forward/stop") >= 0) {
              forwardState = "stop";
              digitalWrite(LED_BUILTIN, LOW);
              robot.stop();
            } else if (header.indexOf("GET /backward/stop") >= 0) {
              backwardState = "stop";
              digitalWrite(LED_BUILTIN, LOW);
              robot.stop();
            } else if (header.indexOf("GET /left/stop") >= 0) {
              leftState = "stop";
              digitalWrite(LED_BUILTIN, LOW);
              robot.stop();
            } else if (header.indexOf("GET /right/stop") >= 0) {
              rightState = "stop";
              digitalWrite(LED_BUILTIN, LOW);
              robot.stop();      
            }          
            
            // Display the HTML page header and CSS
            displayHeader(client);
            
            // If the forwardState is stop, it displays the START button       
            if (forwardState=="stop") {
              client.println("<p><a href=\"/forward/start\"><button class=\"button\">Forward&nbsp;</button></a></p>");
            } else {
              client.println("<p><a href=\"/forward/stop\"><button class=\"button button2\">Stop</button></a></p>");
            } 
            
            // If the backwardState is stop, it displays the START button       
            if (backwardState=="stop") {
              client.println("<p><a href=\"/backward/start\"><button class=\"button\">Backward</button></a></p>");
            } else {
              client.println("<p><a href=\"/backward/stop\"><button class=\"button button2\">Stop</button></a></p>");
            }


            // If the forwardState is stop, it displays the START button       
            if (leftState=="stop") {
              client.println("<p><a href=\"/left/start\"><button class=\"button\">Left</button></a></p>");
            } else {
              client.println("<p><a href=\"/left/stop\"><button class=\"button button2\">Stop</button></a></p>");
            } 
            
            // If the backwardState is stop, it displays the START button       
            if (rightState=="stop") {
              client.println("<p><a href=\"/right/start\"><button class=\"button\">Right</button></a></p>");
            } else {
              client.println("<p><a href=\"/right/stop\"><button class=\"button button2\">Stop</button></a></p>");
            }

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

/*
#include <string>

std::string openHtmlPage()
{  
  return "<!DOCTYPE HTML><html>\r\n";
}

std::string closeHtmlPage()
{
  return "</center></body></html>\n";   
}

std::string indexHtmlPage()
{  
  std::string htmlPage = 
      openHtmlPage() +
            "<body>" +
            "<center>" + 
            "<h1>Motor Controller</h1><br>" +
            "Click to turn <a href=\"ledOn\">LED ON</a><br>" +
            "Click to turn <a href=\"ledOff\">LED OFF</a><br>" +
            "<hr>" +
            "Click to <a href=\"forward\">Move Forward</a><br>" +
            "Click to <a href=\"backward\">Move Backward</a><br>" +
            "Click to <a href=\"stop\">Stop</a>" +
            "<hr>";
  return htmlPage;
}

void printStatus(const std::string msg) {
  std::string s = indexHtmlPage() + msg + closeHtmlPage();
  // server.send(200, "text/html", s);
}
*/

#endif // _HTML_H_