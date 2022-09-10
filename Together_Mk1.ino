/*
  SweepTwoServos
  By Philip van Allen <philvanallen.com> for the VarSpeedServo.h library (October 2013)
  This example code is in the public domain
  
  Sweep two servos from 0-180, 180-0 in unison
  Uses the wait feature of the 2013 version of VarSpeedServo to start the first servo moving in the background
  and immediately starting a second servo moving and waiting for the second one to finish.
  
  Note that two servos will require more power than is available from the USB port - use an external power supply!
*/

#include <VarSpeedServo.h> 
 #include <FlexiTimer2.h>
 #define  I2CAdd 0x40     //Define the I2C address of PCA9685
//#include <Servo.h>       //Call servo Library
#include "HCPCA9685.h"   //Call PCA9685 library 
HCPCA9685 HCPCA9685(I2CAdd);

VarSpeedServo myservo1;  // create servo object to control a servo 
                         // a maximum of eight servo objects can be created 
VarSpeedServo myservo2;
 
const int servoPin1 = 9; // the digital pin used for the first servo
const int servoPin2 = 10; // the digital pin used for the second servo
 
void setup() { 
  /* Initialise the library and set it to 'servo mode' */
  HCPCA9685.Init(SERVO_MODE);

  /* Wake the device up */
  HCPCA9685.Sleep(false);
  Serial.begin(9600);
  //Serial.println());
  myservo1.attach(servoPin1);  // attaches the servo on pin 9 to the servo object
  myservo1.write(0,255,false); // set the intial position of the servo, as fast as possible, run in background
  myservo2.attach(servoPin2);  // attaches the servo on pin 9 to the servo object
  myservo2.write(0,255,true);  // set the intial position of the servo, as fast as possible, wait until done
  FlexiTimer2::set(20, readValue);
  FlexiTimer2::start();
} 

void readValue(void){
  sei();
  Serial.println(myservo1.read());
  HCPCA9685.Servo(1, myservo1.read());
  HCPCA9685.Servo(2, myservo1.read());
  HCPCA9685.Servo(3, myservo1.read());
}
void loop() {
  //Serial.println(myservo1.slowmove());
  myservo1.write(180,127,true);        // move the servo to 180, fast speed, run background                                      // write(degrees 0-180, speed 1-255, wait to complete true-false)
  
  //myservo2.write(180,127,true);         // move the servo to 180, fast speed, wait until done
  
  myservo1.write(0,30,true);           // move the servo to 180, slow speed, run in background
  //myservo2.write(0,30,true);            // move the servo to 180, slow speed, wait until done*/


} 
