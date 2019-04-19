#define aOut A18

void setup() {
}

void loop() {
  for(int i=0; i<255; i++){
    dacWrite(aOut,i);
    delay(10);
  }
}
