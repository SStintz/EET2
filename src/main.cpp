#include <Arduino.h>

//Pinbelegung aus Versuchsstand
//int nullDurchgang = 0;
const int triacPin = 3;
const int interruptPin = 9;
const int taster1 = 0; //für dunkler, d.h Dim up
const int taster2 = 1;  //für heller, d.h Dim down

volatile bool nullDurchgang = false;
volatile unsigned long delaytime = 2000;
//const unsigned long HALBEPERIODE_50HZ = 10000; // 10ms = 50Hz Halbwelle
const unsigned long MIN_ZUNDWINKEL = 0;    // minimaler Zündwinkel in Mikrosekunden (z.B. 0.5ms)
const unsigned long MAX_ZUNDWINKEL = 9000;

unsigned long debounce = 50;                          // debounce time in ms
unsigned long lastDebounce_taster1 = 0;
unsigned long lastDebounce_taster2 = 0;

void erhoehen_Zundwinkels() {
  delaytime += 100;
  if (delaytime > MAX_ZUNDWINKEL) {
    delaytime = MAX_ZUNDWINKEL;
  }
}

void reduzieren_Zundwinkels() {
  delaytime -= 100;
  if (delaytime < MIN_ZUNDWINKEL) {
    delaytime = MIN_ZUNDWINKEL;
  }
}
 
void onZeroCross() {
  nullDurchgang = true;   // Nulldurchgang registrieren
}

void checkTaster() {
 static bool lastStateTaster1 = LOW;
 static bool lastStateTaster2 = LOW;

 bool stateTaster1 = digitalRead(taster1);
 bool stateTaster2 = digitalRead(taster2);

 if (stateTaster1 == HIGH && lastStateTaster1 == LOW && (millis() - lastDebounce_taster1 >= debounce)) {
  erhoehen_Zundwinkels();
  lastDebounce_taster1 = millis();
 }

 if (stateTaster2 == HIGH && lastStateTaster2 == LOW && (millis() - lastDebounce_taster2 >= debounce)) {
  reduzieren_Zundwinkels();
  lastDebounce_taster2 = millis();
 }

 lastStateTaster1 = stateTaster1;
 lastStateTaster2 = stateTaster2;

}

void setup() {
  // initialisiert Pin 14 (LED1) als output
  pinMode(triacPin, OUTPUT);
  digitalWrite(triacPin, LOW);
  pinMode(interruptPin, INPUT);
  pinMode(taster1, INPUT_PULLDOWN);
  pinMode(taster2, INPUT_PULLDOWN);

  // Interrupt bei steigendem Signal (= Nulldurchgang)
  attachInterrupt(digitalPinToInterrupt(interruptPin), onZeroCross, RISING);
}

void loop() {
   if (nullDurchgang) { //if es gab Nulldurchgang, züruck auf False setzen
    nullDurchgang = false;

    delayMicroseconds(delaytime);  // Phasenanschnitt

    digitalWrite(triacPin, HIGH);            // Triac zünden
    delayMicroseconds(10);                    // kurze Pulsdauer
    digitalWrite(triacPin, LOW);               //Triac bleibt leitend, solange wieder Nulldurchgang
  }

   checkTaster();
 
}












