int song[512][2] = {};
int pointerLoc = 0;

void addNote(unsigned int note, int len) {
  Serial.println(pointerLoc);
  song[pointerLoc][0] = note;
  song[pointerLoc][1] = len;
  Serial.println(song[pointerLoc][0] + " " + song[pointerLoc][1]);
  pointerLoc = pointerLoc + 1;
}
