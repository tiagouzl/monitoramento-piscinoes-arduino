#ifndef MONITOR_NIVEL_AGUA_WATER_LEVEL_H
#define MONITOR_NIVEL_AGUA_WATER_LEVEL_H

#include <stdint.h>

/// Estado do nível de água utilizado para tomada de decisão do sistema.
enum class WaterLevelState : uint8_t {
  Unknown = 0,
  Safe,
  Alert,
  Critical,
};

#endif // MONITOR_NIVEL_AGUA_WATER_LEVEL_H
