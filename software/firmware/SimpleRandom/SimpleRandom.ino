/*

  This sketch demonstrates how to make SCOB randomly walk around
  using a minimal amount of code.

*/

#include <EEPROM.h>
#include <Servo.h>

#include <SimpleBezier.h>
#include <ServoAnimator.h>
#include <ScobEeprom.h>

#include "Config.h"
#include "Animations.h"

ServoAnimator anim(NUM_JOINTS);

void setup() {
  Serial.begin(9600);
  Serial.println("SCOB - Simple Random");
  
  // load servo centers from EEPROM
  ScobEeprom::loadConfig(NUM_JOINTS, servoCenters);

  // init servos
  for (uint8_t i=0; i<NUM_JOINTS; i++) {
      anim.initServo(i, servoPins[i], servoCenters[i]);
  }
  
  // set inital animation
  anim.setAnimation(stand);
}

void loop() {
  // let the animation run
  anim.update();
  
  if (!anim.isBusy()) {
    // current animation is complete, pick another one to run with next
    int r = random(-10, MAX_ANIM_CMD);
    Serial.println(r);
    
    // Make forwards and backwards more likely by saying
    // between -10 and -5 means forwards
    // between -5 and 0 means backwards
    if (r < -5) {
      anim.setAnimation(walkForward);
    } else if (r < 0) {
      anim.setAnimation(walkForward, true);
    } else {
      // else run whatever animation was randomly picked
      anim.setAnimation(anims[r]);
    }
  }
}
