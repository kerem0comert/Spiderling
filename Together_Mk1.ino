#include <VarSpeedServo.h>
#include <FlexiTimer2.h>
#define I2CAdd 0x40 // Define the I2C address of PCA9685
#include "HCPCA9685.h" //Call PCA9685 library
HCPCA9685 HCPCA9685(I2CAdd);

const unsigned char SERVO_COUNT = 8;
VarSpeedServo servos[SERVO_COUNT];
const int SERVO_MIN_ANGLE = 0;
const int SERVO_MAX_ANGLE = 180;
const int HCPCA_MIN_ANGLE = 0;
const int HCPCA_MAX_ANGLE = 480;

void setup()
{
  Serial.begin(9600);

  HCPCA9685.Init(SERVO_MODE);
  HCPCA9685.Sleep(false);

  initDefaultPositions();

  FlexiTimer2::set(70, readValue);
  FlexiTimer2::start();
}

void initDefaultPositions(){
  int defaultAngles[8] = {150, 40, 140, 350, 250, 40, 160, 350};
  for(int i=0; i < SERVO_COUNT;i++){
    writeToHPCPA(i, defaultAngles[i]);
  }
}

void writeToHPCPA(unsigned char servoId, int angle){
  HCPCA9685.Servo(servoId + 1, angle);
}

void readValue(void)
{
  sei();
  for(int i=0;i<SERVO_COUNT;i++){
    writeToHPCPA(map(valueToMap, SERVO_MIN_ANGLE, SERVO_MAX_ANGLE, HCPCA_MIN_ANGLE, HCPCA_MAX_ANGLE));
  }
}

void loop()
{
  servos[0].write(100, 30, false);
  servos[1].write(70, 50, false);
  servos[2].write(100, 30, true);

  servos[0].write(150, 50, false);
  servos[1].write(10, 50, false);
  servos[2].write(170, 90, true);
}
