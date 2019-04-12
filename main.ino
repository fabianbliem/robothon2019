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

void setup()
{
  motor1.begin();
  motor2.begin();
  motor3.begin();
  motor4.begin();
  Serial.begin(9600);
}

void loop()
{
  if(_dir){
    _desiredSpeed = _speed;
  }else{
    _desiredSpeed = -_speed;
  }
  //forward +--+
  //right ++--
  motor1.runSpeed(_desiredSpeed);
  motor2.runSpeed(-_desiredSpeed);
  motor3.runSpeed(-_desiredSpeed);
  motor4.runSpeed(_desiredSpeed);
  delay(3000);
  motor1.runSpeed(0);
  motor2.runSpeed(0);
  motor3.runSpeed(0);
  motor4.runSpeed(0);
  delay(3000);

  _dir = !_dir;
}
