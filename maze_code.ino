#include <Servo.h>

Servo servo1;
Servo servo2;
int joyX = 0; // pin ca joystick-ul sa se poata misca pe axa x
int joyY = 1; // pin ca joystick-ul sa se poata misca pe axa y

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
  //maparea valorilor pe care le citesc de la controalele joystick-ului
  servoVal = analogRead(joyX);
  int servo1Pos = map(servoVal, 0, 1023, 180, 130);
  servo1.write(servo1Pos);

  servoVal = analogRead(joyY);
  int servo2Pos = map(servoVal, 0, 1023, 130, 180);
  servo2.write(servo2Pos);


}


