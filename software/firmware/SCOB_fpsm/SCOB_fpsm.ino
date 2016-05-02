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
#include <CommandQueue.h>

#include "Config.h"
#include "Animations.h"
#include "Sequences.h"

ServoAnimator anim(NUM_JOINTS);
HC_SR04 sonar(TRIGGER_PIN, ECHO_PIN, ECHO_INT);
unsigned long nextSonarPing = 0;
#define SONAR_DELAY 500

CRGB leds[NUM_LEDS];   // The pixels
CRGB (&refleds)[NUM_LEDS] = leds; // refleds is a reference to the pixels
StartUpSequence seqStart(refleds);
ScannerSequence seqScanner(refleds);
RetreatSequence seqRetreat(refleds);
AdvanceSequence seqAdvance(refleds);
unsigned long sequenceTimer;

CommandQueue cmdQ(COMMAND_QUEUE_LENGTH);
String cmd;  // cmd received over serial - builds up char at a time

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
  if (nextSonarPing < millis()) {
    //Serial.println(state.range);
    sonar.start();
    nextSonarPing = millis() + SONAR_DELAY;
  } else if (sonar.isFinished()) {
    state.range = (3 * state.range + sonar.getRange()) / 4;
  }
  
  // Parse Logo commands from Serial
  if (Serial.available()) {
    char c = toupper(Serial.read());
    if (c == '\r' || c == '\n') {  // if found a line end
      if (cmd != "") {  // check the command isn't blank
        if (cmdQ.isFull()) {
          Serial.println("BUSY");
        } else {
          parseCommand(cmd);
          Serial.println("OK:" + cmd);
          do_commandMode();
        }
        
        // reset the command buffer
        cmd = "";
      }
    } else {
      cmd += c;  // append the character onto the command buffer
    }
  }
  
  // continue with current activity
  state.activity();
}

static void parseCommand(String c) {
    // parse and queue/insert
    uint8_t cmdType = 0xff;

    // check for urgent commands
    boolean doNow = false;
    if (c[0] == '!') {
        doNow = true;
        c = c.substring(1);
    }

    // parse the command type
    for (int i = 0; i < sizeof(anims) / sizeof(anims[0]); i++)
      if (c.startsWith(anims[i].cmd))
        cmdType = anims[i].cmdType;

    if (cmdType != 0xff) {
      // already matches an animation
    } else if (c.startsWith("BK")) {
        cmdType = CMD_BK;
    }

    // give up if command not recognised
    if (cmdType == 0xff) return;

    // lose the command name, keep the parameters
    int sp = c.indexOf(' ');
    if (sp > -1) {
        c = c.substring(sp+1);
    } else {
        c = "";
    }

    // insert/queue the command
    if (doNow) {
        anim.stop();  // stop the bot
        cmdQ.insert(c, cmdType);  // insert the new command at the head of the command queue
    } else {
        cmdQ.enqueue(c, cmdType);
    }
}

static void doCommand(COMMAND *c)
{
    if (c == NULL) return;

    // Parse out parameter values
    int sp = c->cmd.indexOf(' ');
    float f1 = 0;
    float f2 = 0;
    if (sp > -1) {
        f1 = c->cmd.substring(0,sp).toFloat();
        f2 = c->cmd.substring(sp+1).toFloat();
    } else {
        f1 = c->cmd.toFloat();
    }

    // Handle the animation commands that can be "auto" processed
    if (c->cmdType <= MAX_ANIM_CMD) {
      for (int i = 0; i < sizeof(anims) / sizeof(anims[0]); i++) {
        if (anims[i].cmdType == c->cmdType) {
          anim.setAnimation(anims[i]);
          anim.setRepeatCount(f1);
          return;
        }
      }
    }
    
    // Handle all the other commands that can't be dealt with by the "auto" process
    switch(c->cmdType) {
        case CMD_BK:
            anim.setAnimation(walkForward, true);
            anim.setRepeatCount(f1);
            break;
    }
}

void do_commandMode()
{
  if (state.activity != activity_commandMode) {
    anim.stop();
    state.activity = activity_commandMode;
    state.mouthAnim = &seqScanner;
    state.mouthAnim->init();
    FastLED.show();
  }
}

void activity_commandMode() {
  static unsigned long lastCommandEnd = 0;
  
  // Do commands
  if (!anim.isBusy()) {
    if (lastCommandEnd == 0) lastCommandEnd = millis();
    
    if (!cmdQ.isEmpty()) {
      // next command
      doCommand(cmdQ.dequeue());
      lastCommandEnd = 0;
    }
    else if (millis() - lastCommandEnd > 10000) {
      // exit command mode after 10s of no more commands
      do_explore();
    }
  }
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
