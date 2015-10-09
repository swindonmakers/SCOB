/*
  This is an attempt write a "function pointer state machine" based on the article
  here : http://hackaday.com/2015/09/04/embed-with-elliot-practical-state-machines/
  
  The state machine works perfectly well and is quite elegant if you hold SCOB off
  the floor and move your hand backwards and forwards in front of the sonar sensor.
  However because the sonar senor moves all over the place as SCOB moves around 
  on the floow the distance readins are all over the shop and so the states don't
  really work very well...
*/

#include <EEPROM.h>
#include <Servo.h>
#include <HC_SR04.h>
#include <FastLED.h>

#include <SimpleBezier.h>
#include <ServoAnimator.h>
#include <ScobEeprom.h>

#include "Config.h"
#include "Animations.h"
#include "Sequences.h"

ServoAnimator anim(NUM_JOINTS);
HC_SR04 sonar(TRIGGER_PIN, ECHO_PIN, ECHO_INT);

CRGB leds[NUM_LEDS];   // The pixels
CRGB (&refleds)[NUM_LEDS] = leds; // refleds is a reference to the pixels
StartUpSequence seqStart(refleds);
ScannerSequence seqScanner(refleds);
RetreatSequence seqRetreat(refleds);
AdvanceSequence seqAdvance(refleds);
unsigned long sequenceTimer;

struct ScobState
{
  unsigned int range;
  void (*activity)(void);
  Sequence *mouthAnim;
};

ScobState state;

void setup() {
  Serial.begin(9600);
  Serial.println("SCOB - Function Pointer State Machine");
  
  // load servo centers from EEPROM
  ScobEeprom::loadConfig(NUM_JOINTS, servoCenters);

  // init servos
  for (uint8_t i=0; i<NUM_JOINTS; i++) {
      anim.initServo(i, servoPins[i], servoCenters[i]);
  }
  
  // init sonar
  sonar.begin();
  sonar.start();
  
  // init state
  state.range = 100;
  state.activity = activity_explore;
  state.mouthAnim = &seqScanner;
  
  // inital pose
  anim.setAnimation(stand);
  
  // Setup Neopixel library
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
  FastLED.clear();
  FastLED.show();

  // Show startup LEDS
  seqStart.runSync();
  // Start main mouth animation
  state.mouthAnim->init();
  FastLED.show();
}

void loop() {
  // keep animations running
  anim.update();
  
  // run LED animations
  if (millis() - sequenceTimer > state.mouthAnim->delayMs) {
    state.mouthAnim->next();
    FastLED.show();
    sequenceTimer = millis();
  }
  
  // keep sonar state updated
  if (sonar.isFinished()) {
    state.range = (3 * state.range + sonar.getRange()) / 4;
    //Serial.println(state.range);
    sonar.start();
  }
  
  // continue with current activity
  state.activity();
}

#define DIST_TOO_CLOSE        10
#define DIST_CLOSE_ENOUGH     12
#define DIST_INTERESTING      20
#define DIST_NOTHING_TO_SEE   40

void do_explore()
{
  anim.stop();
  state.activity = activity_explore;
  state.mouthAnim = &seqScanner;
  state.mouthAnim->init();
  FastLED.show();
}

// Randomly move around until we detect something
void activity_explore()
{
  // Make some moves
  if (!anim.isBusy()) {
    int r = random(-10, MAX_ANIM_CMD);
    Serial.print("Randomness:");
    anim.setSpeed(1);
    if (r < -5) {
      anim.setAnimation(walkForward);
      Serial.println("FD");
    } else if (r < 0) {
      anim.setAnimation(walkForward, true);
      Serial.println("BK");
    } else {
      anim.setAnimation(anims[r]);
      Serial.println(anims[r].cmd);
    }
  }
  
  // Check state and possibly take action
  if (state.range > DIST_INTERESTING && state.range < DIST_NOTHING_TO_SEE) {
    Serial.println("That looks interesting...");
    do_advance();
  } else if (state.range < DIST_TOO_CLOSE) {
    Serial.println("Whoa, too close!");
    do_retreat();
  }
}

void do_advance()
{
  anim.stop();
  state.activity = activity_advance;
  state.mouthAnim = &seqAdvance;
  state.mouthAnim->init();
  FastLED.show();
}

void activity_advance()
{
  static int movesMade;
  
  // Make some moves
  if (!anim.isBusy()) {
    anim.setSpeed(1);
    anim.setAnimation(walkForward);
    movesMade++;
  }
  
  // Check state and possibly take action
  if (movesMade > 1) {
    if (state.range > DIST_TOO_CLOSE && state.range < DIST_CLOSE_ENOUGH) {
      Serial.println("Found you!");
      movesMade = 0;
      do_explore();
    } else if (state.range > DIST_NOTHING_TO_SEE) {
      Serial.println("Where'd you go?");
      movesMade = 0;
      do_explore();
    }
  } 
  if (state.range < DIST_TOO_CLOSE) {
    Serial.println("Whoa, too close!");
    movesMade = 0;
    do_retreat();
  }
}

void do_retreat()
{
  state.activity = activity_retreat;
  anim.stop();
  state.mouthAnim = &seqRetreat;
  state.mouthAnim->init();
  FastLED.show();
}

void activity_retreat()
{
  static int movesMade;
  
  // Make some moves
  if (!anim.isBusy()) {
    anim.setSpeed(2);
    anim.setAnimation(walkForward, true); // reverse!
    movesMade++;
  }
  
  // Check state and possible take action
  if (movesMade > 4) {
    if (state.range > DIST_NOTHING_TO_SEE) {
      Serial.println("Safe now");
      movesMade = 0;
      do_explore();
    }
  }
}
