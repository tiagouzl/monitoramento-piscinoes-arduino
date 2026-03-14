#ifndef MONITOR_NIVEL_AGUA_SD_TELEMETRY_H
#define MONITOR_NIVEL_AGUA_SD_TELEMETRY_H

#include <Arduino.h>
#include "Telemetry.h"

/**
 * Grava leituras em um arquivo CSV no cartão SD.
 *
 * Requer a biblioteca SD e um módulo de cartão SD conectado ao pino CS configurado.
 */
class SdTelemetry : public Telemetry {
public:
  SdTelemetry(uint8_t chipSelectPin, const char* filename = "log.csv");

  void begin() override;
  void publish(float distanceCm, float waterHeightCm, WaterLevelState state) override;

private:
  uint8_t _chipSelect;
  const char* _filename;
  bool _initialized;
};

#endif // MONITOR_NIVEL_AGUA_SD_TELEMETRY_H
