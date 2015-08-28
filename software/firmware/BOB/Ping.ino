void Ping()
{
Start:
unsigned int uS = sonar.ping(); // Send ping, get ping time in microseconds (uS).
  Serial.print("Ping: ");
  Serial.print(uS / US_ROUNDTRIP_CM); // Convert ping time to distance in cm and print result (0 = outside set distance range)
  Serial.println("cm");  
  if (uS / US_ROUNDTRIP_CM>20){goto pass1;}
  Stop();
  delay (500);
  WalkBackwards();
uS = sonar.ping();
  Serial.print("Ping: ");
  Serial.print(uS / US_ROUNDTRIP_CM); // Convert ping time to distance in cm and print result (0 = outside set distance range)
  Serial.println("cm");    
if (uS / US_ROUNDTRIP_CM<5){goto Start;}
Stop();
TurnRight (); //by about 90degrees

  uS = sonar.ping();
  Serial.print("Ping: ");
  Serial.print(uS / US_ROUNDTRIP_CM); // Convert ping time to distance in cm and print result (0 = outside set distance range)
  Serial.println("cm");  
  if (uS / US_ROUNDTRIP_CM>20){goto pass1;}
Stop();  
TurnLeft (); //By about 180degrees  

uS = sonar.ping();
  Serial.print("Ping: ");
  Serial.print(uS / US_ROUNDTRIP_CM); // Convert ping time to distance in cm and print result (0 = outside set distance range)
  Serial.println("cm");  
  if (uS / US_ROUNDTRIP_CM>20){goto pass1;}
  if (uS / US_ROUNDTRIP_CM<20){goto Start;}
pass1:;  
}
