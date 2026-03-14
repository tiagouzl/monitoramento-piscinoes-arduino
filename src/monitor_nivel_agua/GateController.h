#ifndef MONITOR_NIVEL_AGUA_GATE_CONTROLLER_H
#define MONITOR_NIVEL_AGUA_GATE_CONTROLLER_H

#include <Servo.h>
#include "WaterLevel.h"

/**
 * Controla o atuador (servo que simula a comporta).
 */
class GateController {
public:
  GateController(uint8_t servoPin, uint8_t closedAngle, uint8_t openAngle);

  /// Inicializa o servo (deve ser chamado em setup)
  void begin();

  /// Atualiza a posição da comporta com base no nível da água
  void update(WaterLevelState level);

private:
  Servo _servo;
  uint8_t _servoPin;
  uint8_t _closedAngle;
  uint8_t _openAngle;
  WaterLevelState _lastState;
};

#endif // MONITOR_NIVEL_AGUA_GATE_CONTROLLER_H
