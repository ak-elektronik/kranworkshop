int brightness = 0;

void setup()
{
  // Serielle Schnittstelle starten und IO konfigurieren
  Serial.begin(115200);
  pinMode(11, OUTPUT);
  pinMode(A0, INPUT);
}

void loop()
{
  // vom Pin A0 eingelesenen Analogwert an Computer senden
  Serial.println(analogRead(A0));

  // Pin 11 "analog" (per PWM) auf den Wert von brightness setzen
  // nicht alle Pins können PWM!
  // https://www.arduino.cc/reference/en/language/functions/analog-io/analogwrite/
  analogWrite(11, brightness);

  // brightness erhöhen, bis Maximalwert erreicht wird. Dann zurücksetzen
  brightness++;
  if(brightness > 255)
  {
    brightness = 0;
  }

  // kurz warten, um Effekt sehen zu können
  delay(10);
}
