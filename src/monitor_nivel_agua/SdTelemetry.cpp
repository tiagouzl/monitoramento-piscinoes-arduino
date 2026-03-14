#include "SdTelemetry.h"
#include <SD.h>

SdTelemetry::SdTelemetry(uint8_t chipSelectPin, const char* filename)
    : _chipSelect(chipSelectPin), _filename(filename), _initialized(false) {
}

void SdTelemetry::begin() {
  if (_initialized) {
    return;
  }

  if (!SD.begin(_chipSelect)) {
    return;
  }

  // Se o arquivo não existir, cria e adiciona cabeçalho.
  if (!SD.exists(_filename)) {
    File f = SD.open(_filename, FILE_WRITE);
    if (f) {
      f.println("ts,dist_cm,water_cm,state");
      f.close();
    }
  }

  _initialized = true;
}

void SdTelemetry::publish(float distanceCm, float waterHeightCm, WaterLevelState state) {
  if (!_initialized) {
    return;
  }

  File f = SD.open(_filename, FILE_WRITE);
  if (!f) {
    return;
  }

  f.print(millis());
  f.print(",");
  if (isnan(distanceCm)) {
    f.print("NaN");
  } else {
    f.print(distanceCm, 1);
  }
  f.print(",");
  if (isnan(waterHeightCm)) {
    f.print("NaN");
  } else {
    f.print(waterHeightCm, 1);
  }
  f.print(",");

  switch (state) {
    case WaterLevelState::Safe:
      f.print("safe");
      break;
    case WaterLevelState::Alert:
      f.print("alert");
      break;
    case WaterLevelState::Critical:
      f.print("critical");
      break;
    default:
      f.print("unknown");
      break;
  }

  f.println();
  f.close();
}
