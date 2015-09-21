#include <EEPROM.h>
#include <Servo.h>
#include <NewPing.h>
#include <SimpleBezier.h>
#include <ServoAnimator.h>
#include "Config.h"
#include "Animations.h"
#include <CommandQueue.h>
#include <FastLED.h>
#include <ScobEeprom.h>

// Objects
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance.
ServoAnimator anim(NUM_JOINTS);
CommandQueue cmdQ(COMMAND_QUEUE_LENGTH);

// mode
uint8_t mode = MODE_INTERACTIVE;
boolean enableRandom = true; // true to enable random wanders, false to disable
boolean enableCollisionHandling = true;
unsigned long lastCommand, pauseUntil;

String cmd;  // cmd received over serial - builds up char at a time

COMMAND randomCmd;

unsigned int lastSonarReading = MAX_DISTANCE;
unsigned int avgSonarReading = MAX_DISTANCE;  // averaged over several samples
unsigned long sonarTimer;

// Fast LED Neopixel
#define NUM_LEDS 8
#define DATA_PIN 12
CRGB leds[NUM_LEDS];
int ledn = 0;
int ledi = 0;
unsigned long lastLedUpdate;

void setup() {
  Serial.begin(9600);
  Serial.println("SCOB");
  randomSeed(analogRead(0));
  
  // Setup Neopixel library
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);

  // load servo centers from EEPROM
  ScobEeprom::loadConfig(NUM_JOINTS, servoCenters);

  // init servos
  for (uint8_t i=0; i<NUM_JOINTS; i++) {
      anim.initServo(i, servoPins[i], servoCenters[i]);
  }

  anim.setAnimation(stand);
  lastCommand = millis();
  sonarTimer = lastCommand;
  pauseUntil = lastCommand;
  lastLedUpdate = lastCommand;
}

// Read sonar distance, correct for the fact that "inifite" distance is reported as zero
unsigned int sonarDist() {
  unsigned int dist;
  dist = sonar.ping_cm();
  if (dist == 0)
    dist = MAX_DISTANCE;
  return dist;  
}

void loop() {
    // Parse Logo commands from Serial
    if (Serial.available()) {
        char c = toupper(Serial.read());
        if (c == '\r' || c == '\n') {  // if found a line end
            if (cmd != "") {  // check the command isn't blank
                mode = MODE_INTERACTIVE;
                enableRandom = false; // turn off at first interactive command, turn back on with RND
                enableCollisionHandling = true;

                if (cmdQ.isFull()) {
                    Serial.println("BUSY");
                } else {
                    parseCommand(cmd);
                    Serial.println("OK:" + cmd);
                }

                // reset the command buffer
                cmd = "";
            }
        } else {
            cmd += c;  // append the character onto the command buffer
        }
        lastCommand = millis();
    }

  if (enableRandom && cmdQ.isEmpty() && millis() - lastCommand > 5000) {
    mode = MODE_WANDER;
    enableRandom = false;  // disable after first random walk
    enableCollisionHandling = true;
  }

  // keep animation going
  anim.update();

  // keep leds updating
  if (millis() - lastLedUpdate > 100) {
    lastLedUpdate = millis();
    FastLED.clear();
    leds[abs((ledn%14) - 7)] = CHSV(ledi, 255, 255);
    ledn++;
    ledi++;
    if (ledi == 256) {
      ledi = 0;
      ledn = 0;
    }
    FastLED.show();
  }

  // take sonar readings
  if (millis() > sonarTimer) {
      lastSonarReading = sonarDist();
      avgSonarReading = (3 * avgSonarReading + lastSonarReading) / 4;
      sonarTimer = millis() + SONAR_INTERVAL;
      //Serial.println(avgSonarReading);

      if (enableCollisionHandling && avgSonarReading < 2*STRIDE_LENGTH) {
        // aargh, something is in my face!!!
        Serial.println(F("Aargh, get out of my face!"));
        anim.stop();  // stop whatever we're doing
        // avoid taking another sonar reading until we've have time to start backing up
        sonarTimer += 2000;
        // start backing up
        anim.setAnimation(walkForward, true);
      }
  }

  // is current movement complete?  and we're not pausing?
  if (!anim.isBusy() && millis() > pauseUntil) {
      // work out what to do next
      switch(mode){
          case MODE_INTERACTIVE:
            if (!cmdQ.isEmpty()) doCommand(cmdQ.dequeue());
            break;

          case MODE_RANDOM:
            doRandom();
            break;

        case MODE_WANDER:
            doWander();
            break;
      }
  }
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
    } else if (c.startsWith("PG")) {
        cmdType = CMD_PG;
    } else if (c.startsWith("POS")) {
        cmdType = CMD_POS;
    } else if (c.startsWith("SP")) {
        cmdType = CMD_SP;
    } else if (c.startsWith("SV")) {
        cmdType = CMD_SV;
    } else if (c.startsWith("SC")) {
        cmdType = CMD_SC;
    } else if (c.startsWith("PF")) {
        cmdType = CMD_PF;
    } else if (c.startsWith("RND")) {
        mode = MODE_RANDOM;
    } else if (c.startsWith("TO")) {
        cmdType = CMD_TO;
    } else if (c.startsWith("WANDER")) {
        mode = MODE_WANDER;
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

    // debug what bot is up to
    /*
    Serial.print("CQ Peek: ");
    Serial.println(cmdQ.peekAtType());
    Serial.print("bot.isQFull:");
    Serial.println(bot.isQFull());
    */
}

static void doCommand(COMMAND *c)
{
    if (c == NULL) return;

    // Special case for TO because it has more than two parameters
    if (c->cmdType == CMD_TO) {
        // 5 parameters to parse, all integers
        // parse directly into interactive animation
        uint8_t pn = 0;  // param number
        uint8_t sp1 = 0; // start of next param
        uint8_t sp2;
        while(1) {
            sp2 = c->cmd.indexOf(' ', sp1); // space following next param
            if (pn == 4) {
                interactiveDurations[0] = c->cmd.substring(sp1).toInt();
                break;
            } else {
                interactiveKeyFrames[0][pn] = c->cmd.substring(sp1, sp2).toInt();
                sp1 = sp2 + 1;
                pn++;
            }
        }
        updateInteractivePositions();
        return;
    }

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
        case CMD_PG:
            Serial.print(sonarDist());
            Serial.println("cm");
            break;
        case CMD_POS:
            if (f1 < 0 || f1 > NUM_JOINTS-1) break;
            interactiveKeyFrames[0][(uint8_t)f1] = (byte)f2;
            updateInteractivePositions();
            break;
        case CMD_SP:
            anim.setSpeed(f1);
            break;
        case CMD_SV:
            ScobEeprom::saveConfig(NUM_JOINTS, servoCenters);
            break;
        case CMD_SC:
            if (f1 < 0 || f1 > NUM_JOINTS-1) break;
            if (c->cmd == "") {
              for (int i = 0; i < NUM_JOINTS; i++) {
                Serial.print(servoCenters[i]);
                Serial.print(",");
              }
              Serial.println();
            } else {
              servoCenters[(uint8_t)f1] = (uint8_t)f2;
              anim.setServoCenter((uint8_t)f1, (uint8_t)f2);
              anim.setAnimation(stand);
            }
            break;
        case CMD_PF:
            pauseUntil = millis() + (f1*1000);
            break;
    }
}

void updateInteractivePositions() {
    anim.setAnimation(interactive);

    // debug
    for (uint8_t i=0; i<NUM_JOINTS; i++) {
        if (i>0) Serial.print(',');
        Serial.print(interactiveKeyFrames[0][i]);
    }
    Serial.println();
}

void doRandom() {
  if (cmdQ.isEmpty()) {
    int r = random(-10, MAX_ANIM_CMD + 1);
    if (r == -10) {
      // rest for a little while - so we can easily tell the random is over!
      pauseUntil = millis() + 2000;
      //randomCmd.cmdType = CMD_ST;

      // starting wandering
      mode = MODE_WANDER;
    } else if (r < -5) {
      randomCmd.cmdType = CMD_FD;
    } else if (r < 0) { 
      randomCmd.cmdType = CMD_BK; 
    } else {
      randomCmd.cmdType = r;
    }
    doCommand(&randomCmd);
  }
}

void doWander() {
    /*
    Wander algorithm:
    1) lookLeft, take sonar reading
    2) lookRight, take sonar reading
    3) stand, take sonar reading
    4) work out farthest distance reading
    5) turn to face farthest distance reading (if required)
    6) walk forward x cycles - determined by sensor distance divided by stride of ~4cm
    */

    // state enum
    typedef enum {
        LOOKLEFT,
        LOOKRIGHT,
        LOOKFWD,
        TURN,
        WALK,
        BACKUP
    } WANDER_STATE;

    // internal state variables
    static WANDER_STATE wanderState = LOOKLEFT;

    static unsigned int
        leftDist = MAX_DISTANCE,
        rightDist = MAX_DISTANCE,
        fwdDist = MAX_DISTANCE;

    static uint8_t numStrides = 1;
    static uint8_t cycles = 0;

    switch(wanderState) {
        case LOOKLEFT:
            Serial.println("lookLeft");
            enableCollisionHandling = false;  // turn off during looking, turning and reversing
            // start by turning to look left
            anim.setAnimation(lookLeft);
            anim.setRepeatCount(1);
            wanderState = LOOKRIGHT;
            break;
        case LOOKRIGHT:
            Serial.println("lookRight");
            // now looking left, so take the left sonar reading
            leftDist = sonarDist();

            // and turn to look to the right
            anim.setAnimation(lookRight);
            anim.setRepeatCount(1);
            wanderState = LOOKFWD;
            break;
        case LOOKFWD:
            Serial.println("lookFwd");
            // looking right, take right sonar reading
            rightDist = sonarDist();

            // turn to look forward
            anim.setAnimation(stand);
            anim.setRepeatCount(1);
            wanderState = TURN;
            break;
        case TURN:
            Serial.println("turn");
            // looking forwards, take final sonar reading
            fwdDist = sonarDist();
            
            Serial.print('L');
            Serial.println(leftDist);
            Serial.print('F');
            Serial.println(fwdDist);
            Serial.print('R');
            Serial.println(rightDist);

            // check if waaay too close to something
            if (fwdDist < 6) {
                wanderState = BACKUP;
            } else {
                // turn left or right, if required
                if ((leftDist > rightDist && leftDist > fwdDist)) {
                    numStrides = leftDist / STRIDE_LENGTH;
                    anim.setAnimation(turnLeft);
                    anim.setRepeatCount(1);
                } else if (rightDist > leftDist && rightDist > fwdDist) {
                    numStrides = rightDist / STRIDE_LENGTH;
                    anim.setAnimation(turnRight);
                    anim.setRepeatCount(1);
                } else {
                    numStrides = fwdDist / STRIDE_LENGTH;
                }

                if (numStrides > 5) {
                  numStrides = 5;
                }

                wanderState = WALK;
            }
            break;
        case WALK:
            Serial.print("walk:");
            Serial.println(numStrides);
            enableCollisionHandling = true; // turn on for normal walking
            // now turned to face correct direction, so walk forwards
            anim.setAnimation(walkForward);
            anim.setRepeatCount(numStrides);
            wanderState = LOOKLEFT;

            // been wandering for a while and miles from anything? then lets do something random! :)
            if (cycles > 2 && numStrides == 5) {
              mode = MODE_RANDOM;
              cycles = 0;
            } else {
              cycles++;
            }
            
            break;
        case BACKUP:
            Serial.println("backup");
            // too close to something, backup a bit
            anim.setAnimation(walkForward, true);
            anim.setRepeatCount(2);
            wanderState = LOOKLEFT;
            break;
    }
}
