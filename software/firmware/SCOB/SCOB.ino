#include <Servo.h>
#include <NewPing.h>

// Pins
#define TRIGGER_PIN  10 
#define ECHO_PIN     11  
#define LHIP_PIN 5
#define LANKLE_PIN 4
#define RANKLE_PIN 3 
#define RHIP_PIN 2

// Other defines
#define MAX_DISTANCE 100 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.
#define NUM_JOINTS 4

// Motion patterns - joint angles relative to center
static const byte walkForward[4][NUM_JOINTS] = {
  {-25,-23,-23,-25},
  {25,-23,-23,25},
  {25,23,23,25},
  {-25,23,23,-25}
};

static const byte turnLeft[8][NUM_JOINTS] = {
  {0, -23, -23, 0},
  {30, -23, -23, 0},
  {30, 23, 23, 0},
  {30, 23, 23, 25},
  {0, 23, 23, 25},
  {0, -23, -23, 25},
  {0, -23, -23, 0},
  {0, 0, 0, 0}
};

// Objects
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance.

// pin references
uint8_t servoPins[NUM_JOINTS] = {LHIP_PIN, LANKLE_PIN, RANKLE_PIN, RHIP_PIN};

// centers - calibrate these!  TODO: move to EEPROM with serial re-program
uint8_t servoCenters[NUM_JOINTS] = {97, 92, 102, 75};

// Manage Servo objects as array
Servo servos[NUM_JOINTS];

// current joint positions
uint8_t servoPos[NUM_JOINTS];

// start of tween joint positions
uint8_t servoStartPos[NUM_JOINTS];

// target joint positions
uint8_t servoTargetPos[NUM_JOINTS];

// how long to take in ms to get to targetPos from startPos
unsigned long moveDuration = 250;

// millis() value at the start of the move
unsigned long moveStartedAt = 0;

// move status flag - true when moving, cleared by updateServos()
boolean busyMoving = false; 

int patternNum = 0;

void setup() {
  Serial.begin(9600);
  Serial.println("SCOB");
  
  initServos();
  //moveServosTo(walkForward[0], 1000);
  moveServosTo(turnLeft[0], 1000);
}

void loop() {
  updateServos();

  // queue moves
  if (!busyMoving) {
    patternNum++;
    if (patternNum > 7) patternNum = 0;
    //moveServosTo(walkForward[patternNum], 700);
    moveServosTo(turnLeft[patternNum], 400);
  }
}

void initServos() {
  for (uint8_t i=0; i<NUM_JOINTS; i++) {
    servos[i].attach(servoPins[i]);
    servoPos[i] = servoCenters[i];
    servoStartPos[i] = servoPos[i];
    servoTargetPos[i] = servoPos[i];
    servos[i].write(servoPos[i]);
  }
}

// angles are relative to center positions!
void moveServosTo(const byte *target, unsigned long dur) {
   if (busyMoving) return;

   moveStartedAt = millis();
   busyMoving = true;
   moveDuration = dur;
   for (uint8_t i=0; i<NUM_JOINTS; i++)
       servoTargetPos[i] = servoCenters[i] + target[i];
}

void updateServos() {
  if (!busyMoving || moveDuration == 0) return; // nothing to do

  // calc elapsed time
  float t = millis() - moveStartedAt;
  if (t < 0) // overflow
    t += 4294967295 - moveStartedAt;

  t = t / moveDuration;  // t now in range 0..1
  if (t>=1) t = 1;

  // apply easing - smooth in/out
  t = bezierAt(t, 0, 0.6, 0.4, 1);
  if (t>1) t = 1;
  if (t<0) t = 0;

  for (uint8_t i=0; i<NUM_JOINTS; i++) {
    // update joint positions
    servoPos[i] = servoStartPos[i] + t*(servoTargetPos[i] - servoStartPos[i]);

    // update servos
    servos[i].write(servoPos[i]);
  }

  if (t>=1) {
    busyMoving = false;
    for (uint8_t i=0; i<NUM_JOINTS; i++) 
      servoStartPos[i] = servoPos[i];
  }
}


// Bezier routines
float Bez4(float t) { return t*t*t; }  
float Bez3(float t) { return 3*t*t*(1-t); }  
float Bez2(float t) { return 3*t*(1-t)*(1-t); }  
float Bez1(float t) { return (1-t)*(1-t)*(1-t); }  
float bezierAt(float t, float c1,float c2,float c3,float c4) {  
  return c1*Bez1(t) + c2*Bez2(t) + c3*Bez3(t) + c4*Bez4(t);  
}

