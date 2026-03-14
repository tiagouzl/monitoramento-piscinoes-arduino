#include "ConfigStorage.h"
#include <EEPROM.h>

bool ConfigStorage::load(SystemConfig& config) {
  uint32_t magic = 0;
  EEPROM.get(kEepromAddress, magic);
  if (magic != kMagic) {
    return false;
  }

  EEPROM.get(kEepromAddress + sizeof(magic), config);
  // Garante valores válidos para campos que podem ter alterações de estrutura.
  if (config.sdLoggingEnabled != 0 && config.sdLoggingEnabled != 1) {
    config.sdLoggingEnabled = false;
  }
  return true;
}

void ConfigStorage::save(const SystemConfig& config) {
  uint32_t magic = kMagic;
  EEPROM.put(kEepromAddress, magic);
  EEPROM.put(kEepromAddress + sizeof(magic), config);

#if defined(ESP8266) || defined(ESP32)
  EEPROM.commit();
#endif
}
