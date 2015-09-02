#include <Servo.h>
#include <NewPing.h>
#include "ServoAnimator.h"
#include "Config.h"
#include "Animations.h"

// Objects
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance.
ServoAnimator anim(NUM_JOINTS);

// Animation state - used to switch between different animations
uint8_t state = 0;


void setup() {
  Serial.begin(9600);
  Serial.println("SCOB");

  // init servos
  for (uint8_t i=0; i<NUM_JOINTS; i++) {
      anim.initServo(i, servoPins[i], servoCenters[i]);
  }

  //anim.moveServosTo((const byte*) walkForwardKeyframes, 1000);
  anim.setAnimation(&walkForward);
}


void loop() {
  anim.update();

  // repeat the animation
  if (!anim.isBusy()) {
      switch(state) {
          case 0:
          anim.setAnimation(&turnLeft);
          anim.setRepeatCount(2); // turn left total of 3 times
          break;

          case 1:
          anim.setAnimation(&walkForward);
          anim.setRepeatCount(4);
          break;

          case 2:
          anim.setAnimation(&turnRight);
          anim.setRepeatCount(1);   // turn right twice
          break;

          case 3:
          anim.setAnimation(&walkForward);
          anim.setRepeatCount(4);
          break;
      }
      state++;
  }
}
