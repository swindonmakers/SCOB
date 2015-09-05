
// Motion patterns - joint angles relative to center
const byte standKeyFrames[1][NUM_JOINTS] = {
  {0, 0, 0, 0}
};
const unsigned long standDurations[1] = {1000};
ANIMATION stand {
  1, (byte *) &standKeyFrames, (unsigned long*)&standDurations
};

const byte walkForwardKeyframes[4][NUM_JOINTS] = {
  {-25,-15,-15,-25},
  {25,-15,-15,25},
  {25,15,15,25},
  {-25,15,15,-25}
};
const unsigned long walkForwardDurations[4] = {700,700,700,700};
ANIMATION walkForward {
    4, (byte *) &walkForwardKeyframes, (unsigned long*)&walkForwardDurations
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
    8, (byte *) &turnLeftKeyframes, (unsigned long*)&turnLeftDurations
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
    8, (byte *) &turnRightKeyframes, (unsigned long*)&turnRightDurations
};

const byte footTapKeyframes[2][NUM_JOINTS] = {
  {0, -25, 5, 0},
  {0 ,0 ,0, 0}
};
const unsigned long footTapDurations[2] = {400, 250};
ANIMATION footTap {
  2, (byte *) &footTapKeyframes, (unsigned long*)&footTapDurations
};

ANIMATION standTall {
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
  1, (byte *) &interactiveKeyFrames, (unsigned long*)&interactiveDurations
};
