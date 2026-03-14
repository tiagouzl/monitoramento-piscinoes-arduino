#ifndef MONITOR_NIVEL_AGUA_TELEMETRY_H
#define MONITOR_NIVEL_AGUA_TELEMETRY_H

#include <Arduino.h>
#include "WaterLevel.h"

/**
 * Interface para telemetria (registro/saída de dados).
 */
class Telemetry {
public:
  virtual ~Telemetry() = default;

  /// Inicializa o transporte (Serial, SD, rede, etc.)
  virtual void begin() = 0;

  /// Publica um registro de leitura.
  virtual void publish(float distanceCm, float waterHeightCm, WaterLevelState state) = 0;
};

#endif // MONITOR_NIVEL_AGUA_TELEMETRY_H
