int song[512][3] = {};
int pointerLoc = 0;

void addNote(unsigned int note, int len) {
  Serial.println(pointerLoc);
  song[pointerLoc][0] = 0;
  song[pointerLoc][1] = note;
  song[pointerLoc][2] = len;
  Serial.println(song[pointerLoc][0] + " " + song[pointerLoc][1] + " " + song[pointerLoc][2]);
  pointerLoc = pointerLoc + 1;
}

void addControl(int code, int arg1, int arg2) {
 Serial.println(pointerLoc);
  //Implement parity checks here
  song[pointerLoc][0] = 0;
  song[pointerLoc][1] = arg1;
  song[pointerLoc][2] = arg2;
  Serial.println(song[pointerLoc][0] + " " + song[pointerLoc][1] + " " + song[pointerLoc][2]);
  pointerLoc = pointerLoc + 1;
}
