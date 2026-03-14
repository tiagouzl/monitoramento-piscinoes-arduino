#ifndef MONITOR_NIVEL_AGUA_INDICATOR_H
#define MONITOR_NIVEL_AGUA_INDICATOR_H

#include <Arduino.h>
#include "WaterLevel.h"

/**
 * Controla os LEDs indicadores de status.
 *
 * O método update() deve ser chamado frequentemente, pois gerencia o piscar
 * do LED sem bloquear o loop principal.
 */
class Indicator {
public:
  Indicator(uint8_t greenPin, uint8_t redPin, unsigned long blinkIntervalMs);

  /// Inicializa os pinos (deve ser chamado em setup)
  void begin();

  /// Atualiza o estado do indicador (não bloqueante)
  void update(WaterLevelState level, unsigned long currentMillis);

private:
  uint8_t _greenPin;
  uint8_t _redPin;
  unsigned long _blinkIntervalMs;

  unsigned long _lastToggleMs;
  bool _greenOn;
};

#endif // MONITOR_NIVEL_AGUA_INDICATOR_H
