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
#include <SoftwareSerial.h>
HCPCA9685 HCPCA9685(I2CAdd);
SoftwareSerial BTSerial (2,3);

const unsigned char SERVO_COUNT = 8;
VarSpeedServo servos[SERVO_COUNT];

const int SERVO_MIN_ANGLE = 0;
const int SERVO_MAX_ANGLE = 180;
const int HCPCA_MIN_ANGLE = 0;
const int HCPCA_MAX_ANGLE = 480;
const int SERVO_SPEED = 30;

// The servos are connected to consecutive pins starting from Pin 9
const unsigned int SERVO_PIN_OFFSET = 9; 
const unsigned int HCPCA_SERVO_OFFSET = 2;
int defaultAngles[8] = {90, 30, 60, 125, 90, 20, 50, 120};

void setup()
{
  for (int i = 0; i < SERVO_COUNT; i++) {
    servos[i].attach(SERVO_PIN_OFFSET + i);
  }
  HCPCA9685.Init(SERVO_MODE);
  HCPCA9685.Sleep(false);
  Serial.begin(9600);
  BTSerial.begin(9600);
  //initDefaultPositions();
  FlexiTimer2::set(1, readValue);
  FlexiTimer2::start();
}

void loop()
{
 handleBluetooth();
  defaultPos();
  delay(500);
  /*
  forward3();
  forward3();
  forward3();
  */
  
  //initDefaultPositions();
}

void writeToHPCPA(unsigned char servoId, int angle) {
  HCPCA9685.Servo(servoId + HCPCA_SERVO_OFFSET, angle);
}

void readValue(void)
{
  sei();
  for (int i = 0; i < SERVO_COUNT; i++) {
    //Serial.println(String(i) + " moved to " + servos[i].read());
    writeToHPCPA(i, map(servos[i].read(), SERVO_MIN_ANGLE, SERVO_MAX_ANGLE, HCPCA_MIN_ANGLE, HCPCA_MAX_ANGLE));
  }
}

void defaultPos() {
  //Serial.println("In forward2");
  for (int i = 0; i < SERVO_COUNT; i++) {
    servos[i].write(defaultAngles[i], SERVO_SPEED, false);
  }
  delay(1000);
}

void forward3() {
  Serial.println("In forward3");
  servos[3].write(defaultAngles[3] - 30, SERVO_SPEED, false);
  servos[7].write(defaultAngles[7] - 30, SERVO_SPEED, false);
  delay(500);
  servos[0].write(defaultAngles[0] - 20, SERVO_SPEED, false);
  servos[4].write(defaultAngles[4] + 20, SERVO_SPEED, false);
  servos[2].write(defaultAngles[2] - 20, SERVO_SPEED, false);
  servos[6].write(defaultAngles[6] + 20, SERVO_SPEED, false);
  delay(500);
  servos[3].write(defaultAngles[3] , SERVO_SPEED, false);
  servos[7].write(defaultAngles[7] , SERVO_SPEED, false);
  delay(500);
  servos[1].write(defaultAngles[1] + 30 , SERVO_SPEED, false);
  servos[5].write(defaultAngles[5] + 30 , SERVO_SPEED, false);
  delay(500);
  servos[0].write(defaultAngles[0] + 20, SERVO_SPEED, false);
  servos[4].write(defaultAngles[4] - 20, SERVO_SPEED, false);
  servos[2].write(defaultAngles[2] + 20, SERVO_SPEED, false);
  servos[6].write(defaultAngles[6] - 20, SERVO_SPEED, false);
  delay(500);
  servos[1].write(defaultAngles[1] , SERVO_SPEED, false);
  servos[5].write(defaultAngles[5] , SERVO_SPEED, false);
  delay(500);
}

void handleBluetooth(){
  if(BTSerial.available()){
    char bluetoothCommand = BTSerial.read();
    switch(bluetoothCommand){
      case 'F': forward3(); break;
      case 'X': break;
    }
    Serial.write("Bluetooth command: " + bluetoothCommand);
  }
  if(Serial.available()){
    BTSerial.write(Serial.read());
  }
}
