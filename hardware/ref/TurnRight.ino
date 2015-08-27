//this is a 90degree turn to the right sub routine

int StepDelay3 = 200; // small delay inbetween step moves, adjust to slow down walk  
int AngleDelay3 = 15; //small dealy to slow down servo speed  

void TurnRight()
{
LAP=0;  
RAP=0;
LHP=0;
RHP=0;

Move1:  
LAP = LAP+1; 
RAP = RAP+1;
servoLeftAnkle.write(LeftAnkleCenter+LAP);
servoRightAnkle.write(RightAnkleCenter+RAP);
delay (AngleDelay3);
Serial.print("LAP: ");
Serial.print(LAP);
Serial.println("Degrees");

Serial.print("RAP: ");
Serial.print(RAP);
Serial.println("Degrees");
if (LAP<23){goto Move1;}//Left Ankle goto 23deg
if (RAP<23){goto Move1;}//Right Ankle goto 23deg

delay (StepDelay3); 

Move2:
RHP=RHP+1;
servoRightHip.write(RightHipCenter-RHP);
delay (AngleDelay3);
Serial.print("RHP: ");
Serial.print(RHP);
Serial.println("Degrees");
if (RHP<30){goto Move2;}//Right Hipe goto 25deg
 
delay (StepDelay3);  

Move3:  
LAP = LAP-1; 
RAP = RAP-1;
servoLeftAnkle.write(LeftAnkleCenter+LAP);
servoRightAnkle.write(RightAnkleCenter+RAP);
delay (AngleDelay3);
Serial.print("LAP: ");
Serial.print(LAP);
Serial.println("Degrees");

Serial.print("RAP: ");
Serial.print(RAP);
Serial.println("Degrees");

if (LAP>-23){goto Move3;}//Left Ankle goto 23degrees
if (RAP>-23){goto Move3;}//Right Ankle goto 23degrees

delay (StepDelay3); 

Move4:
LHP=LHP-1;
servoLeftHip.write(LeftHipCenter+LHP);
delay (AngleDelay3);
if (LHP>-25){goto Move4;}//Left Hip goto -25deg
Serial.print("LHP: ");
Serial.print(LHP);
Serial.println("Degrees"); 
delay (StepDelay3);  


Move5:
RHP=RHP-1;
servoRightHip.write(RightHipCenter-RHP);
delay (AngleDelay3);
Serial.print("RHP: ");
Serial.print(RHP);
Serial.println("Degrees");
if (RHP>0){goto Move5;}//Right Hipe goto 25deg


Move6:  
LAP = LAP+1; 
RAP = RAP+1;
servoLeftAnkle.write(LeftAnkleCenter+LAP);
servoRightAnkle.write(RightAnkleCenter+RAP);
delay (AngleDelay3);
Serial.print("LAP: ");
Serial.print(LAP);
Serial.println("Degrees");

Serial.print("RAP: ");
Serial.print(RAP);
Serial.println("Degrees");

if (LAP<+23){goto Move6;}//Left Ankle goto 23degrees
if (RAP<+23){goto Move6;}//Right Ankle goto 23degrees

delay (StepDelay3); 

Move7:
LHP=LHP+1;
servoLeftHip.write(LeftHipCenter+LHP);
delay (AngleDelay3);
if (LHP<0){goto Move7;}//Left Hip goto -25deg
Serial.print("LHP: ");
Serial.print(LHP);
Serial.println("Degrees"); 
delay (StepDelay3);  

Move8:  
LAP = LAP-1; 
RAP = RAP-1;
servoLeftAnkle.write(LeftAnkleCenter+LAP);
servoRightAnkle.write(RightAnkleCenter+RAP);
delay (AngleDelay3);
Serial.print("LAP: ");
Serial.print(LAP);
Serial.println("Degrees");

Serial.print("RAP: ");
Serial.print(RAP);
Serial.println("Degrees");

if (LAP>0){goto Move8;}//Left Ankle goto 23degrees
if (RAP>0){goto Move8;}//Right Ankle goto 23degrees
  delay (StepDelay3); 
}
