//This is the Walking Backwards Sub Routine.


int StepDelay1 = 200; // small delay inbetween step moves, adjust to slow down walk  
int AngleDelay1 = 10; //small dealy to slow down servo speed  
int Count=0;


void WalkBackwards()
{
  
LAP=0;
RAP=0;
LHP=0;
RHP=0;
Loop1:
Count=Count+1;

Move1:  
LAP = LAP+1; 
RAP = RAP+1;
servoLeftAnkle.write(LeftAnkleCenter-LAP);
servoRightAnkle.write(RightAnkleCenter-RAP);
delay (AngleDelay1);
if (LAP<20){goto Move1;}//Left Ankle goto 23deg
if (RAP<20){goto Move1;}//Right Ankle goto 23deg

delay (StepDelay1); 

Move2:
LHP=LHP+1;
RHP=RHP+1;
servoLeftHip.write(LeftHipCenter-LHP);
servoRightHip.write(RightHipCenter-RHP);
delay (AngleDelay1);
if (LHP<25){goto Move2;}//Left Hip goto 25deg
if (RHP<25){goto Move2;}//Right Hipe goto 25deg
 
delay (StepDelay1);  

Move3:  
LAP = LAP-1; 
RAP = RAP-1;
servoLeftAnkle.write(LeftAnkleCenter-LAP);
servoRightAnkle.write(RightAnkleCenter-RAP);
delay (AngleDelay1);
if (LAP>-20){goto Move3;}//Left Ankle goto 23degrees
if (RAP>-20){goto Move3;}//Right Ankle goto 23degrees

delay (StepDelay1); 

Move4:
LHP=LHP-1;
RHP=RHP-1;
servoLeftHip.write(LeftHipCenter-LHP);
servoRightHip.write(RightHipCenter-RHP);
delay (AngleDelay1);
if (LHP>-25){goto Move4;}//Left Hip goto 25deg
if (RHP>-25){goto Move4;}//Right Hipe goto 25deg
 
delay (StepDelay1);  
if (Count<3){goto Loop1;}
}


