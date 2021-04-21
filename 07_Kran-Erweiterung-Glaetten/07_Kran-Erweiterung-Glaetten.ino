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

// Array zum Glätten der Analog-Inputs
constexpr int filterDepth = 20;
int pitchHistory[filterDepth];
int yawHistory[filterDepth];
int historyIndex = 0;


void setup()
{
  // konfiguriere Servos, Pins und Serial
  Serial.begin(115200);
  servoPitch.attach(servoPitchPin);
  servoYaw.attach(servoYawPin);
  pinMode(joystickButtonPin, INPUT_PULLUP);
  pinMode(joystickXPin, INPUT);
  pinMode(joystickYPin, INPUT);

  // initialisiere Arrays auf 0
  for(int i = 0; i < filterDepth; i++)
  {
    pitchHistory[i] = 0;
    yawHistory[i] = 0;
  }

  // lese Joystick Offset, während dieser noch in Ruhelage ist
  pitchReadOffset = analogRead(joystickYPin);
  yawReadOffset = analogRead(joystickXPin);
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
    // neuen Wert zur Historie hinzufügen
    pitchHistory[historyIndex] = analogRead(joystickYPin) - pitchReadOffset;
    yawHistory[historyIndex] = analogRead(joystickXPin) - yawReadOffset;
    
    //Index erhöhen und ggf. zurücksetzen
    historyIndex = (historyIndex + 1) % 20;

    // Mittelwert aus Historie berechnen (dabei ggf. maximalen Wertebereich von int berücksichtigen!)
    int pitchTotal = 0;
    int yawTotal = 0;
    for(int i = 0; i < filterDepth; i++)
    {
      pitchTotal += pitchHistory[i];
      yawTotal += yawHistory[i];
    }
    float pitchAvg = pitchTotal / filterDepth;
    float yawAvg = yawTotal / filterDepth;
    // auch möglich: gewichteter Mittelwert, neuere Werte stärker gewichten => ergibt "echtes" Tiefpassverhalten
    
    Serial.print(String(pitchAvg) + ", " + String(yawAvg) + "; ");
    
    // auf die Prüfung mit Mindeständerung sollte immer noch nicht verzichtet werden
    if(abs(pitchAvg) > deadzone) pitchPos += pitchAvg * sensitivity;
    if(abs(yawAvg) > deadzone) yawPos += yawAvg * sensitivity;

    // verhindert, dass wir aus dem ansteuerbaren Bereich herauskommen
    pitchPos = constrain(pitchPos, 0, 180);
    yawPos = constrain(yawPos, 0, 180);
  }

  // setze die Servos auf die neue Stellung
  servoPitch.write(round(pitchPos));
  servoYaw.write(round(yawPos));

  Serial.println(String(pitchPos) + ", " + String(yawPos));
}
