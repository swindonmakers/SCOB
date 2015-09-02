#include <Servo.h>
#include <NewPing.h>
#define SERVOANIMATOR_NUM_SERVOS 4
#include "ServoAnimator.h"

// Pins
#define TRIGGER_PIN  10
#define ECHO_PIN     11
#define LHIP_PIN 5
#define LANKLE_PIN 4
#define RANKLE_PIN 3
#define RHIP_PIN 2

// Other defines
#define MAX_DISTANCE 100 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.
#define NUM_JOINTS SERVOANIMATOR_NUM_SERVOS

const byte rest[NUM_JOINTS] = { 0, 0, 0, 0 };

// Motion patterns - joint angles relative to center
const byte walkForwardKeyframes[4][NUM_JOINTS] = {
  {-25,-23,-23,-25},
  {25,-23,-23,25},
  {25,23,23,25},
  {-25,23,23,-25}
};
const unsigned long walkForwardDurations[4] = {700,700,700,700};
ANIMATION walkForward {
    4, (const byte *) &walkForwardKeyframes, (unsigned long*)&walkForwardDurations
};

const byte turnLeft[8][NUM_JOINTS] = {
  {0, -23, -23, 0},
  {30, -23, -23, 0},
  {30, 23, 23, 0},
  {30, 23, 23, 25},
  {0, 23, 23, 25},
  {0, -23, -23, 25},
  {0, -23, -23, 0},
  {0, 0, 0, 0}
};
const unsigned long turnLeftDur[8] = {400,400,400,400,400,400,400,400};

// Objects
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance.

ServoAnimator anim();

// pin references
uint8_t servoPins[NUM_JOINTS] = {LHIP_PIN, LANKLE_PIN, RANKLE_PIN, RHIP_PIN};

// centers - calibrate these!  TODO: move to EEPROM with serial re-program
uint8_t servoCenters[NUM_JOINTS] = {97, 92, 102, 75};


void setup() {
  Serial.begin(9600);
  Serial.println("SCOB");

  // init servos
  for (uint8_t i=0; i<NUM_JOINTS; i++) {
      anim.initServo(i, servoPins[i], servoCenters[i]);
  }

  anim.moveServosTo((const byte*) rest, 1000);
  //anim.setAnimation(&walkForward);
}

void loop() {
  anim.update();

  // queue moves
  if (!anim.isBusy()) {
    //anim.moveToFrame(0);
  }
}
