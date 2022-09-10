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
  for(int i=0; i < SERVO_COUNT;i++){
    servos[i].attach(9+i);
  }
  Serial.begin(9600);

  HCPCA9685.Init(SERVO_MODE);
  HCPCA9685.Sleep(false);

  initDefaultPositions();
  delay(500);
  FlexiTimer2::set(10, readValue);
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
  for(int i=0;i<3;i++){
    Serial.println(String(i) + " moved to " + servos[i].read());
    writeToHPCPA(i, map(servos[i].read(), SERVO_MIN_ANGLE, SERVO_MAX_ANGLE, HCPCA_MIN_ANGLE, HCPCA_MAX_ANGLE));
  }
}

void loop()
{
  servos[0].write(180, 10, false);
  servos[1].write(180, 10, false);
  servos[2].write(180, 10, false);
  servos[0].wait();
  servos[1].wait();
  servos[2].wait();

 
  servos[0].write(0, 10, false);
  servos[1].write(0, 10, false);
  servos[2].write(0, 10, false);

  /*servos[0].write(150, 50, false);
  servos[1].write(10, 50, false);
  servos[2].write(170, 90, true);*/
}
