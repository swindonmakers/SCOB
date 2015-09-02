
// Motion patterns - joint angles relative to center
const byte walkForwardKeyframes[4][NUM_JOINTS] = {
  {-25,-23,-23,-25},
  {25,-23,-23,25},
  {25,23,23,25},
  {-25,23,23,-25}
};
const unsigned long walkForwardDurations[4] = {700,700,700,700};
ANIMATION walkForward {
    4, (byte *) &walkForwardKeyframes[0][0], (unsigned long*)&walkForwardDurations
};

const byte turnLeftKeyframes[8][NUM_JOINTS] = {
  {0, -23, -23, 0},
  {30, -23, -23, 0},
  {30, 23, 23, 0},
  {30, 23, 23, 25},
  {0, 23, 23, 25},
  {0, -23, -23, 25},
  {0, -23, -23, 0},
  {0, 0, 0, 0}
};
const unsigned long turnLeftDurations[8] = {400,400,400,400,400,400,400,400};
ANIMATION turnLeft {
    8, (byte *) &turnLeftKeyframes[0][0], (unsigned long*)&turnLeftDurations
};

const byte turnRightKeyframes[8][NUM_JOINTS] = {
  {0, -23, -23, 0},
  {0, -23, -23, 30},
  {0, 23, 23, 30},
  {25, 23, 23, 30},
  {25, 23, 23, 0},
  {25, -23, -23, 0},
  {0, -23, -23, 0},
  {0, 0, 0, 0}
};
const unsigned long turnRightDurations[8] = {400,400,400,400,400,400,400,400};
ANIMATION turnRight {
    8, (byte *) &turnRightKeyframes[0][0], (unsigned long*)&turnRightDurations
};
