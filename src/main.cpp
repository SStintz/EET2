#include <Arduino.h>

// Pinbelegung
const int triacPin = 3;
const int interruptPin = 9;
const int taster1 = 0; // Dimmer up (dunkler)
const int taster2 = 1; // Dimmer down (heller)


volatile bool nullDurchgang = false;
volatile unsigned long delaytime = 2000;

const unsigned long MIN_ZUNDWINKEL = 0;
const unsigned long MAX_ZUNDWINKEL = 9000; //??????Zuenden nicht mehr moeglich, da Haltestrom unterschritten wird

unsigned long debounce = 50;
unsigned long lastDebounce_taster1 = 0;
unsigned long lastDebounce_taster2 = 0;

bool lastStateTaster1 = LOW;
bool lastStateTaster2 = LOW;

bool blink = true;
unsigned long letzteBlinkZeit = 0;

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

      delayMicroseconds(delaytime);  // Dimmen mit Zündwinkel
      digitalWrite(triacPin, blink);  // Triac zünden
      delayMicroseconds(10);          // Kurzer Impuls
      digitalWrite(triacPin, LOW);
    }
    // else Lampe aus -> kein Triac-Impuls
  


void checkTaster() {
  bool stateTaster1 = digitalRead(taster1);
  bool stateTaster2 = digitalRead(taster2);

  if (stateTaster1 == HIGH && lastStateTaster1 == LOW && (millis() - lastDebounce_taster1 >= debounce)) {
    erhoehen_Zundwinkels();
    lastDebounce_taster1 = millis();
    lastStateTaster1 = HIGH;
  }
  else if (stateTaster1 == LOW && lastStateTaster1 == HIGH){
    lastStateTaster1 = LOW; // ruecksetzen  von lastStateTaster1 so, dass  neu State detektiert werden kann 
  }

  if (stateTaster2 == HIGH && lastStateTaster2 == LOW && (millis() - lastDebounce_taster2 >= debounce)) {
    reduzieren_Zundwinkels();
    lastDebounce_taster2 = millis();
    stateTaster2 = HIGH;
  }
  else if (stateTaster2 == LOW && lastStateTaster2 == HIGH ){
    lastStateTaster2 = LOW;
  }

}

void setup() {
  pinMode(triacPin, OUTPUT);
  digitalWrite(triacPin, LOW);
  pinMode(interruptPin, INPUT);
  pinMode(taster1, INPUT_PULLDOWN);
  pinMode(taster2, INPUT_PULLDOWN);

  attachInterrupt(digitalPinToInterrupt(interruptPin), onZeroCross, RISING);

  lastStateTaster1 = digitalRead(taster1);
  lastStateTaster2 = digitalRead(taster2);

  letzteBlinkZeit = millis();  // Blink-Timer starten
}

void loop() {

  checkTaster();
  // Blink-Status jede Sekunde wechseln
  if (millis() - letzteBlinkZeit >= 1000) {
    blink = !blink;
    letzteBlinkZeit = millis();
  }

}
