#include <Servo.h>

// lege ein neues Objekt vom Typ "Servo" an
Servo servoPitch;
Servo servoYaw;

// Variablen zum Speichern der aktuellen Position
float pitchPos = 90;
float yawPos = 90;

// Konstante für die Sensitivität
const float sensitivity = 0.001f;
int pitchReadOffset;
int yawReadOffset;

void setup()
{
  // konfiguriere Servos, Pins und Serial
  Serial.begin(115200);
  servoPitch.attach(8);
  servoYaw.attach(7);
  pinMode(12, INPUT_PULLUP);
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pitchReadOffset = analogRead(A0);
  yawReadOffset = analogRead(A1);
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

   Serial.print("pitch Read:" + String(pitchRead) + ", "+ "yaw Read:" + String(yawRead) + "; ");

    // subtrahiere X, um den den Wertebereich anzupassen, sodass sich 0 bei der Mittelstellung ergibt (alternativ: map())
    // multipliziere mit der Sensitivität, da sonst selbst kleinste Bewegungen zum Vollausschlag führen
    float pitchInput = (pitchRead - pitchReadOffset) * sensitivity;
    float yawInput = (yawRead - yawReadOffset) * sensitivity;

    Serial.print("pitchInput:" + String(pitchInput) + ", " + "yawInput:" + String(yawInput) + "; ");

    // wenn Mindestwert erreicht, addiere das Ergebnis zur aktuellen Position
    // verhindert, dass sich der Kran bewegt, ohne dass der Joystick verwendet wird
    
    if(abs(pitchInput) > sensitivity * 100.0f) pitchPos += pitchInput;
    if(abs(yawInput) > sensitivity * 100.0f) yawPos += yawInput;

    // verhindere, dass wir aus dem ansteuerbaren Bereich herauskommen
    pitchPos = constrain(pitchPos, 0.0f, 180.0f);
    yawPos = constrain(yawPos, 0.0f, 180.0f);
  }

  // setze die Servos auf die neue Stellung
  servoPitch.write(pitchPos);
  servoYaw.write(yawPos);
  delay(5);   // kurz warten, um Störungen zwischen Servo und Joystick zu vermeiden

  Serial.println("pitchPos:" + String(pitchPos) + ", " + "yawPos:" + String(yawPos));
}
