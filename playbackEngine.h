#include "frequencies.h"
#include "compilationEngine.h"
#define speaker 10 // pin no. for speaker
int xOver = 499;


//Lawrence's playNote void here (replacement for Oliver's placeholder void)

void playTone(int buzzer, unsigned int note, int len) {
  tone(buzzer, note, len);
}

void playSong(int startPos) {
  int inLoop = 0;
  int loopedFrom = 0;
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
      if (line[0] == 0) {
        //Lawrence's other bit here
      } else {
        //Harry's stuff here
        if (line[0] == 1 && inLoop == 0) {
          loopedFrom = pointerLoc;
          pointerLoc = line[1];
          inLoop = 1;
        }
        
        if line[0] == 1 and inLoop == 1 and loopedFrom == pointerLoc;
        inLoop == 0;
        pointerLoc++;
        
        if line[0] == 2 and inLoop == 1 then;
        if line[2] == 0;
        pointerLoc == line[1];
        else {
          if line[2] != 0 and line[2] == loopedFrom;
          pointerLoc == line[1];
            pointerLoc++;
         
        if line[0] == 3 and inLoop == 1;
          if line[2] == 0;
          pointerLoc == line[1]
        else {
            if line[2] != 0 and line[2] == loopedFrom;
            pointerLoc == line[1];
              else {
              pointerLoc++;
              
      }
    }
  }
  pointerLoc = oldLoc;
}

void setPointer(int loc) {
  pointerLoc = loc;
}
