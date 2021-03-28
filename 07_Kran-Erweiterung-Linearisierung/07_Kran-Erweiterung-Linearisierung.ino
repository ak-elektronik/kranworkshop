#include <Servo.h>

// lege ein neues Objekt vom Typ "Servo" an
Servo servoPitch;
Servo servoYaw;

// Variablen zum Speichern der aktuellen Position
float pitchPos = 90;
float yawPos = 90;

// Konstante für die Sensitivität
const float sensitivity = 1.0f;
const float threshold = 0.01f;


void setup()
{
  // konfiguriere Servos, Pins und Serial
  Serial.begin(115200);
  servoPitch.attach(8);
  servoYaw.attach(7);
  pinMode(12, INPUT);
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
}

void loop()
{
  if(!digitalRead(12))
  {
    // wenn der Joystick gedrückt wird, sezte den Kran auf die Ausgangsstellung zurück
    pitchPos = 90;
    yawPos = 90;
  }
  else
  {
    // lese die Stellung des Joysticks ein
    int pitchRead = analogRead(A0);
    int yawRead = analogRead(A1);
    
    Serial.print(String(pitchRead) + ", " + String(yawRead) + "; ");
    
    // Wertebereich auf -pi bis +pi anpassen
    float pitchInput = (pitchRead - 517) / 1023.0f * PI;
    float yawInput = (yawRead - 497) / 1023.0f * PI;

    // nicht-Linearität des Joysticks ausgleichen, ergibt Werte zwischen -1 und 1
    pitchInput = sin(pitchInput);
    yawInput = sin(yawInput);

    Serial.print(String(pitchInput) + ", " + String(yawInput) + "; ");
    
    // wenn Mindestwert erreicht, addiere das Ergebnis zur aktuellen Position
    // verhindert, dass sich der Kran bewegt, ohne dass der Joystick verwendet wird
    if(abs(pitchInput) > threshold) pitchPos += pitchInput * sensitivity;
    if(abs(yawInput) > threshold) yawPos += yawInput * sensitivity;

    // verhindert, dass wir aus dem ansteuerbaren Bereich herauskommen
    pitchPos = constrain(pitchPos, 0, 180);
    yawPos = constrain(yawPos, 0, 180);
  }

  // setze die Servos auf die neue Stellung
  servoPitch.write(pitchPos);
  servoYaw.write(yawPos);

  Serial.println(String(pitchPos) + ", " + String(yawPos));
}
