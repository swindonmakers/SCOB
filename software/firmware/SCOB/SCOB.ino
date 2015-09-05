#include <EEPROM.h>
#include <Servo.h>
#include <NewPing.h>
#include "ServoAnimator.h"
#include "Config.h"
#include "Animations.h"
#include <CommandQueue.h>

// Objects
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance.
ServoAnimator anim(NUM_JOINTS);
CommandQueue cmdQ(COMMAND_QUEUE_LENGTH);

// mode
uint8_t mode = MODE_INTERACTIVE;
boolean enableRandom = true; // true to enable random wanders, false to disable
long lastCommand;

String cmd;  // cmd received over serial - builds up char at a time

COMMAND randomCmd;

void setup() {
  Serial.begin(9600);
  Serial.println("SCOB");

  // load servo centers from EEPROM
  loadConfig();

  // init servos
  for (uint8_t i=0; i<NUM_JOINTS; i++) {
      anim.initServo(i, servoPins[i], servoCenters[i]);
  }

  //anim.moveServosTo((const byte*) walkForwardKeyframes, 1000);
  anim.setAnimation(stand);
  lastCommand = millis();
}


void loop() {
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
                }

                // reset the command buffer
                cmd = "";
            }
        } else {
            cmd += c;  // append the character onto the command buffer
        }
        lastCommand = millis();
        mode = MODE_INTERACTIVE;
    }

  if (enableRandom && millis() - lastCommand > 5000) {
    mode = MODE_RANDOM;
    enableRandom = false;  // disable after first random walk
  }

  // keep animation going
  anim.update();

  // is current movement complete?
  if (!anim.isBusy()) {
      // work out what to do next
      switch(mode){
          case MODE_INTERACTIVE:
            if (!cmdQ.isEmpty()) doCommand(cmdQ.dequeue());
            break;

          case MODE_RANDOM:
            if (cmdQ.isEmpty()) {
              int r = random(-1, 7);
              if (r == -1) {
                // rest for a little while
                // TODO: replace with wait command
                lastCommand = millis();
                randomCmd.cmdType = CMD_ST;
                mode = MODE_INTERACTIVE;
              } else {
                randomCmd.cmdType = r;
              }
              doCommand(&randomCmd);
            }
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
    if (c.startsWith("FD")) {
        cmdType = CMD_FD;
    } else if (c.startsWith("BK")) {
        cmdType = CMD_BK;
    } else if (c.startsWith("RT")) {
        cmdType = CMD_RT;
    } else if (c.startsWith("LT")) {
        cmdType = CMD_LT;
    } else if (c.startsWith("ST")) {
        cmdType = CMD_ST;
    } else if (c.startsWith("PG")) {
        cmdType = CMD_PG;
    } else if (c.startsWith("POS")) {
        cmdType = CMD_POS;
    } else if (c.startsWith("FT")) {
        cmdType = CMD_FT;
    } else if (c.startsWith("TL")) {
        cmdType = CMD_TL;
    } else if (c.startsWith("SP")) {
        cmdType = CMD_SP;
    } else if (c.startsWith("SV")) {
        cmdType = CMD_SV;
    } else if (c.startsWith("SC")) {
        cmdType = CMD_SC;
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

    // Handle each command type
    switch(c->cmdType) {
        case CMD_FD:
            anim.setAnimation(walkForward);
            anim.setRepeatCount(f1);
            break;
        case CMD_BK:
            anim.setAnimation(walkForward, true);
            anim.setRepeatCount(f1);
            break;
        case CMD_LT:
            anim.setAnimation(turnLeft);
            anim.setRepeatCount(f1);
            break;
        case CMD_RT:
            anim.setAnimation(turnRight);
            anim.setRepeatCount(f1);
            break;
        case CMD_ST:
            anim.setAnimation(stand);
            break;
        case CMD_PG:
            Serial.print(sonar.ping_cm());
            Serial.println("cm");
            break;

        case CMD_POS:
            if (f1 < 0 || f1 > NUM_JOINTS-1) break;
            interactiveKeyFrames[0][(uint8_t)f1] = (byte)f2;
            updateInteractivePositions();
            break;
        case CMD_FT:
            anim.setAnimation(footTap);
            anim.setRepeatCount(f1);
            break;
        case CMD_TL:
            anim.setAnimation(standTall);
            anim.setRepeatCount(f1);
            break;
        case CMD_SP:
            anim.setSpeed(f1);
            break;
        case CMD_SV:
            saveConfig();
            break;
        case CMD_SC:
            if (f1 < 0 || f1 > NUM_JOINTS-1) break;
            servoCenters[(uint8_t)f1] = (uint8_t)f2;
            anim.setServoCenter((uint8_t)f1, (uint8_t)f2);
            anim.setAnimation(stand);
            break;
    }
}

void updateInteractivePositions() {
    anim.stop();
    anim.setAnimation(interactive);

    // debug
    for (uint8_t i=0; i<NUM_JOINTS; i++) {
        if (i>0) Serial.print(',');
        Serial.print(interactiveKeyFrames[0][i]);
    }
    Serial.println();
}

void loadConfig() {
    // read first byte, see if it equals our "magic" value
    // if not, then we've never saved values to EEPROM, so just use defaults
    uint8_t m = EEPROM.read(EEPROM_MAGIC_ADDR);
    if (m == EEPROM_MAGIC) {
        for (uint8_t i=0; i<NUM_JOINTS; i++) {
            servoCenters[i] = EEPROM.read(EEPROM_CENTERS_ADDR + i);
        }
    }
}

void saveConfig() {
    // update magic
    EEPROM.update(EEPROM_MAGIC_ADDR, EEPROM_MAGIC);

    // update centers
    for (uint8_t i=0; i<NUM_JOINTS; i++) {
        EEPROM.update(EEPROM_CENTERS_ADDR + i, servoCenters[i]);
    }

    Serial.println("SAVED");
}
