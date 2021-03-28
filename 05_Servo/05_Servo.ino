// Servo-Bibliothek einbinden
#include <Servo.h>

// lege ein neues Objekt vom Typ "Servo" mit dem Namen "servo" an
Servo servo;

void setup()
{
  // initialisiere Servo auf Pin 8
  servo.attach(8);

  pinMode(A0, INPUT);
}

void loop()
{
  int val = analogRead(A0);         // Wert von A0 einlesen und in "val" abspeichern
  val = map(val, 0, 1023, 0, 180);  // Wertebereich anpassen von 0-1023 zu 0-180
  servo.write(val);                 // Servo auf entsprechenden Winkel einstellen
  delay(15);                        // kurz warten, bis der Servo die neue Stellung erreicht hat
}
