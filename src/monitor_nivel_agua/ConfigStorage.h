#ifndef MONITOR_NIVEL_AGUA_CONFIG_STORAGE_H
#define MONITOR_NIVEL_AGUA_CONFIG_STORAGE_H

#include "SerialConsole.h"

/**
 * Armazena e recupera configuração do EEPROM do Arduino.
 *
 * O layout no EEPROM é:
 *   - uint32_t magic (0x4D4E5641 = 'MNVA')
 *   - SystemConfig (dados)
 */
class ConfigStorage {
public:
  static constexpr uint32_t kMagic = 0x4D4E5641UL; // 'MNVA'
  static constexpr int kEepromAddress = 0;

  /// Tenta carregar a configuração; retorna true se a configuração for válida.
  bool load(SystemConfig& config);

  /// Salva a configuração no EEPROM.
  void save(const SystemConfig& config);
};

#endif // MONITOR_NIVEL_AGUA_CONFIG_STORAGE_H
