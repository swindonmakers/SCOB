// EEPROM
#define EEPROM_MAGIC_ADDR 0  // address of magic byte
#define EEPROM_MAGIC 88
#define EEPROM_CENTERS_ADDR 1   // start address for servo centers



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

// pin references
uint8_t servoPins[NUM_JOINTS] = {LHIP_PIN, LANKLE_PIN, RANKLE_PIN, RHIP_PIN};

// default centers - will be overwritten from EEPROM once calibrated
uint8_t servoCenters[NUM_JOINTS] = {90,90,90,90};


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
#define CMD_SP 22  // set speed multiplier 1.0 is nominal
#define CMD_SV 23  // save config
#define CMD_SC 24 // servo center - set servo x center to y (abs)
#define CMD_PF 25 // pause for x seconds
#define CMD_TO 26 // move to <lhi> <lankle> <rankle> <rhi> <dur>

#define CMD_RND 100 // switch to random movement mode

// Modes
#define MODE_INTERACTIVE 0
#define MODE_RANDOM 1
