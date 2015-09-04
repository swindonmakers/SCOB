// Number of servo joints
#define NUM_JOINTS 4

// Pins
#define TRIGGER_PIN  10
#define ECHO_PIN     11
#define LHIP_PIN 5
#define LANKLE_PIN 4
#define RANKLE_PIN 3
#define RHIP_PIN 2

// Other defines
#define MAX_DISTANCE 100 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.
#define COMMAND_QUEUE_LENGTH 15
#define ENABLE_RANDOM 1 // 1 to enable random wanders, zero to disable

// pin references
uint8_t servoPins[NUM_JOINTS] = {LHIP_PIN, LANKLE_PIN, RANKLE_PIN, RHIP_PIN};

// centers - calibrate these!  TODO: move to EEPROM with serial re-program
uint8_t servoCenters[NUM_JOINTS] = {97, 92, 102, 75};


// Commands
#define CMD_FD 0  // forward
#define CMD_BK 1  // backward
#define CMD_LT 2  // turn left
#define CMD_RT 3  // turn right
#define CMD_ST 4  // stop
#define CMD_FT 5  // tap foot
#define CMD_TL 6  // stand tall (balerina)

#define CMD_PG 20  // ping - report sonar distance
#define CMD_POS 21 // set position of servo x to y (relative to center)


// Modes
#define MODE_INTERACTIVE 0
#define MODE_RANDOM 1
