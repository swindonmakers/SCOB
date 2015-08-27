#include <Servo.h>
#include <NewPing.h>
#include <Serial.h>


#define TRIGGER_PIN  7  //
#define ECHO_PIN     8  // Arduino pin tied to echo pin on the ultrasonic sensor - change as needed - these are just what I have setup on a Baby Orangutan
#define MAX_DISTANCE 200 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance.


Servo servoLeftHip;
Servo servoRightHip;
Servo servoLeftAnkle;
Servo servoRightAnkle;

int LeftHipCenter = 80; //Enter actual servo alingment values here
int RightHipCenter = 95;
int LeftAnkleCenter = 80;
int RightAnkleCenter = 90;



void setup()
{
  servoLeftHip.attach(9);  // LH attaches the servo to digital pin 9
  servoRightHip.attach(10);  // RH attaches the servo to digital pin 10
  servoLeftAnkle.attach(11);  // LA attaches the servo to digital pin 11
  servoRightAnkle.attach(12);  // RA attaches the servo to digital pin 12

   
  servoLeftHip.write(LeftHipCenter);  //LH tells the servo to go to the center position  
  servoRightHip.write(RightHipCenter);  //RH tells the servo to go to the center position  
  servoLeftAnkle.write(LeftAnkleCenter);  //LA tells the servo to go to the center position  
  servoRightAnkle.write(RightAnkleCenter);  //RA tells the servo to go to the center position  

  
 Serial.begin(115200); 
 
delay (5000); //10 sec delay to place Bob on the floor before starting to walk

}

void loop()
{

Ping();
WalkForwards1();
Ping();
WalkForwards2();
Ping();
WalkForwards3();
Ping();
WalkForwards4();
}



