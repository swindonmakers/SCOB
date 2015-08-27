void Stop()
{
servoLeftAnkle.write(LeftAnkleCenter);
servoRightAnkle.write(RightAnkleCenter);  
servoLeftHip.write(LeftHipCenter);
servoRightHip.write(RightHipCenter); 
}  
