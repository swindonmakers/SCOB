

int StepDelay2 = 200; // small delay inbetween step moves, adjust to slow down walk  
int AngleDelay2 = 15; //small dealy to slow down servo speed  

int LAP=0;
int RAP=0;
int LHP=0;
int RHP=0;
int TLLoop=0;
void TurnLeft()
{
  
LAP=0;  
RAP=0;
LHP=0;
RHP=0;
TLLoop=0;

Loop:
Move1:  
LAP = LAP+1; 
RAP = RAP+1;
servoLeftAnkle.write(LeftAnkleCenter-LAP);
servoRightAnkle.write(RightAnkleCenter-RAP);
delay (AngleDelay2);
Serial.print("LAP: ");
Serial.print(LAP);
Serial.println("Degrees");

Serial.print("RAP: ");
Serial.print(RAP);
Serial.println("Degrees");
if (LAP<23){goto Move1;}//Left Ankle goto 23deg
if (RAP<23){goto Move1;}//Right Ankle goto 23deg

delay (StepDelay2); 

Move2:
LHP=LHP+1;
servoLeftHip.write(LeftHipCenter+LHP);
delay (AngleDelay2);
Serial.print("LHP: ");
Serial.print(RHP);
Serial.println("Degrees");
if (LHP<30){goto Move2;}
 
delay (StepDelay2);  

Move3:  
LAP = LAP-1; 
RAP = RAP-1;
servoLeftAnkle.write(LeftAnkleCenter-LAP);
servoRightAnkle.write(RightAnkleCenter-RAP);
delay (AngleDelay2);
Serial.print("LAP: ");
Serial.print(LAP);
Serial.println("Degrees");

Serial.print("RAP: ");
Serial.print(RAP);
Serial.println("Degrees");

if (LAP>-23){goto Move3;}//Left Ankle goto 23degrees
if (RAP>-23){goto Move3;}//Right Ankle goto 23degrees

delay (StepDelay2);

Move4:
RHP=RHP-1;
servoRightHip.write(RightHipCenter-RHP);
delay (AngleDelay2);
if (RHP>-25){goto Move4;}
Serial.print("RHP: ");
Serial.print(RHP);
Serial.println("Degrees"); 
delay (StepDelay2);  


Move5:
LHP=LHP-1;
servoLeftHip.write(LeftHipCenter+LHP);
delay (AngleDelay2);
Serial.print("LHP: ");
Serial.print(LHP);
Serial.println("Degrees");
if (LHP>0){goto Move5;}


Move6:  
LAP = LAP+1; 
RAP = RAP+1;
servoLeftAnkle.write(LeftAnkleCenter-LAP);
servoRightAnkle.write(RightAnkleCenter-RAP);
delay (AngleDelay2);
Serial.print("LAP: ");
Serial.print(LAP);
Serial.println("Degrees");

Serial.print("RAP: ");
Serial.print(RAP);
Serial.println("Degrees");

if (LAP<+23){goto Move6;}//Left Ankle goto 23degrees
if (RAP<+23){goto Move6;}//Right Ankle goto 23degrees

delay (StepDelay2); 

Move7:
RHP=RHP+1;
servoRightHip.write(RightHipCenter-RHP);
delay (AngleDelay2);
if (RHP<0){goto Move7;}//Left Hip goto -25deg
Serial.print("LHP: ");
Serial.print(LHP);
Serial.println("Degrees"); 
delay (StepDelay2);  

Move8:  
LAP = LAP-1; 
RAP = RAP-1;
servoLeftAnkle.write(LeftAnkleCenter-LAP);
servoRightAnkle.write(RightAnkleCenter-RAP);
delay (AngleDelay2);
Serial.print("LAP: ");
Serial.print(LAP);
Serial.println("Degrees");

Serial.print("RAP: ");
Serial.print(RAP);
Serial.println("Degrees");

if (LAP>0){goto Move8;}//Left Ankle goto 23degrees
if (RAP>0){goto Move8;}//Right Ankle goto 23degrees
  delay (StepDelay2); 
  TLLoop=TLLoop+1;
if (TLLoop<3){goto Loop;}
  
}
