#include <Servo.h>
#include <NewPing.h>


#define TRIGGER_PIN  10  //
#define ECHO_PIN     11  // Arduino pin tied to echo pin on the ultrasonic sensor - change as needed - these are just what I have setup on a Baby Orangutan
#define MAX_DISTANCE 100 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance.


Servo servoLeftHip;
Servo servoRightHip;
Servo servoLeftAnkle;
Servo servoRightAnkle;

int LeftHipCenter = 97; //Enter actual servo alingment values here
int RightHipCenter = 75;
int LeftAnkleCenter = 92;
int RightAnkleCenter = 102;

void setup()
{
  servoLeftHip.attach(5);  
  servoLeftAnkle.attach(4);  
  servoRightAnkle.attach(3);  
  servoRightHip.attach(2);  

   
  servoLeftHip.write(LeftHipCenter);  //LH tells the servo to go to the center position  
  servoRightHip.write(RightHipCenter);  //RH tells the servo to go to the center position  
  servoLeftAnkle.write(LeftAnkleCenter);  //LA tells the servo to go to the center position  
  servoRightAnkle.write(RightAnkleCenter);  //RA tells the servo to go to the center position  

  
 Serial.begin(9600); 
 
delay (5000); // delay to place Bob on the floor before starting to walk

}

void loop()
{
  WalkForwards1();
  WalkForwards2();
  WalkForwards3();
  WalkForwards4();  
/*
Ping();
WalkForwards1();
Ping();
WalkForwards2();
Ping();
WalkForwards3();
Ping();
WalkForwards4();
*/
}



