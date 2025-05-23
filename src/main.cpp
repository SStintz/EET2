#include <Arduino.h>

//Pinbelegung aus Versuchsstand

const int taster1 = 0;
const int taster2 = 1;
const int led1 = 14; 

void setup() {
  // initialisiert Pin 14 (LED1) als output
  pinMode(taster1, INPUT);
  pinMode(taster2, INPUT);
  pinMode(led1, OUTPUT);
}
//LED 1 blinkt im Zweisekundentakt

void loop() {
  if (digitalRead(taster1) == HIGH) {
    digitalWrite(led1, HIGH);
  }
  if (digitalRead(taster2) == HIGH) {
    digitalWrite(led1, LOW);
  }
  //digitalWrite(14, HIGH);
  //delay(1000);
  //digitalWrite(14, LOW);
  //delay(1000);
}

