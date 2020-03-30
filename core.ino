#include "arduino_secrets.h"
//Based on web server API by Tom Igoe on created 25 Nov 2012

//Required Libraries and Files
#include <ArduinoHttpClient.h>
#include <SPI.h>
#include <WiFiNINA.h>
#include "wrapper.h"
#include "discord.h"
//Contains Passwords etc. 

//Fetch Secret Data

int status = WL_IDLE_STATUS;     // the WiFi radio's status
unsigned long lastMillis = 0; //number of milliseconds into execution last command was processed
IPAddress ip;

WiFiServer server(80);

void setup() {
  pinMode(13, OUTPUT);
  pinMode(12, INPUT);
  pinMode(11, INPUT);
  pinMode(10, OUTPUT);
  // Initialize serial and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  // check for the WiFi module:
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed!");
    wifiStat[0] = 3;
    // don't continue
    while (true);
  }
  String fv = WiFi.firmwareVersion();
  if (fv < WIFI_FIRMWARE_LATEST_VERSION) {
    Serial.println("Please upgrade the firmware");
    wifiStat[0] = 3;
  }
}

void updateNetwork()
{
  
}

//Connect to WiFi
void connect() {
  digitalWrite(13, LOW);
  // attempt to connect to WiFi network:
  Serial.print("Attempting to connect to WPA SSID: ");
  Serial.println(ssid);
  // Connect to WPA/WPA2 network:
  status = WiFi.begin(ssid, pass);
  wifiStat[0] = 1;

  // wait 5 seconds for connection:
  delay(5000);
  if (status != WL_CONNECTED) { wifiStat[0] = 3; } else { printWiFiData(); wifiStat[0] = 2; }

  // you're connected now, so register with discord bot:
  if (wifiStat[0] == 2) {
    digitalWrite(13, HIGH);
    Serial.println("/disc register " + String(ip[0]) + "." + String(ip[1]) + "." + String(ip[2]) + "." + String(ip[3]));
    discord_send("/disc register " + String(ip[0]) + "." + String(ip[1]) + "." + String(ip[2]) + "." + String(ip[3]));
    wifiStat[0] = 4;
    server.begin();                           // start the web server on port 80
    printServerStatus();                        // you're connected now, so print out the status
  }
  Serial.println("Connection Attempt Complete");
}

//Process WiFi Connectivity Status
void processStatus() {
  if (wifiStat[0] == 0) { connect(); }
  if (wifiStat[0] == 3) {
    while (wifiStat[0] == 3) {
      delay(500);
      if (wifiStat[1] == 0) { wifiStat[1] = 1; digitalWrite(13, HIGH); } else { wifiStat[1] = 0; digitalWrite(13, LOW); }
      if (digitalRead(11) == HIGH) { wifiStat[0] = 0; while (digitalRead(11) == HIGH) { delay(10); }} //Reconnect to WiFi
    }
  }
  if (digitalRead(11) == HIGH) { wifiStat[0] = 0; while (digitalRead(11) == HIGH) { delay(10); }} //Reconnect to WiFi
}

//Print Server Connectivity Status
void printServerStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your board's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
  // print where to go in a browser:
  Serial.print("To see this page in action, open a browser to http://");
  Serial.println(ip);
}

void printWiFiData() {
  // print your board's IP address:
  ip = WiFi.localIP();
  Serial.print("IP address : ");
  Serial.println(ip);

  Serial.print("Subnet mask: ");
  Serial.println((IPAddress)WiFi.subnetMask());

  Serial.print("Gateway IP : ");
  Serial.println((IPAddress)WiFi.gatewayIP());

  // print your MAC address:
  byte mac[6];
  WiFi.macAddress(mac);
  //Serial.print("MAC address: ");
  //printMacAddress(mac);
}


void loop() {
  processStatus();
  WiFiClient client = server.available();   // listen for incoming clients
    if (client) {                             // if you get a client,
      Serial.println("new client");           // print a message out the serial port
      String currentLine = "";                // make a String to hold incoming data from the client
      while (client.connected()) {            // loop while the client's connected
        if (client.available()) {             // if there's bytes to read from the client,
          char c = client.read();             // read a byte, then
          Serial.write(c);                    // print it out the serial monitor
          if (c == '\n') {                    // if the byte is a newline character
  
            // if the current line is blank, you got two newline characters in a row.
            // that's the end of the client HTTP request, so send a response:
            if (currentLine.length() == 0) {
              sendHTTP(client);
              // break out of the while loop:
              break;
            } else {    // if you got a newline, parse then clear currentLine:
              parseHTTP(currentLine);
              currentLine = "";
            }
          } else if (c != '\r') {  // if you got anything else but a carriage return character,
            currentLine += c;      // add it to the end of the currentLine
          }
        }
        
      }
      // close the connection:
      client.stop();
      Serial.println("client disonnected");
    }
}


////


void parseHTTP(String currentLine) {
  String args[] = {"", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", ""};
  // Check to see if the client request was a command
  if (currentLine.endsWith(";;") and millis() > lastMillis + 25) {
    int m = 0;
    int n = 0;
    while (m < currentLine.length()) {
      if (String(currentLine[m]) == " " || String(currentLine[m]) == ";") { n = n + 1; } else { args[n] = args[n] + currentLine[m]; }
      m = m + 1;
    }
    Serial.println("CMD >> " + currentLine);
    processCMD(args);
    lastMillis = millis();
  }
  else if (currentLine.startsWith ("GET") and currentLine.endsWith("/run") and millis() > lastMillis + 25) {
    int m = 0;
    int n = 0;
    while (m < currentLine.length()) {
      if (String(currentLine[m]) == "/" || String(currentLine[m]) == ";") { n = n + 1; } else { args[n] = args[n] + currentLine[m]; }
      m = m + 1;
      lastMillis = millis();
    }
    Serial.println("WEB >> " + currentLine);
    n = 3;
    while (n < 24) {
      args[n-3] = args[n];
      n = n + 1;
    }
    args[20] = ""; args[21] = ""; args[23] = ""; args[24] = "";
    processCMD(args);
  }
  else if (currentLine.startsWith("GET /HTP?") and millis() > lastMillis + 25) {
    Serial.println("HTP >> " + currentLine);
    String cmdConstruct = "";
    int z = 9;
    while (String(currentLine[z]) != String("x") and z != currentLine.length()) {
      cmdConstruct = cmdConstruct + currentLine[z];
      z = z + 1;
    }
    Serial.println(cmdConstruct);
    args[0] = cmdConstruct;
    int q = 1;
    z = z + 2;
    while (z - 2 != currentLine.length()) {
      cmdConstruct = "";
      while (String(currentLine[z]) != String("&") and z != currentLine.length() and String(currentLine[z]) != String(" ")) {
        cmdConstruct = cmdConstruct + currentLine[z];
        z = z + 1;
      }
      Serial.println("HTA >> " + cmdConstruct);
      args[q] = cmdConstruct;
      q = q + 1;
      while (z != currentLine.length() and String(currentLine[z]) != String("x")) { z++; }
      z = z + 2;
    }
    processCMD(args);
  }
}

void sendHTTP(WiFiClient client) {
  // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
  // and a content-type so the client knows what's coming, then a blank line:
  client.println("HTTP/1.1 200 OK");
  client.println("Content-type:text/html");
  client.println();

  // the content of the HTTP response follows the header:
  int q = 0;
  //client.print("<p>digitalWrite</p>");
  client.print("<p></p><form action=\"/HTP\"> <label for=\"digitalWritex\">Pin #</label> <input type=\"number\" id=\"digitalWritex\" name=\"digitalWritex\" value=\"0\">  <label for=\"x\">State</label> <select id=\"x\" name=\"x\"><option value=\"HIGH\">HIGH</option><option value=\"LOW\">LOW</option></select> <input type=\"submit\" value=\"digitalWrite\"> </form>");
  client.print("<p></p><form action=\"/HTP\"> <label for=\"pinModex\">Pin #</label> <input type=\"number\" id=\"pinModex\" name=\"pinModex\" value=\"0\">  <label for=\"x\">State</label> <select id=\"x\" name=\"x\"><option value=\"INPUT\">INPUT</option><option value=\"OUTPUT\">OUTPUT</option></select> <input type=\"submit\" value=\"pinMode\"> </form>");
  /*while (q <= 13)
  {
    client.print("<a href=\"/digitalWrite/" + String(q) + "/HIGH/run\">" + String(q) + " H </a><span>/</span>" + "<a href=\"/digitalWrite/" + String(q) + "/LOW/run\">" + String(q) + " L </a><span>/</span>");
    q = q + 1;
  }
  client.print("<p>pinMode</p>");
  q = 0;
  while (q <= 13)
  {
    client.print("<a href=\"/pinMode/" + String(q) + "/INPUT/run\">" + String(q) + " I </a><span>/</span>" + "<a href=\"/pinMode/" + String(q) + "/OUTPUT/run\">" + String(q) + " O </a><span>/</span>");
    q = q + 1;
  }*/
  
  client.print("<p></p><form action=\"/HTP\"> <label for=\"playNotex\">Play a Note Now  | Frequency (Hz)</label> <input type=\"number\" id=\"playNotex\" name=\"playNotex\" value=\"440\"> <label for=\"x\">Duration (ms)</label> <input type=\"number\" id=\"x\" name=\"x\" value=\"250\"><input type=\"submit\" value=\"Play\"> </form>");
  client.print("<p></p><form action=\"/HTP\"> <label for=\"addNotex\">Add Note to Song | Frequency (Hz)</label> <input type=\"number\" id=\"addNotex\" name=\"addNotex\" value=\"440\"> <label for=\"x\">Duration (ms)</label> <input type=\"number\" id=\"x\" name=\"x\" value=\"250\"><input type=\"submit\" value=\"Add\"> </form>");
  client.print("<p></p><form action=\"/HTP\"> <label for=\"pointerLocx\">Song Edit Cursor | Location</label> <input type=\"number\" id=\"pointerLocx\" name=\"pointerLocx\" value=\"0\"> <input type=\"submit\" value=\"Move Cursor\"> </form>");
  client.print("<p></p><form action=\"/HTP\"> <label for=\"playSongx\">Play the Song    | Start Position</label> <input type=\"number\" id=\"playSongx\" name=\"playSongx\" value=\"0\"> <input type=\"submit\" value=\"Play\"> </form>");
  
  // The HTTP response ends with another blank line:
  client.println();
  
}
