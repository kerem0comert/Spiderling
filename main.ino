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

/*
    Unsigned char is used rather than unsigned int for saving memory.
    RELEVANT: Yes, interrupts occur, and are processed, during delay functions.
    When the ISR finishes, and returns, the code that was executing resumes executing
    where it left off. If a delay was in process, the delay will be resumed.
    The delay will end at the same time it would have if the interrupt had not occurred
*/
typedef struct Vector3
{
  unsigned int x;
  unsigned int y;
  unsigned int z;
} Vector3;

typedef struct Joint
{
  volatile bool isMoving;
  unsigned long moveTimeSoFar;
  unsigned long targetTime;
  volatile unsigned int currAngle;
  unsigned int targetAngle;
  unsigned char diff;
} Joint;
/*
  enum MoveSpeeds {
  TURN = 4,
  LEG_MOVE = 8,
  BODY_MOVE = 3,
  STAND_SEAT = 1,
  };
*/
const unsigned char JOINT_COUNT = 8;
Joint joints[JOINT_COUNT];
const unsigned int SERVO_MOVEMENT_TIME = 1000;
const unsigned char SERVO_MOVEMENT_THERSHOLD = 10;


void setup()
{
  HCPCA9685.Init(SERVO_MODE);
  HCPCA9685.Sleep(false);
  Serial.begin(9600);
  // Initialize joints
  //int defaultAngles[8] = {150, 40, 140, 350, 250, 40, 160, 350};
  int i;
  for (i = 0; i < JOINT_COUNT; i++)
  {
    joints[i].targetAngle = 0;
    joints[i].targetTime = 0;
    joints[i].isMoving = false;
    joints[i].moveTimeSoFar = 0;
    joints[i].currAngle = 0; // defaultAngles[i];
    joints[i].diff = 100;
  }

  FlexiTimer2::set(100, checkMovements);
  FlexiTimer2::start();

  // Here, specify which move to start, e.g.:
  /*
    startTimedMovement(1, 350, 10000);
    startTimedMovement(2, 350, 3000);
    startTimedMovement(3, 100, 5000);
    startTimedMovement(4, 100, 7000);
    startTimedMovement(5, 350, 100000);
  */
  forward();

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

void waitAllToReach()
{
  bool allReached = false;
  while (!allReached)
  {
    allReached = true;
    for (int i = 0; i < JOINT_COUNT; i++)
    {
      if (joints[i].isMoving)
      {
        // Serial.println("Servo " + String(i) + " is still moving."); 
        allReached = false;
        break;
      }
    }
  }
  noInterrupts();
  Serial.println("All reached."); 

  delay(50);
  interrupts();
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
      if ((joints[i].moveTimeSoFar > joints[i].targetTime) || 
          (joints[i].diff <= 3))
          
      {
        joints[i].targetAngle = 0;
        joints[i].targetTime = 0;
        joints[i].isMoving = false;
      }
      else
      { // Still need to move more
        joints[i].currAngle = map(
                                joints[i].moveTimeSoFar, // Current progress
                                0,                       // Start time
                                joints[i].targetTime,    // End time
                                joints[i].currAngle,
                                joints[i].targetAngle);
        joints[i].moveTimeSoFar = currentTime;
        Serial.println("Servo " + String(i) + " moves to angle " + String(joints[i].currAngle) + ". MoveTimeSoFar " + String(joints[i].moveTimeSoFar));
        // delay(50);
        HCPCA9685.Servo(i + 1, joints[i].currAngle);
        joints[i].diff = abs(joints[i].currAngle - joints[i].targetAngle);
        Serial.println(String(i) + "d" + String(joints[i].diff));
      }
    }
    
  }
  //Serial.println("It took function " + String(millis() - currentTime) + " seconds to complete");
}

void forward()
{
  startTimedMovement(0, joints[0].currAngle + 100, SERVO_MOVEMENT_TIME);
  startTimedMovement(1, joints[1].currAngle + 200, SERVO_MOVEMENT_TIME);
  startTimedMovement(2, joints[2].currAngle + 300, SERVO_MOVEMENT_TIME);
  startTimedMovement(3, joints[3].currAngle + 480, SERVO_MOVEMENT_TIME);

  waitAllToReach();

  startTimedMovement(0, 0, SERVO_MOVEMENT_TIME);
  startTimedMovement(1, 0, SERVO_MOVEMENT_TIME);
  startTimedMovement(2, 0, SERVO_MOVEMENT_TIME);
  startTimedMovement(3, 0, SERVO_MOVEMENT_TIME);
}

/*
  startTimedMovement(3, joints[3].currAngle - 200, 100000);
    startTimedMovement(2, joints[2].currAngle - 200, 100000);

    delay(50);

    startTimedMovement(3, joints[3].currAngle + 200, 10000);
    startTimedMovement(2, joints[2].currAngle + 200, 10000);
    startTimedMovement(4, joints[4].currAngle - 50, SERVO_MOVEMENT_TIME);
    startTimedMovement(6, joints[6].currAngle - 50, SERVO_MOVEMENT_TIME);

    delayMicroseconds(50);

    startTimedMovement(0, joints[0].currAngle - 50, SERVO_MOVEMENT_TIME);
    startTimedMovement(4, joints[4].currAngle + 50, SERVO_MOVEMENT_TIME);
    startTimedMovement(2, joints[2].currAngle - 60, SERVO_MOVEMENT_TIME);
    startTimedMovement(6, joints[6].currAngle + 60, SERVO_MOVEMENT_TIME);

    delayMicroseconds(50);

    startTimedMovement(3, joints[3].currAngle + 50, SERVO_MOVEMENT_TIME);
    startTimedMovement(7, joints[7].currAngle + 50, SERVO_MOVEMENT_TIME);

    delayMicroseconds(50);

    startTimedMovement(1, joints[1].currAngle + 60, SERVO_MOVEMENT_TIME);
    startTimedMovement(5, joints[5].currAngle + 60, SERVO_MOVEMENT_TIME);

    delayMicroseconds(50);

    startTimedMovement(0, joints[0].currAngle + 50, SERVO_MOVEMENT_TIME);
    startTimedMovement(4, joints[4].currAngle - 50, SERVO_MOVEMENT_TIME);
    startTimedMovement(2, joints[2].currAngle + 60, SERVO_MOVEMENT_TIME);
    startTimedMovement(6, joints[6].currAngle - 60, SERVO_MOVEMENT_TIME);
    delayMicroseconds(50);

    startTimedMovement(1, joints[1].currAngle - 60, SERVO_MOVEMENT_TIME);
    startTimedMovement(5, joints[5].currAngle - 60, SERVO_MOVEMENT_TIME);
    delayMicroseconds(50);
*/

