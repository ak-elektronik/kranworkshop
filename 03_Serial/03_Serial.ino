int counter = 0;  // Erstelle neue Variable vom Typ Integer mit Anfangswert 0

void setup()
{
  Serial.begin(115200); // Starte Kommunikation mit Computer
  Serial.println("Programm startet!");  // sende "Programm startet!"
}

void loop()
{
  Serial.print(counter);  // sende den Wert der Variable "counter"
  Serial.print(' ');      // sende ein Leerzeichen
  counter = counter + 1;  // erhöhe "counter" um 1
  delay(1000);
}
