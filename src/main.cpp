#include <Arduino.h>

void setup() {
  // initialisiert Pin 14 (LED1) als output
  pinMode(14, OUTPUT);
}
//LED 1 blinkt im Zweisekundentakt

void loop() {
  digitalWrite(14, HIGH);
  delay(1000);
  digitalWrite(14, LOW);
  delay(1000);
}

