#include <Servo.h>
#include <NewPing.h>
#include "ServoAnimator.h"
#include "Config.h"
#include "Animations.h"
#include <CommandQueue.h>

// Objects
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance.
ServoAnimator anim(NUM_JOINTS);
CommandQueue cmdQ(15);

// mode
uint8_t mode = MODE_INTERACTIVE;

String cmd;  // cmd received over serial - builds up char at a time

void setup() {
  Serial.begin(9600);
  Serial.println("SCOB");

  // init servos
  for (uint8_t i=0; i<NUM_JOINTS; i++) {
      anim.initServo(i, servoPins[i], servoCenters[i]);
  }

  //anim.moveServosTo((const byte*) walkForwardKeyframes, 1000);
  anim.setAnimation(&stand);
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
      }
  }
}

static void parseCommand(String c) {
    // parse and queue/insert
    uint8_t cmdType = 0xff;

    // check for urgent commands
    boolean doNow = false;
    if (cmd[0] == '!') {
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
            anim.setAnimation(&walkForward);
            anim.setRepeatCount(f1);
            break;
        case CMD_BK:
            anim.setAnimation(&walkForward);
            anim.setRepeatCount(f1);
            break;
        case CMD_LT:
            anim.setAnimation(&turnLeft);
            anim.setRepeatCount(f1);
            break;
        case CMD_RT:
            anim.setAnimation(&turnRight);
            anim.setRepeatCount(f1);
            break;
        case CMD_ST:
            anim.stop();
            anim.setAnimation(&stand);
            break;
        case CMD_PG:
            Serial.print(sonar.ping_cm());
            Serial.println("cm");
            break;
    }
}
