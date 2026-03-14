#include "GateController.h"

GateController::GateController(uint8_t servoPin, uint8_t closedAngle, uint8_t openAngle)
    : _servoPin(servoPin), _closedAngle(closedAngle), _openAngle(openAngle), _lastState(WaterLevelState::Unknown) {
}

void GateController::begin() {
  _servo.attach(_servoPin);
  _servo.write(_closedAngle);
}

void GateController::update(WaterLevelState level) {
  // Apenas altera a posição do servo se houver mudança de estado, para minimizar movimento.
  if (level == _lastState) {
    return;
  }

  _lastState = level;
  switch (level) {
    case WaterLevelState::Critical:
      _servo.write(_openAngle);
      break;

    case WaterLevelState::Safe:
    case WaterLevelState::Alert:
    case WaterLevelState::Unknown:
    default:
      _servo.write(_closedAngle);
      break;
  }
}
