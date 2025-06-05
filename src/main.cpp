#include <Arduino.h>

// Pinbelegung
const int triacPin = 3;         // Ausgang zum Steuern des Triacs
const int interruptPin = 9;     // Eingang für den Nulldurchgang (Zero-Cross Detection)
const int taster1 = 0;          // Eingang für Taster 1 (Zündwinkel erhöhen -> Licht dunkler)
const int taster2 = 1;          // Eingang für Taster 2 (Zündwinkel verringern -> Licht heller)

// Zustandsvariablen
//volatile bool nullDurchgang = false;       
volatile unsigned long delaytime = 2000;   // Initialer Zündwinkel (µs Verzögerung nach Nulldurchgang)

const unsigned long MIN_ZUNDWINKEL = 0;        // Minimaler Zündwinkel (voll hell)
const unsigned long MAX_ZUNDWINKEL = 9000;     // Maximaler Zündwinkel (dunkel – danach hält Triac evtl. nicht mehr)

unsigned long debounce = 50;  // Entprellzeit für Taster
unsigned long lastDebounce_taster1 = 0;   // Zeitstempel der letzten Betätigung von Taster 1
unsigned long lastDebounce_taster2 = 0;   // Zeitstempel der letzten Betätigung von Taster 2

bool lastStateTaster1 = LOW;  // Letzter Zustand von Taster 1
bool lastStateTaster2 = LOW;  // Letzter Zustand von Taster 2

bool blink = true;            // Status für Blinken (an/aus), steuert ob Triac-Impuls ausgelöst wird
unsigned long letzteBlinkZeit = 0; // Zeitstempel für letztes Umschalten des Blink-Status

// Erhöht den Zündwinkel (Licht wird dunkler)
void erhoehen_Zundwinkels() {
  delaytime += 100;
  if (delaytime > MAX_ZUNDWINKEL) {
    delaytime = MAX_ZUNDWINKEL;
  }
}

// Verringert den Zündwinkel (Licht wird heller)
void reduzieren_Zundwinkels() {
  delaytime -= 100;
  if (delaytime < MIN_ZUNDWINKEL) {
    delaytime = MIN_ZUNDWINKEL;
  }
}

// Interrupt-Service-Routine: Wird bei Nulldurchgang aufgerufen
void onZeroCross() {
  delayMicroseconds(delaytime);  // Wartezeit für Zündwinkel-Dimmung
  digitalWrite(triacPin, blink); // Triac nur zünden, wenn blink == true
  delayMicroseconds(10);         // Kurzer Zündimpuls
  digitalWrite(triacPin, LOW);   // Triac-Ausgang wieder deaktivieren
}

// Überprüft die Tasterzustände und ändert den Zündwinkel entsprechend
void checkTaster() {
  bool stateTaster1 = digitalRead(taster1);
  bool stateTaster2 = digitalRead(taster2);

  // Taster 1 gedrückt -> Zündwinkel erhöhen (dunkler)
  if (stateTaster1 == HIGH && lastStateTaster1 == LOW && (millis() - lastDebounce_taster1 >= debounce)) {
    erhoehen_Zundwinkels();
    lastDebounce_taster1 = millis();
    lastStateTaster1 = HIGH;
  }
  //  // Wenn Taster 1 jetzt losgelassen wurde, Zustand zurücksetzen für erneute Erkennung
  else if (stateTaster1 == LOW && lastStateTaster1 == HIGH) {
    lastStateTaster1 = LOW; 
  }

  // Taster 2 gedrückt -> Zündwinkel verringern (heller)
  if (stateTaster2 == HIGH && lastStateTaster2 == LOW && (millis() - lastDebounce_taster2 >= debounce)) {
    reduzieren_Zundwinkels();
    lastDebounce_taster2 = millis();
    lastStateTaster2 = HIGH;
  }
    // Wenn Taster 2 jetzt losgelassen wurde, Zustand zurücksetzen für erneute Erkennung
  else if (stateTaster2 == LOW && lastStateTaster2 == HIGH) {
    lastStateTaster2 = LOW;
  }
}

void setup() {
  pinMode(triacPin, OUTPUT);
  digitalWrite(triacPin, LOW); // Triac initial deaktivieren
  pinMode(interruptPin, INPUT); // Nulldurchgang als Eingang
  pinMode(taster1, INPUT_PULLDOWN); // Taster mit Pulldown-Widerstand
  pinMode(taster2, INPUT_PULLDOWN);

  attachInterrupt(digitalPinToInterrupt(interruptPin), onZeroCross, RISING); // Interrupt bei steigender Flanke (Nulldurchgang)

  // Anfangszustände der Taster lesen
  lastStateTaster1 = digitalRead(taster1);
  lastStateTaster2 = digitalRead(taster2);

  letzteBlinkZeit = millis(); // Startzeit für Blink-Logik
}

void loop() {
  checkTaster(); // Taster prüfen

  // Blinkstatus jede Sekunde umschalten 
  if (millis() - letzteBlinkZeit >= 1000) {
    blink = !blink;
    letzteBlinkZeit = millis();
  }
}
