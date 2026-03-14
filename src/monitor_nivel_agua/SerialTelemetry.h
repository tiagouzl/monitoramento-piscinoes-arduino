#ifndef MONITOR_NIVEL_AGUA_SERIAL_TELEMETRY_H
#define MONITOR_NIVEL_AGUA_SERIAL_TELEMETRY_H

#include <Arduino.h>
#include "Telemetry.h"

/**
 * Produz saída JSON simples no Serial.
 */
class SerialTelemetry : public Telemetry {
public:
  explicit SerialTelemetry(Stream& stream);

  void begin() override;
  void publish(float distanceCm, float waterHeightCm, WaterLevelState state) override;

private:
  Stream* _stream;
};

#endif // MONITOR_NIVEL_AGUA_SERIAL_TELEMETRY_H
