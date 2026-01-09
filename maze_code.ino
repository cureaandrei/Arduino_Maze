#include <Servo.h>

Servo servo1;
Servo servo2;
int joyX = 0; 
int joyY = 1; 

int servoVal;
unsigned long previousMillis = 0; 
const long interval = 500;        

void setup()
{
  Serial.begin(9600); 
  servo1.attach(3);    
  servo2.attach(5);    
}

void loop()
{
  servoVal = analogRead(joyX);
  int servo1Pos = map(servoVal, 0, 1023, 180, 130);
  servo1.write(servo1Pos);

  servoVal = analogRead(joyY);
  int servo2Pos = map(servoVal, 0, 1023, 130, 180);
  servo2.write(servo2Pos);

  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval)
  {
    previousMillis = currentMillis; 

    Serial.print("Joystick X: ");
    Serial.print(analogRead(joyX));
    Serial.print(" -> Servo1 Position: ");
    Serial.println(servo1Pos);

    Serial.print("Joystick Y: ");
    Serial.print(analogRead(joyY));
    Serial.print(" -> Servo2 Position: ");
    Serial.println(servo2Pos);
  }

  delay(50); 
}


