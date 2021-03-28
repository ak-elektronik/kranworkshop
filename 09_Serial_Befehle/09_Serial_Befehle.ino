#include <Servo.h>

Servo servoPitch;
Servo servoYaw;

float pitch;
float yaw;

void setup() {
Serial.begin(115200); 
Serial.setTimeout(5);
 
  servoPitch.attach(8);
  servoYaw.attach(7);
}


// wird ausgeführt, wenn Serielle Daten anliegen
void serialEvent() {
  
        while (Serial.available()) {

                char recievedString[7];
                byte recievedStringLength;

                // Char-Array mit 0 terminiert -> zum C-String gemacht
                recievedStringLength = Serial.readBytes(recievedString, 7);
                recievedString[recievedStringLength] = 0;

                //Format: Pitch,Yaw   z.B.: 30,50
                char *p;
                p = strtok(recievedString, ",");
                pitch = atof(p);
                p = strtok(nullptr, ",");
                yaw = atof(p);
                pitch = constrain(pitch, 0.0f, 180.0f);
                yaw = constrain(yaw, 0.0f, 180.0f);
                }
}

void loop(){
  servoPitch.write(pitch);
  servoYaw.write(yaw);
}
