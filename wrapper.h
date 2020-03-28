#include "arduino_secrets.h"
#include "playbackEngine.h" // this incluce contains the include for "compilationEngine.h" thus is it not required here
#include "frequencies.h"
int wifiStat[] = {0, 0}; //0 = off (led off), 1 = connecting, 2 = connected (led on), 3 = failed (led flashing). 4 = complete [1] refers to LED status
char ssid[] = SECRET_SSID;        // your network SSID (name)
char pass[] = SECRET_PASS;    // your network password (use for WPA, or use as key for WEP)

void processCMD(String cmd[])
{
  //Print ARGS
  Serial.println("ARGS>> " + cmd[0] + "/" + cmd[1] + "/" + cmd[2]);
  
  //digitalWrite
  if (cmd[0] == "digitalWrite") {
    if (cmd[2] == "HIGH") {
      digitalWrite(cmd[1].toInt(), HIGH);               // GET /H turns the LED on
    }
    else if (cmd[2] == "LOW") { digitalWrite(cmd[1].toInt(), LOW); }
  }
  
  //pinMode
  else if (cmd[0] == "pinMode") {
    if (cmd[2] == "OUTPUT") {
      pinMode(cmd[1].toInt(), OUTPUT);               // GET /H turns the LED on
    }
    else if (cmd[2] == "INPUT") { pinMode(cmd[1].toInt(), INPUT); }
  }

  //wifiStat
  else if (cmd[0] == "status") {
    wifiStat[0] = cmd[1].toInt();
  }
  else if (cmd[0] == "reconnect") {
    //Program me if wanted
  }
  else if (cmd[0] == "tone") {
    if (cmd[3].toInt() > 10000) { cmd[3] = "10000"; }
    if (cmd[1].toInt() < 14) {
      playTone(cmd[1].toInt(), cmd[2].toInt(), cmd[3].toInt());
    }
  }
  else if (cmd[0] == "addNote") {
    int len = cmd[2].toInt();
    if (len > 500) { len = 500; }
    unsigned int note = cmd[1].toInt();
    addNote(note, len);
  }
  else if (cmd[0] == "playSong") {
    if (cmd[1] == "") { playSong(0); } else { playSong(cmd[1].toInt()); }
  }
  else if (cmd[0] == "pointerLoc" || cmd[0] == "setLoc" || cmd[0] == "")
  {
    int pos = cmd[1].toInt();
    if (pos > 511) { pos = 1;}
    setPointer(pos);
  }
  //simplified cmd
  else if (cmd[0] == "playNote") {
    int len = cmd[2].toInt();
    if (len > 500) { len = 500; }
    unsigned int note = cmd[1].toInt();
    playNote(note, len);
  }
  Serial.println("Parsed.");
}
