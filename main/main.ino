#include "MeAuriga.h"
#include <Wire.h>
#include <SoftwareSerial.h>

MeEncoderNew motor1(0x11, SLOT1);
MeEncoderNew motor2(0x11, SLOT2);
MeEncoderNew motor3(0x12, SLOT1);
MeEncoderNew motor4(0x12, SLOT2);

bool _dir = false;
int _speed = 30;
int _desiredSpeed;

float ultraAngle = 30.0;

const float pi = 3.14;

float coneDistance;

int delayTime = 100;

bool searchForBlackLine = true;
bool foundItem = false;
bool firstTime = true;

MePort port(PORT_5);
Servo myservo1;  // create servo object to control a servo 
int16_t servo1pin =  port.pin1();//attaches the servo on PORT_3 SLOT1 to the servo object

MeUltrasonicSensor ultraSensorLeft(PORT_10); /* Ultrasonic module can ONLY be connected to port 3, 4, 6, 7, 8 of base shield. */
MeUltrasonicSensor ultraSensorForward(PORT_9);

MeLineFollower lineFinderLeft(PORT_8);
MeLineFollower lineFinderForward(PORT_7);
MeLineFollower lineFinderRight(PORT_6);


void setup()
{
  ultraAngle = (30.0/180.0)*pi;
  
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

void moveSideways(int s) {
  motor1.runSpeed(s);
  motor2.runSpeed(-s);
  motor3.runSpeed(-s);
  motor4.runSpeed(s);
}

void moveForward(int s) {
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

void spinAround(int s) {
  motor1.runSpeed(s);
  motor2.runSpeed(s);
  motor3.runSpeed(s);
  motor4.runSpeed(s);
}

void loop()
{
  delayTime = 50;
  // - speed right / + speed left
  // - speed backwards / + speed forward
  int lineFollowerLeftState = lineFinderLeft.readSensors();
  int lineFollowerForwardState = lineFinderForward.readSensors();
  int lineFollowerRightState = lineFinderRight.readSensors();

  //reached black line
  if(lineFollowerLeftState == S1_IN_S2_IN && searchForBlackLine == true) {
    resetSpeed();
    // if (firstTime) {
    //   moveSideways(_speed);
    //   delay(400);
    //   resetSpeed(); 
    //   firstTime = false; 
    // }

    searchForBlackLine = false;
  }
  
  if(searchForBlackLine == true) {
    resetSpeed();
    moveSideways(-30);
  } else {
    float itemDistance = ultraSensorLeft.distanceCm();
    if (lineFollowerForwardState == S1_IN_S2_IN) {
      resetSpeed();
    } else if(itemDistance < 100 || foundItem == true) {
      if (foundItem == false) {
        foundItem = true;
        resetSpeed();
        moveForward(_speed);
        if(itemDistance > 8 && itemDistance < 25) {
          delayTime = 300;
        } else if (itemDistance > 25 && itemDistance < 45) {
          delayTime = 700;
        } else if (itemDistance > 45 && itemDistance < 70) {
          delayTime = 1200;
        } else  {
          delayTime = 1200;
        }
      } else if (ultraSensorLeft.distanceCm() < 8) {
        resetSpeed();
        // execute grap logic
      } else {
        if(lineFollowerRightState == S1_IN_S2_IN) {
          searchForBlackLine = true;
          foundItem = false;
          resetSpeed();
        } else {
          resetSpeed();
          moveSideways(_speed);
        }
      }
    } else if (ultraSensorForward.distanceCm() < 8) {
      resetSpeed();
      moveForward(-_speed);
      delay(300);
      spinAround(-_speed);
      delay(2600);
      // execute grap logic
    } else {
      if(foundItem == false) {
        moveForward(_speed);
      }
    }
  }



  delay(delayTime);
}
