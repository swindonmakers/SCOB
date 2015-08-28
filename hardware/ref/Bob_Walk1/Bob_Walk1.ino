#include <NewPing.h>
#include <Servo.h>

#define TRIGGER_PIN  10  //
#define ECHO_PIN     11  // Arduino pin tied to echo pin on the ultrasonic sensor - change as needed - these are just what I have setup on a Baby Orangutan
#define MAX_DISTANCE 100 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance.

Servo servoLeftHip;
Servo servoRightHip;
Servo servoLeftAnkle;
Servo servoRightAnkle;

int LeftHipCenter = 97; //Enter actual servo alingment values here
int RightHipCenter = 87;
int LeftAnkleCenter = 92;
int RightAnkleCenter = 102;

void setup()
{ 
  servoLeftHip.attach(5);  
  servoLeftAnkle.attach(4);  
  servoRightAnkle.attach(3);  
  servoRightHip.attach(2);  

   
  servoLeftHip.write(LeftHipCenter); 
  servoRightHip.write(RightHipCenter);  
  servoLeftAnkle.write(LeftAnkleCenter);  
  servoRightAnkle.write(RightAnkleCenter); 
 
 Serial.begin(9600); 
 
delay (1000); //10 sec delay to place Bob on the floor before starting to walk

}

int ctr = 0;

void loop()
{
   ctr++;

   float a = 20 * cos((float)ctr / 10);
   a=0;
   
  servoLeftHip.write(LeftHipCenter + a); 
  servoRightHip.write(RightHipCenter + a);  
  servoLeftAnkle.write(LeftAnkleCenter + a);  
  servoRightAnkle.write(RightAnkleCenter + a); 

   delay(50);

  int uS = sonar.ping();
  Serial.print("Ping: ");
  Serial.print(uS / US_ROUNDTRIP_CM);
  Serial.println("cm");
}

