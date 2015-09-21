#include <ScobEeprom.h>
#include <EEPROM.h>

namespace ScobEeprom
{
	void loadConfig(uint8_t n, uint8_t centers[])
	{
		// read first byte, see if it equals our "magic" value
		// if not, then we've never saved values to EEPROM, so just use defaults
		uint8_t m = EEPROM.read(EEPROM_MAGIC_ADDR);
		if (m == EEPROM_MAGIC) {
			for (uint8_t i = 0; i < n; i++) {
				centers[i] = EEPROM.read(EEPROM_CENTERS_ADDR + i);
			}
		}
	}

	void saveConfig(uint8_t n, uint8_t centers[])
	{
		// update magic
		EEPROM.update(EEPROM_MAGIC_ADDR, EEPROM_MAGIC);

		// update centers
		for (uint8_t i = 0; i < n; i++) {
			EEPROM.update(EEPROM_CENTERS_ADDR + i, centers[i]);
		}

		Serial.println("SAVED");
	}
}