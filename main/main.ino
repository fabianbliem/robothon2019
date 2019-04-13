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

MeDCMotor needle(PORT_4);
MeDCMotor claw(PORT_3);

MePort port(PORT_5);
Servo crane;              // create servo object to control a servo
int16_t cranePin = port.pin2(); //attaches the servo on PORT_3 SLOT1 to the servo object

MeUltrasonicSensor ultraSensorLeft(PORT_10); /* Ultrasonic module can ONLY be connected to port 3, 4, 6, 7, 8 of base shield. */
MeUltrasonicSensor ultraSensorForward(PORT_9);

MeLineFollower lineFinderLeft(PORT_6);
MeLineFollower lineFinderForward(PORT_7);
MeLineFollower lineFinderRight(PORT_8);

#define IDLE -1
#define SEARCH_BORDER 0
#define SEARCH_ITEM 1
#define GO_TO_ITEM 2
#define PICK_UP_ITEM 3
#define PUT_ITEM 4
#define BLACK S1_IN_S2_IN
#define MOTOR_SPEED 30

int state;

void initWheels()
{
  motor1.begin();
  motor2.begin();
  motor3.begin();
  motor4.begin();
}

void moveLeft(int s)
{
  moveX(abs(s));
}

void moveRight(int s)
{
  moveX(-abs(s));
}

void moveForward(int s)
{
  moveY(abs(s));
}

void moveBackwards(int s)
{
  moveY(-abs(s));
}

float deg2Rad(float d)
{
  return d * (PI / 180);
}

void moveX(int s)
{
  motor1.runSpeed(s);
  motor2.runSpeed(-s);
  motor3.runSpeed(-s);
  motor4.runSpeed(s);
}

void moveY(int s)
{
  motor1.runSpeed(-s);
  motor2.runSpeed(-s);
  motor3.runSpeed(s);
  motor4.runSpeed(s);
}

void resetSpeed()
{
  motor1.runSpeed(0);
  motor2.runSpeed(0);
  motor3.runSpeed(0);
  motor4.runSpeed(0);
}

void spinAround(int s)
{
  motor1.runSpeed(s);
  motor2.runSpeed(s);
  motor3.runSpeed(s);
  motor4.runSpeed(s);
}

void setup()
{
  initWheels();
  crane.attach(cranePin);
  Serial.begin(9600);

  state = SEARCH_BORDER;
}

void loop()
{
  delay(50);

  int lineStateLeft = lineFinderLeft.readSensors();
  int lineStateForward = lineFinderForward.readSensors();
  int lineStateRight = lineFinderRight.readSensors();
  float dist = ultraSensorLeft.distanceCm();

  switch (state)
  {
  case SEARCH_BORDER:
    moveRight(MOTOR_SPEED);
    if (lineStateRight == BLACK)
    {
      state = SEARCH_ITEM;
      moveLeft(MOTOR_SPEED);
      delay(100);
    }
    break;
  case SEARCH_ITEM:
    moveForward(MOTOR_SPEED);
    if (dist < 100)
    {
      float a = sin(deg2Rad(15.0)) * dist;
      float time = a / 15.7;

      delay((int)(time*1000));

      state = GO_TO_ITEM;
    }
    if(lineStateForward == BLACK){
      resetSpeed();
      state = IDLE;
    }
    break;
  case GO_TO_ITEM:
    moveLeft(MOTOR_SPEED);
    if (dist < 6)
    {
      state = PICK_UP_ITEM;
    }
    if(lineStateLeft == BLACK){
      resetSpeed();
      state = SEARCH_BORDER;
    }
    break;
  case PICK_UP_ITEM:
    resetSpeed();
    delay(1000);


    crane.write(180); //front
    delay(3000);

    //open claw
    claw.run(-50);
    delay(2000);
    claw.stop();

    delay(1000);

    needle.run(-50);
    delay(2000);
    needle.stop();
    delay(1000);
    
    needle.run(50);
    delay(3500);
    needle.stop();
    delay(1000);

    //close claw
    claw.run(50);
    delay(3000);
    claw.stop();

    state = PUT_ITEM;
    // crane.write(0); //basket
    // delay(5000);
    
    break;
  case PUT_ITEM:
    delay(1000);
    crane.write(0); 
    delay(3000);

     //open claw
    claw.run(-50);
    delay(2000);
    claw.stop();
    delay(1000);

    needle.run(-50);
    delay(1000);
    needle.stop();
    delay(1000);

    //close claw
    claw.run(50);
    delay(3000);
    claw.stop();
    delay(1000);

    needle.run(50);
    delay(2000);
    needle.stop();
    delay(1000);
    break;
  case IDLE:
    break;
  }

  //reached black line
  // if (lineFollowerLeftState == S1_IN_S2_IN && searchForBlackLine == true)
  // {
  //   resetSpeed();

  //   searchForBlackLine = false;
  // }

  // if (searchForBlackLine == true)
  // {
  //   resetSpeed();
  //   moveSideways(-30);
  // }
  // else
  // {
  //   float itemDistance = ultraSensorLeft.distanceCm();
  //   if (lineFollowerForwardState == S1_IN_S2_IN)
  //   {
  //     resetSpeed();
  //   }
  //   else if (itemDistance < 100 || foundItem == true)
  //   {
  //     if (foundItem == false)
  //     {
  //       foundItem = true;
  //       resetSpeed();
  //       moveForward(_speed);
  //       if (itemDistance > 8 && itemDistance < 25)
  //       {
  //         delayTime = 300;
  //       }
  //       else if (itemDistance > 25 && itemDistance < 45)
  //       {
  //         delayTime = 700;
  //       }
  //       else if (itemDistance > 45 && itemDistance < 70)
  //       {
  //         delayTime = 1200;
  //       }
  //       else
  //       {
  //         delayTime = 1200;
  //       }
  //     }
  //     else if (ultraSensorLeft.distanceCm() < 8)
  //     {
  //       resetSpeed();
  //       // execute grap logic
  //     }
  //     else
  //     {
  //       if (lineFollowerRightState == S1_IN_S2_IN)
  //       {
  //         searchForBlackLine = true;
  //         foundItem = false;
  //         resetSpeed();
  //       }
  //       else
  //       {
  //         resetSpeed();
  //         moveSideways(_speed);
  //       }
  //     }
  //   }
  //   else if (ultraSensorForward.distanceCm() < 8)
  //   {
  //     resetSpeed();
  //     moveForward(-_speed);
  //     delay(300);
  //     spinAround(-_speed);
  //     delay(2600);
  //     // execute grap logic
  //   }
  //   else
  //   {
  //     if (foundItem == false)
  //     {
  //       moveForward(_speed);
  //     }
  //   }
  // }
}
