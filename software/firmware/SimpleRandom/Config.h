
// Number of servo joints
#define NUM_JOINTS 4

// Pins
#define TRIGGER_PIN  10
#define ECHO_PIN     11
#define LHIP_PIN 5
#define LANKLE_PIN 4
#define RANKLE_PIN 3
#define RHIP_PIN 2

// pin references
uint8_t servoPins[NUM_JOINTS] = {LHIP_PIN, LANKLE_PIN, RANKLE_PIN, RHIP_PIN};

// default centers - will be overwritten from EEPROM once calibrated
uint8_t servoCenters[NUM_JOINTS] = {90,90,90,90};


// Commands
#define MAX_ANIM_CMD 8

#define CMD_FD 0  // forward
#define CMD_BK 1  // backward
#define CMD_LT 2  // turn left
#define CMD_RT 3  // turn right
#define CMD_ST 4  // stop
#define CMD_FT 5  // tap foot
#define CMD_TT 6  // tip toes (balerina)
#define CMD_LL 7  // look left
#define CMD_LR 8  // look right

