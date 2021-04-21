void setup()
{
  // konfiguriere IO Pins
  pinMode(12, INPUT);
  pinMode(13, OUTPUT);
}

void loop()
{
  bool button = digitalRead(12);  // lese den Status des Pins 12 ein und speichere das Ergebnis in "button"
  digitalWrite(13, button);       // setze Pin 13 auf den eingelesenen Wert
}
