#include "SerialTelemetry.h"

SerialTelemetry::SerialTelemetry(Stream& stream)
    : _stream(&stream) {
}

void SerialTelemetry::begin() {
  // Nada além de garantir que a Serial esteja inicializada.
}

void SerialTelemetry::publish(float distanceCm, float waterHeightCm, WaterLevelState state) {
  // Produz JSON simples com timestamp em ms
  _stream->print("{\"ts\":");
  _stream->print(millis());
  _stream->print(",\"dist\":");
  if (isnan(distanceCm)) {
    _stream->print("null");
  } else {
    _stream->print(distanceCm, 1);
  }
  _stream->print(",\"water\":");
  if (isnan(waterHeightCm)) {
    _stream->print("null");
  } else {
    _stream->print(waterHeightCm, 1);
  }
  _stream->print(",\"state\":\"");
  switch (state) {
    case WaterLevelState::Safe:
      _stream->print("safe");
      break;
    case WaterLevelState::Alert:
      _stream->print("alert");
      break;
    case WaterLevelState::Critical:
      _stream->print("critical");
      break;
    default:
      _stream->print("unknown");
      break;
  }
  _stream->println("\"}");
}
