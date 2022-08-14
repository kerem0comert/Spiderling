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


#define  I2CAdd 0x40     //Define the I2C address of PCA9685
//#include <Servo.h>       //Call servo Library
#include "HCPCA9685.h"   //Call PCA9685 library 
HCPCA9685 HCPCA9685(I2CAdd);

// Define home positions of servos

int home_joint2 = 250;
int home_joint3 = 40;
int home_joint4 = 140;
int home_joint5 = 350;
int home_joint6 = 250;
int home_joint7 = 40;
int home_joint8 = 160;
int home_joint9 = 350;

bool isJoint4Moving = false;

//Future head servo/distance sensor

#define neck_survoPin 10
#define trigPin 13
#define echoPin 12


void setup() {

  /* Initialise the library and set it to 'servo mode' */
  HCPCA9685.Init(SERVO_MODE);

  /* Wake the device up */
  HCPCA9685.Sleep(false);



  //Future servo/distance sensor
  /*
    neck_servo.attach(neck_survoPin, 500, 2400);
    neck_servo.write(90);

    pinMode(echoPin,INPUT);
    pinMode(trigPin,OUTPUT);
  */

  //Brings servos to the home position for start


  HCPCA9685.Servo(2, home_joint2);
  HCPCA9685.Servo(3, home_joint3);
  HCPCA9685.Servo(4, home_joint4);
  HCPCA9685.Servo(5, home_joint5);
  HCPCA9685.Servo(6, home_joint6);
  HCPCA9685.Servo(7, home_joint7);
  HCPCA9685.Servo(8, home_joint8);
  HCPCA9685.Servo(9, home_joint9);

}

//Real stuff goes on here

void loop() {

  delay(2000);

  //  PCATest();
  //  standhome();
  forward3(5);
  // forward(5);
  //forward2(3);
  //wave(1);
  //delay(2000);
  standhome();
  
  
}

//Idle function

void idle() {
  delay(100);
}


//Default stand position

void standhome() {


  HCPCA9685.Servo(2, home_joint2);
  HCPCA9685.Servo(3, home_joint3);
  HCPCA9685.Servo(4, home_joint4);
  HCPCA9685.Servo(5, home_joint5);
  HCPCA9685.Servo(6, home_joint6);
  HCPCA9685.Servo(7, home_joint7);
  HCPCA9685.Servo(8, home_joint8);
  HCPCA9685.Servo(9, home_joint9);

}

//Walk forward function

void forward(unsigned int step) {
  while (step-- > 0) {
    HCPCA9685.Servo(3, home_joint3 + 50);
    HCPCA9685.Servo(7, home_joint7 + 50);
    delay(100);
    HCPCA9685.Servo(2, home_joint2 + 60);
    HCPCA9685.Servo(8, home_joint8 - 60);
    HCPCA9685.Servo(4, home_joint4);
    HCPCA9685.Servo(6, home_joint6);
    delay(100);
    HCPCA9685.Servo(3, home_joint3);
    HCPCA9685.Servo(7, home_joint7);
    idle();

    HCPCA9685.Servo(5, home_joint5 - 50);
    HCPCA9685.Servo(9, home_joint9 - 50);
    delay(100);
    HCPCA9685.Servo(2, home_joint2);
    HCPCA9685.Servo(8, home_joint8);
    HCPCA9685.Servo(4, home_joint4 - 60);
    HCPCA9685.Servo(6, home_joint6 + 60);
    delay(100);
    HCPCA9685.Servo(5, home_joint5);
    HCPCA9685.Servo(9, home_joint9);
    idle();
  }


}

void PCATest() {

  HCPCA9685.Servo(5, 30);
  delay(100);
  HCPCA9685.Servo(5, 60);
  delay(100);
  HCPCA9685.Servo(5, 90);
  delay(100);
  HCPCA9685.Servo(5, 120);
  delay(100);
}


void wave(unsigned int step) {
  while (step-- > 0) {
    HCPCA9685.Servo(5, home_joint5 - 300); //Levitate Foot1
    delay(1000);
    HCPCA9685.Servo(4, home_joint4 - 90); //Move forward Foot3
    delay(350);
    HCPCA9685.Servo(4, home_joint4 - 30); //Move forward Foot1
    delay(350);
    HCPCA9685.Servo(4, home_joint4 - 90); //Move forward Foot3
    delay(350);
    HCPCA9685.Servo(4, home_joint4 - 30); //Move forward Foot1
    delay(350);
    HCPCA9685.Servo(4, home_joint4 - 90); //Move forward Foot3
    delay(350);
    HCPCA9685.Servo(4, home_joint4 - 30); //Move forward Foot1
    delay(350);
  }
}


void forward2(unsigned int step) {
  while (step-- > 0) {

    HCPCA9685.Servo(5, home_joint5 - 50); //Levitate Foot1
    HCPCA9685.Servo(9, home_joint9 - 50); //Levitate Foot3
    delay(100);

    HCPCA9685.Servo(2, home_joint2 - 50); //Move backward Foot2
    HCPCA9685.Servo(6, home_joint6 + 50); //Move backward Foot4
    HCPCA9685.Servo(4, home_joint4 - 60); //Move forward Foot1
    HCPCA9685.Servo(8, home_joint8 + 60); //Move forward Foot3
    delay(100);

    HCPCA9685.Servo(5, home_joint5 + 50); //Land Foot1
    HCPCA9685.Servo(9, home_joint9 + 50); //Land Foot3
    delay(100);

    HCPCA9685.Servo(3, home_joint3 + 60); //Levitate Foot2
    HCPCA9685.Servo(7, home_joint7 + 60); //Levitate Foot4
    delay(100);

    HCPCA9685.Servo(2, home_joint2 + 50); //Move backward Foot2
    HCPCA9685.Servo(6, home_joint6 - 50); //Move backward Foot4
    HCPCA9685.Servo(4, home_joint4 + 60); //Move forward Foot1
    HCPCA9685.Servo(8, home_joint8 - 60); //Move forward Foot3
    delay(100);

    HCPCA9685.Servo(3, home_joint3 - 60); //Land Foot2
    HCPCA9685.Servo(7, home_joint7 - 60); //Land Foot4
    delay(100);

  }
}

void forward3(unsigned int step) {
  while (step-- > 0) {

    HCPCA9685.Servo(5, home_joint5 - 50); //Levitate Foot1
    HCPCA9685.Servo(9, home_joint9 - 50); //Levitate Foot3
    delay(100);

    HCPCA9685.Servo(2, home_joint2 - 50); //Move backward Foot2
    HCPCA9685.Servo(6, home_joint6 + 50); //Move backward Foot4
    HCPCA9685.Servo(4, home_joint4 - 60); //Move forward Foot1
    HCPCA9685.Servo(8, home_joint8 + 60); //Move forward Foot3
    delay(100);

    HCPCA9685.Servo(5, home_joint5 + 50); //Land Foot1
    HCPCA9685.Servo(9, home_joint9 + 50); //Land Foot3
    delay(100);

    HCPCA9685.Servo(3, home_joint3 + 60); //Levitate Foot2
    HCPCA9685.Servo(7, home_joint7 + 60); //Levitate Foot4
    delay(100);

    HCPCA9685.Servo(2, home_joint2 + 50); //Move backward Foot2
    HCPCA9685.Servo(6, home_joint6 - 50); //Move backward Foot4
    HCPCA9685.Servo(4, home_joint4 + 60); //Move forward Foot1
    HCPCA9685.Servo(8, home_joint8 - 60); //Move forward Foot3
    delay(100);

    HCPCA9685.Servo(3, home_joint3 - 60); //Land Foot2
    HCPCA9685.Servo(7, home_joint7 - 60); //Land Foot4
    delay(100);

  }
}

void forward4(){
    while (step-- > 0) {

    HCPCA9685.Servo(5, home_joint5 - 50); //Levitate Foot1
    HCPCA9685.Servo(9, home_joint9 - 50); //Levitate Foot3
    delay(100);

    HCPCA9685.Servo(2, home_joint2 - 50); //Move backward Foot2
    HCPCA9685.Servo(6, home_joint6 + 50); //Move backward Foot4
    HCPCA9685.Servo(4, home_joint4 - 60); //Move forward Foot1
    HCPCA9685.Servo(8, home_joint8 + 60); //Move forward Foot3
    delay(100);

    HCPCA9685.Servo(5, home_joint5 + 50); //Land Foot1
    HCPCA9685.Servo(9, home_joint9 + 50); //Land Foot3
    delay(100);

    HCPCA9685.Servo(3, home_joint3 + 60); //Levitate Foot2
    HCPCA9685.Servo(7, home_joint7 + 60); //Levitate Foot4
    delay(100);

    HCPCA9685.Servo(2, home_joint2 + 50); //Move backward Foot2
    HCPCA9685.Servo(6, home_joint6 - 50); //Move backward Foot4
    HCPCA9685.Servo(4, home_joint4 + 60); //Move forward Foot1
    HCPCA9685.Servo(8, home_joint8 - 60); //Move forward Foot3
    delay(100);

    HCPCA9685.Servo(3, home_joint3 - 60); //Land Foot2
    HCPCA9685.Servo(7, home_joint7 - 60); //Land Foot4
    delay(100);

  }

  moveWithTime(home_joint3, 200, millis())
}

void moveWithTime(int jointId, int targetAngle, unsigned long startTime, unsigned long duration){
 unsigned long progress = millis() - moveStartTime;
  if (progress <= duration) {
    long angle = map(progress, 0, duration, jointId, jointId + targetAngle);
    Serial.println(angle);
    HCPCA9685.Servo(4, angle);
  }
}


