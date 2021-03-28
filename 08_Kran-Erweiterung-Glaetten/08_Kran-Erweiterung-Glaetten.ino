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

// Array zum Glätten der Analog-Inputs
#define HISTORY_DEPTH 20
int pitchHistory[HISTORY_DEPTH];
int yawHistory[20];
int historyIndex = 0;


void setup()
{
  // konfiguriere Servos, Pins und Serial
  Serial.begin(115200);
  servoPitch.attach(8);
  servoYaw.attach(7);
  pinMode(12, INPUT);
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);

  for(int i = 0; i < HISTORY_DEPTH; i++)
  {
    pitchHistory[i] = 0;
    yawHistory[i] = 0;
  }
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
    // neuen Wert zur Historie hinzufügen, Index erhöhen und ggf. zurücksetzen
    pitchHistory[historyIndex] = analogRead(A0) - 517;
    yawHistory[historyIndex] = analogRead(A1) - 497;
    historyIndex = (historyIndex + 1) % 20;

    // Mittelwert aus Historie berechnen (dabei ggf. maximalen Wertebereich von int berücksichtigen!)
    int pitchTotal = 0;
    int yawTotal = 0;
    for(int i = 0; i < HISTORY_DEPTH; i++)
    {
      pitchTotal += pitchHistory[i];
      yawTotal += yawHistory[i];
    }
    float pitchAvg = pitchTotal / HISTORY_DEPTH;
    float yawAvg = yawTotal / HISTORY_DEPTH;
    // auch möglich: gewichteter Mittelwert, neuere Werte stärker gewichten => ergibt "echtes" Tiefpassverhalten
    
    Serial.print(String(pitchAvg) + ", " + String(yawAvg) + "; ");
    
    // Wertebereich auf -pi bis +pi anpassen
    float pitchInput = pitchAvg / 1023.0f * PI;
    float yawInput = yawAvg / 1023.0f * PI;

    // nicht-Linearität des Joysticks ausgleichen, ergibt Werte zwischen -1 und 1
    pitchInput = sin(pitchInput);
    yawInput = sin(yawInput);

    Serial.print(String(pitchInput) + ", " + String(yawInput) + "; ");
    
    // auf die Prüfung mit Mindeständerung kann nun sogar verzichtet werden!
    /*if(abs(pitchInput) > threshold)*/ pitchPos += pitchInput * sensitivity;
    /*if(abs(yawInput) > threshold)*/ yawPos += yawInput * sensitivity;

    // verhindert, dass wir aus dem ansteuerbaren Bereich herauskommen
    pitchPos = constrain(pitchPos, 0, 180);
    yawPos = constrain(yawPos, 0, 180);
  }

  // setze die Servos auf die neue Stellung
  servoPitch.write(pitchPos);
  servoYaw.write(yawPos);

  Serial.println(String(pitchPos) + ", " + String(yawPos));
}
