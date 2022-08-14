/*PCA9685...........Uno/Nano
  GND...............GND
  OE................N/A
  SCL...............A5
  SDA...............A4
  VCC...............5V
*/

#include "HCPCA9685.h"
#define  I2CAdd 0x40
HCPCA9685 HCPCA9685(I2CAdd);
int home_joint4 = 250;

unsigned long MOVING_TIME = 3000; // moving time is 3 seconds
unsigned long moveStartTime;
//int startAngle = 10; // 30°
//int stopAngle  = 170; // 90°

void setup() {

  HCPCA9685.Init(SERVO_MODE);
  HCPCA9685.Sleep(false);

  HCPCA9685.Servo(4, home_joint4);

  Serial.begin(9600);
  moveStartTime = millis(); // start moving

}


void loop() {

  unsigned long progress = millis() - moveStartTime;
  if (progress <= MOVING_TIME) {
    long angle = map(progress, 0, MOVING_TIME, home_joint4, home_joint4 + 150);
    Serial.println(angle);
    HCPCA9685.Servo(4, angle);
  }
//    else {HCPCA9685.Servo(4, home_joint4);
    }
//    delay(1000);

    /*
      delay(1000);
      standhome();
      delay(1500);
      slow1();
    */
  

void slow1() {
  unsigned long progress = millis() - moveStartTime;
  if (progress <= MOVING_TIME) {
    long angle = map(progress, 0, MOVING_TIME, home_joint4, home_joint4 + 150);
    HCPCA9685.Servo(4, angle); //Move to 0
    Serial.println(angle);

  }
}
void test1() {
  HCPCA9685.Servo(4, home_joint4 + 80); //Move to 0
}

void standhome() {
  HCPCA9685.Servo(4, home_joint4);
}
