//  --------                 --------
// |  D9    |               |  D7    |
// | joint9 |               | joint7 |
//  ----- --------     --------- -----
//       |  Foot 3 |  |  Foot4 |
//       |  D8     |  |  D6    |
//       | joint10 |  | joint6 |
//        --------     --------
//       |  Foot2 |  |  Foot1  |
//       |  D2    |  |   D4    |
//       | joint2 |  |  joint4 |
//  ----- --------    --------- -----
// |  D3    |               |  D5    |
// | joint3 |               | joint5 |
//  --------                 --------
//                Front

#define I2CAdd 0x40 // Define the I2C address of PCA9685
//#include <Servo.h>       //Call servo Library
#include "HCPCA9685.h" //Call PCA9685 library
#include <FlexiTimer2.h>
HCPCA9685 HCPCA9685(I2CAdd);

// Unsigned char is used rather than unsigned int for saving memory.

typedef struct Vector3{
  unsigned int x;
  unsigned int y;
  unsigned int z;
}Vector3;

typedef struct Joint
{
  bool isMoving;
  unsigned long moveTimeSoFar
  unsigned long targetTime;
  unsigned int currAngle;
  unsigned int targetAngle
} Joint;

enum MoveSpeeds {
    TURN = 4,
    LEG_MOVE = 8,
    BODY_MOVE = 3,
    STAND_SEAT = 1,
};

const unsigned char JOINT_COUNT = 8;
Joint joints[JOINT_COUNT];


void setup()
{
  Serial.begin(9600);
  // Initialize joints
  int i;
  for (i = 0; i < JOINT_COUNT; i++)
  {
    joints[i].moveTimeSoFar = 0;
    joints[i].targetAngle = 0;
    joints[i].targetTime = 0;
    joints[i].isMoving = false;
  }
  joints[0].currAngle = 250;
  joints[1].currAngle = 40;
  joints[2].currAngle = 140;
  joints[3].currAngle = 350;
  joints[4].currAngle = 250;
  joints[5].currAngle = 40;
  joints[6].currAngle = 160;
  joints[7].currAngle = 350;

  FlexiTimer2::set(20, checkMovements);
  FlexiTimer2::start();

  // Here, specify which move to start, e.g.:
  startTimedMovement(4, 150, 3000);
  delay(500);
  startTimedMovement(4, 220, 3000);
}

void startTimedMovement(unsigned char jointId, unsigned int targetAngle, unsigned long targetTime)
{
  joints[jointId].targetTime = targetTime;
  joints[jointId].targetAngle = targetAngle;
  joints[jointId].isMoving = true;
  joints[jointId].moveTimeSoFar = 0;
}

void loop()
{
}

void test1()
{
  startTimedMovement(4, 150, 3000);
  startTimedMovement(4, 220, 3000);
}

void checkMovements(void)
{
  sei();
  unsigned long currentTime = millis();
  int i;
  for (i = 0; i < JOINT_COUNT; i++)
  {
    if (joints[i].isMoving)
    {
      // Target time has been reached
      if (joints[i].moveTimeSoFar > joints[i].targetTime)
      {
        joints[i].targetAngle = 0;
        joints[i].targetTime = 0;
        joints[i].isMoving = false;
      }
      else
      { // Still need to move more
        unsigned long angle = map(
            joints[i].moveTimeSoFar, // Current progress
            0,                       // Start time
            joints[i].targetTime,    // End time
            joints[i].currAngle,
            joints[i].targetAngle);
        joints[i].moveTimeSoFar = currentTime;
        Serial.println("Servo " + String(i) + " moves to angle " + String(angle) + ". MoveTimeSoFar " + String(joints[i].moveTimeSoFar));
        HCPCA9685.Servo(i, angle);
      }
    }
  }
  Serial.println("It took function " + String(millis() - currentTime) + " seconds to complete");
}
