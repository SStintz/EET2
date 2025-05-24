#include <Arduino.h>

//Pinbelegung aus Versuchsstand
//int nullDurchgang = 0;
const int triacPin = 3;
const int interruptPin = 9;
bool zerocrossFlag = false;
const unsigned long delayAfterZeroCross = 8000;

void setup() {
  // initialisiert Pin 14 (LED1) als output
  pinMode(triacPin, OUTPUT);
  digitalWrite(triacPin, LOW);
  pinMode(interruptPin, INPUT);

  // Interrupt bei steigendem Signal (= Nulldurchgang)
  attachInterrupt(digitalPinToInterrupt(interruptPin), onZeroCross, RISING);
}

void loop() {
   if (zerocrossFlag) { //if es gab Nulldurchgang, züruck auf False setzen
    zerocrossFlag = false;

    delayMicroseconds(delayAfterZeroCross);  // Phasenanschnitt

    digitalWrite(triacPin, HIGH);            // Triac zünden
    delayMicroseconds(10);                    // kurze Pulsdauer
    digitalWrite(triacPin, LOW);               //Triac bleibt leitend, solange wieder Nulldurchgang
  }
 
}
 
void onZeroCross() {
  zerocrossFlag = true;   // Nulldurchgang registrieren
}






