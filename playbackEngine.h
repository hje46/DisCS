#include "frequencies.h"
#include "compilationEngine.h"
#define speaker 10 // pin no. for speaker


void playNote(unsigned int note, int len) {
  tone(speaker, note, len);
}

void playTone(int buzzer, unsigned int note, int len) {
  tone(buzzer, note, len);
}

void playSong(int startPos) {
  if (startPos == "") { startPos = 0; }
  int oldLoc = pointerLoc;
  pointerLoc = startPos;
  int len = sizeof song / sizeof song[0];
  Serial.println(len);
  while (pointerLoc < len) {
    if (song[pointerLoc][0] == "") { break; } else {
      playTone(speaker, song[pointerLoc][0], song[pointerLoc][1]);
      delay(song[pointerLoc][1]);
      delay(10);
      pointerLoc = pointerLoc + 1;
    }
  }
  pointerLoc = oldLoc;
}

void setPointer(int loc) {
  pointerLoc = loc;
}
