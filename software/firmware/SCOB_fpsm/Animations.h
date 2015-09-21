
// Motion patterns - joint angles relative to center
const byte standKeyFrames[1][NUM_JOINTS] = {
  {0, 0, 0, 0}
};
const unsigned long standDurations[1] = {1000};
ANIMATION stand {
  "ST", CMD_ST, 1, (byte *) &standKeyFrames, (unsigned long*)&standDurations
};

const byte walkForwardKeyframes[4][NUM_JOINTS] = {
  {-25,-15,-15,-25},
  {25,-15,-15,25},
  {25,15,15,25},
  {-25,15,15,-25}
};
const unsigned long walkForwardDurations[4] = {700,700,700,700};
ANIMATION walkForward {
  "FD", CMD_FD, 4, (byte *) &walkForwardKeyframes, (unsigned long*)&walkForwardDurations
};

const byte turnLeftKeyframes[8][NUM_JOINTS] = {
  {0, -15, -15, 0},
  {30, -15, -15, 0},
  {30, 15, 15, 0},
  {30, 15, 15, 25},
  {0, 15, 15, 25},
  {0, -15, -15, 25},
  {0, -15, -15, 0},
  {0, 0, 0, 0}
};
const unsigned long turnLeftDurations[8] = {400,400,400,400,400,400,400,400};
ANIMATION turnLeft {
  "LT", CMD_LT, 8, (byte *) &turnLeftKeyframes, (unsigned long*)&turnLeftDurations
};

const byte turnRightKeyframes[8][NUM_JOINTS] = {
  {0, -15, -15, 0},
  {0, -15, -15, 30},
  {0, 15, 15, 30},
  {25, 15, 15, 30},
  {25, 15, 15, 0},
  {25, -15, -15, 0},
  {0, -15, -15, 0},
  {0, 0, 0, 0}
};
const unsigned long turnRightDurations[8] = {400,400,400,400,400,400,400,400};
ANIMATION turnRight {
  "RT", CMD_RT, 8, (byte *) &turnRightKeyframes, (unsigned long*)&turnRightDurations
};

const byte footTapKeyframes[2][NUM_JOINTS] = {
  {0, -25, 5, 0},
  {0 ,0 ,0, 0}
};
const unsigned long footTapDurations[2] = {400, 250};
ANIMATION footTap {
  "FT", CMD_FT, 2, (byte *) &footTapKeyframes, (unsigned long*)&footTapDurations
};

ANIMATION tipToes {
  "TT",
  CMD_TT,
  1,
  (byte *)new byte[2][NUM_JOINTS]
  {
    {0, 60, -60, 0},
    {0, 60, -60, 0}
  },
  new unsigned long[2]{500, 2000}
};

// Interactive state - used when adjusting joint parameters via serial interface
byte interactiveKeyFrames[1][NUM_JOINTS] = {
  {0, 0, 0, 0}
};
unsigned long interactiveDurations[1] = {1000};
ANIMATION interactive {
  "", 0xff, 1, (byte *) &interactiveKeyFrames, (unsigned long*)&interactiveDurations
};

ANIMATION lookLeft {
  "LL",
  CMD_LL,
  5,
  (byte *)new byte[5][NUM_JOINTS] {
    {0,0,0,0},
    {0,-20,-20,0},
    {35,-20,-20,0},
    {0,-20,-20,0},
    {0,0,0,0}
  },
  new unsigned long[5]{600,600,1000,600,600}
};


ANIMATION lookRight {
  "LR",
  CMD_LR,
  5,
  (byte *)new byte[5][NUM_JOINTS] {
    {0,0,0,0},
    {0,20,20,0},
    {0,20,20,-35},
    {0,20,20,0},
    {0,0,0,0}
  },
  new unsigned long[5]{600,600,1000,600,600}
};

/*
ANIMATION sideShuffle {
  "SS",
  CMD_SS,
  2,
  (byte *)new byte[2][NUM_JOINTS] {
    {0,20,20,0},
    {0,-30,-30,0}
  },
  new unsigned long[2]{200,200}
};
*/

/*

ANIMATION wiggleIt {
  8,
  (byte *)new byte[8][NUM_JOINTS] {
    {30,0,0,30},
    {-31,0,0,-30},
    {30,0,0,30},
    {-31,0,0,-30},
    {30,0,0,30},
    {-31,0,0,-30},
    {30,0,0,30},
    {-31,0,0,-30}
  },
  new unsigned long[8]{200,200,200,200,200,200,200,200}
};

*/

ANIMATION anims[MAX_ANIM_CMD] = { stand, walkForward, turnLeft, turnRight, footTap, tipToes, lookLeft, lookRight };
