//This is the Walking Forwards Sub Routine.

int StepDelay = 300; // small delay inbetween step moves, adjust to slow down walk  
int AngleDelay = 10; //small dealy to slow down servo speed

int LAP1=0;
int RAP1=0;
int LHP1=0;
int RHP1=0;

void WalkForwards1()
{
  

Move1:  
LAP1 = LAP1+1; 
RAP1 = RAP1+1;
servoLeftAnkle.write(LeftAnkleCenter-LAP1);
servoRightAnkle.write(RightAnkleCenter-RAP1);
delay (AngleDelay);
if (LAP1<23){goto Move1;}//Left Ankle goto 23deg
if (RAP1<23){goto Move1;}//Right Ankle goto 23deg

delay (StepDelay); 
}

void WalkForwards2()
{

Move2:
LHP1=LHP1+1;
RHP1=RHP1+1;
servoLeftHip.write(LeftHipCenter+LHP1);
servoRightHip.write(RightHipCenter+RHP1);
delay (AngleDelay);
if (LHP1<25){goto Move2;}//Left Hip goto 25deg
if (RHP1<25){goto Move2;}//Right Hipe goto 25deg
 
delay (StepDelay);  
}

void WalkForwards3()
{
  

Move3:  
LAP1 = LAP1-1; 
RAP1 = RAP1-1;
servoLeftAnkle.write(LeftAnkleCenter-LAP1);
servoRightAnkle.write(RightAnkleCenter-RAP1);
delay (AngleDelay);

Serial.print("LAP1: ");
Serial.print(LAP1);
Serial.println("Degrees");

Serial.print("RAP1: ");
Serial.print(RAP1);
Serial.println("Degrees");

if (LAP1>-23){goto Move3;}//Left Ankle goto 23degrees
if (RAP1>-23){goto Move3;}//Right Ankle goto 23degrees

delay (StepDelay); 
}
void WalkForwards4()
{
Move4:
LHP1=LHP1-1;
RHP1=RHP1-1;
servoLeftHip.write(LeftHipCenter+LHP1);
servoRightHip.write(RightHipCenter+RHP1);
delay (AngleDelay);

Serial.print("LHP1: ");
Serial.print(LHP1);
Serial.println("Degrees");

Serial.print("RHP1: ");
Serial.print(RHP1);
Serial.println("Degrees");
if (LHP1>-25){goto Move4;}//Left Hip goto 25deg
if (RHP1>-25){goto Move4;}//Right Hipe goto 25deg
 
delay (StepDelay);  

}


