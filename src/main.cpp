#include <Arduino.h>

//Pinbelegung aus Versuchsstand
int nullDurchgang = 0;
const int triacPin = 3;
const int interruptPin = 9;

void zaehleNullDurchgang() {
    nullDurchgang++;
}

void setup() {
  // initialisiert Pin 14 (LED1) als output
  pinMode(triacPin, OUTPUT);
  pinMode(interruptPin, INPUT);

  // Interrupt bei steigendem Signal (= Nulldurchgang)
  attachInterrupt(digitalPinToInterrupt(interruptPin), zaehleNullDurchgang, RISING);
}

void loop() {
  if (nullDurchgang >= 10) {
    digitalWrite(triacPin, HIGH);
    delay(10); // Nur kurz einschalten
    digitalWrite(triacPin, LOW);

    nullDurchgang = 0; // Zähler zurücksetzen
  }
 
}
 


