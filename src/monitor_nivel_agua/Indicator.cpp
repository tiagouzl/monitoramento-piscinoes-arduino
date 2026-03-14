#include "Indicator.h"

Indicator::Indicator(uint8_t greenPin, uint8_t redPin, unsigned long blinkIntervalMs)
    : _greenPin(greenPin), _redPin(redPin), _blinkIntervalMs(blinkIntervalMs), _lastToggleMs(0), _greenOn(false) {
}

void Indicator::begin() {
  pinMode(_greenPin, OUTPUT);
  pinMode(_redPin, OUTPUT);
  digitalWrite(_greenPin, LOW);
  digitalWrite(_redPin, LOW);
}

void Indicator::update(WaterLevelState level, unsigned long currentMillis) {
  bool redOn = false;
  bool greenOn = false;

  switch (level) {
    case WaterLevelState::Safe:
      greenOn = true;
      break;

    case WaterLevelState::Alert:
      // Pisca o LED verde
      if (currentMillis - _lastToggleMs >= _blinkIntervalMs) {
        _greenOn = !_greenOn;
        _lastToggleMs = currentMillis;
      }
      greenOn = _greenOn;
      break;

    case WaterLevelState::Critical:
      redOn = true;
      break;

    case WaterLevelState::Unknown:
    default:
      // Pisca ambos LEDs para indicar erro de leitura
      if (currentMillis - _lastToggleMs >= _blinkIntervalMs) {
        _greenOn = !_greenOn;
        _lastToggleMs = currentMillis;
      }
      redOn = _greenOn;
      greenOn = !_greenOn;
      break;
  }

  digitalWrite(_greenPin, greenOn ? HIGH : LOW);
  digitalWrite(_redPin, redOn ? HIGH : LOW);
}
