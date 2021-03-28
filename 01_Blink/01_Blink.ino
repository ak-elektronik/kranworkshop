void setup()
{
  // Pin 13 als Ausgang konfigurieren
  pinMode(13, OUTPUT);
}

void loop()
{
  digitalWrite(13, HIGH); // Pin 13 anschalten
  delay(500);             // 0.5s warten
  digitalWrite(13, LOW);  // Pin 13 ausschalten
  delay(500);             // wieder warten, bevor es von vorne losgeht
}
