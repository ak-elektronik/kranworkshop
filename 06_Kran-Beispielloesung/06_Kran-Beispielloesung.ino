#include <Servo.h>

// Pinbelegung
constexpr int joystickButtonPin = 12;
constexpr int joystickXPin = A0;
constexpr int joystickYPin = A1;
constexpr int servoPitchPin = 7;
constexpr int servoYawPin = 8;

// lege ein neues Objekt vom Typ "Servo" an
Servo servoPitch;
Servo servoYaw;

// Variablen zum Speichern der aktuellen Position
float pitchPos = 90;
float yawPos = 90;

// Konstante für die Sensitivität
constexpr float sensitivity = 0.001f;
constexpr float deadzone = sensitivity * 5.0f;

// Nullposition des Joysticks
int pitchReadOffset;
int yawReadOffset;

void setup()
{
  // konfiguriere Servos, Pins und Serial
  Serial.begin(115200);
  servoPitch.attach(servoPitchPin);
  servoYaw.attach(servoYawPin);
  pinMode(joystickButtonPin, INPUT_PULLUP);
  pinMode(joystickXPin, INPUT);
  pinMode(joystickYPin, INPUT);

  // lese Joystick Offset, während dieser noch in Ruhelage ist
  pitchReadOffset = analogRead(joystickYPin);
  yawReadOffset = analogRead(joystickXPin);
}

void loop()
{
  if(!digitalRead(joystickButtonPin))
  {
    // wenn der Joystick gedrückt wird, sezte den Kran auf die Ausgangsstellung zurück
    pitchPos = 90;
    yawPos = 90;
  }
  else
  {
    // lese die Stellung des Joysticks ein
    int pitchRead = analogRead(joystickYPin);
    int yawRead = analogRead(joystickXPin);

    Serial.print("pitchRead:" + String(pitchRead) + ", "+ "yawRead:" + String(yawRead) + "; ");

    // subtrahiere X, um den den Wertebereich anzupassen, sodass sich 0 bei der Mittelstellung ergibt (alternativ: map())
    // multipliziere mit der Sensitivität, da sonst selbst kleinste Bewegungen zum Vollausschlag führen
    float pitchInput = (pitchRead - pitchReadOffset) * sensitivity;
    float yawInput = (yawRead - yawReadOffset) * sensitivity;

    Serial.print("pitchInput:" + String(pitchInput) + ", " + "yawInput:" + String(yawInput) + "; ");

    // wenn Mindestwert erreicht, addiere das Ergebnis zur aktuellen Position
    // verhindert, dass sich der Kran bewegt, ohne dass der Joystick verwendet wird
    if(abs(pitchInput) > deadzone) pitchPos += pitchInput;
    if(abs(yawInput) > deadzone) yawPos += yawInput;

    // verhindere, dass wir aus dem ansteuerbaren Bereich herauskommen
    pitchPos = constrain(pitchPos, 0.0f, 180.0f);
    yawPos = constrain(yawPos, 0.0f, 180.0f);
  }

  // setze die Servos auf die neue Stellung
  servoPitch.write(round(pitchPos));
  servoYaw.write(round(yawPos));

  Serial.println("pitchPos:" + String(pitchPos) + ", " + "yawPos:" + String(yawPos));
}
