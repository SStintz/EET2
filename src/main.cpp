#include <Arduino.h>

//Pinbelegung aus Versuchsstand
//int nullDurchgang = 0;
const int triacPin = 3;
const int interruptPin = 9;
const int taster1 = 0; //für dunkler, d.h Dim up
const int taster2 = 1;  //für heller, d.h Dim down

volatile bool nullDurchgang = false;
volatile unsigned long delaytime = 2000;
const unsigned long HALBEPERIODE_50HZ = 10000; // 10ms = 50Hz Halbwelle

unsigned long debounce = 50;                          // debounce time in ms
unsigned long lastDebounce_taster1 = 0;
unsigned long lastDebounce_taster2 = 0;

void erhoehen_Zundwinkels() {
  delaytime += 500;
  
  if (delaytime < 0) {
    delaytime = 0;
  } 
  else if (delaytime > HALBEPERIODE_50HZ) {
    delaytime = HALBEPERIODE_50HZ;
  }
}

void reduzieren_Zundwinkels() {
  delaytime -= 500;
  
  if (delaytime < 0) {
    delaytime = 0;
  } 
  else if (delaytime > HALBEPERIODE_50HZ) {
    delaytime = HALBEPERIODE_50HZ;
  }
}
 
void onZeroCross() {
  nullDurchgang = true;   // Nulldurchgang registrieren
}

void checkTaster() {
 static bool lastStateTaster1 = HIGH;
 static bool lastStateTaster2 = HIGH;

 bool stateTaster1 = digitalRead(taster1);
 bool stateTaster2 = digitalRead(taster2);

 if (stateTaster1 == LOW && lastStateTaster1 == HIGH && (millis() - lastDebounce_taster1 >= debounce)) {
  erhoehen_Zundwinkels();
  lastDebounce_taster1 = millis();
 }

 if (stateTaster2 == LOW && lastStateTaster2 == HIGH && (millis() - lastDebounce_taster2 >= debounce)) {
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
  pinMode(taster1, INPUT_PULLUP);
  pinMode(taster2, INPUT_PULLUP);

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












