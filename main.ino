#include "MeAuriga.h"
#include <Wire.h>
#include <SoftwareSerial.h>

MeEncoderNew motor1(0x11, SLOT1);
MeEncoderNew motor2(0x11, SLOT2);
MeEncoderNew motor3(0x12, SLOT1);
MeEncoderNew motor4(0x12, SLOT2);

bool _dir = false;
int _speed = 50;
int _desiredSpeed;

MePort port(PORT_5);
Servo myservo1;  // create servo object to control a servo 
int16_t servo1pin =  port.pin1();//attaches the servo on PORT_3 SLOT1 to the servo object


void setup()
{
  initWheels();
  myservo1.attach(servo1pin);  // attaches the servo on servopin1
  Serial.begin(9600);
}

void initWheels() {
  motor1.begin();
  motor2.begin();
  motor3.begin();
  motor4.begin();
}

void moveForward(int s) {
  motor1.runSpeed(s);
  motor2.runSpeed(-s);
  motor3.runSpeed(-s);
  motor4.runSpeed(s);
}

void moveSideways(int s) {
  motor1.runSpeed(-s);
  motor2.runSpeed(-s);
  motor3.runSpeed(s);
  motor4.runSpeed(s);
}

void resetSpeed() {
  motor1.runSpeed(0);
  motor2.runSpeed(0);
  motor3.runSpeed(0);
  motor4.runSpeed(0);
}

void loop()
{
  if(_dir){
    _desiredSpeed = _speed;
  }else{
    _desiredSpeed = -_speed;
  }

  // myservo1.write(0);                  
  // delay(2000);                           
  // myservo1.write(180);
  // delay(2000); 

  moveForward(_desiredSpeed);
  delay(3000);
  resetSpeed();
  delay(1000);

  _dir = !_dir;
}
