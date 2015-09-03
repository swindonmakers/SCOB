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


// pin references
uint8_t servoPins[NUM_JOINTS] = {LHIP_PIN, LANKLE_PIN, RANKLE_PIN, RHIP_PIN};

// centers - calibrate these!  TODO: move to EEPROM with serial re-program
uint8_t servoCenters[NUM_JOINTS] = {97, 92, 102, 75};


// Commands
#define CMD_FD 0
#define CMD_BK 1
#define CMD_LT 2
#define CMD_RT 3
#define CMD_ST 4
#define CMD_PG 5  // ping - report sonar distance


// Modes
#define MODE_INTERACTIVE 0
