#include "frequencies.h"
#include "compilationEngine.h"
#define speaker 10 // pin no. for speaker


//Lawrence's playNote void here (replacement for Oliver's placeholder void)

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
    int[] line = {};
    line[0] = song[pointerLoc][0];
    line[1] = song[pointerLoc][1];
    line[2] = song[pointerLoc][2];
    if (line[0] == "") { break; } else {
      if (line[0] == 1) {
        //Lawrence's other bit here
      } else {
        //Harry's stuff here
      }
    }
  }
  pointerLoc = oldLoc;
}

void setPointer(int loc) {
  pointerLoc = loc;
}
