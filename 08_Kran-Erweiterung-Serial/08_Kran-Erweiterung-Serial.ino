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

void setup()
{
  // konfiguriere Servos und Serial
  Serial.begin(115200);
  Serial.setTimeout(5);
  servoPitch.attach(servoPitchPin);
  servoYaw.attach(servoYawPin);

  Serial.println("Init done. Send positions in format <PITCH>,<YAW>\n");
}


// wird ausgeführt, wenn Serielle Daten eintreffen
// alternativ könnte auch in loop() regelmäßig geprüft werden, ob Daten angekommen sind
void serialEvent()
{
  while (Serial.available())
  {
    // Format: <PITCH>,<YAW>   z.B.: 30,50
    String input = Serial.readString();

    int delimiterPos = input.indexOf(',');
    String pitchStr = input.substring(0, delimiterPos);
    String yawStr = input.substring(delimiterPos + 1);

    // toInt() gibt bei Fehler 0 zurück
    pitchPos = constrain(pitchStr.toInt(), 0, 180);
    yawPos = constrain(yawStr.toInt(), 0, 180);

    Serial.println("New position: (" + String(pitchPos) + ',' + String(yawPos) + ')');
  }
}

void loop()
{
  servoPitch.write(pitchPos);
  servoYaw.write(yawPos);
}
