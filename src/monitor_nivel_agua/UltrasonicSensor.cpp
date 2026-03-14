#include "UltrasonicSensor.h"

UltrasonicSensor::UltrasonicSensor(uint8_t trigPin, uint8_t echoPin, unsigned long timeoutUs, size_t averageSamples)
    : _trigPin(trigPin),
      _echoPin(echoPin),
      _timeoutUs(timeoutUs),
      _averageSamples(averageSamples),
      _bufferIndex(0),
      _bufferCount(0) {
}

void UltrasonicSensor::begin() {
  pinMode(_trigPin, OUTPUT);
  pinMode(_echoPin, INPUT);
  digitalWrite(_trigPin, LOW);
}

float UltrasonicSensor::computeAverage(float newSample) {
  if (_averageSamples <= 1) {
    return newSample;
  }

  // Reset filter if leitura falhar
  if (isnan(newSample)) {
    _bufferCount = 0;
    _bufferIndex = 0;
    return NAN;
  }

  _sampleBuffer[_bufferIndex] = newSample;
  _bufferIndex = (_bufferIndex + 1) % _averageSamples;
  if (_bufferCount < _averageSamples) {
    _bufferCount++;
  }

  float sum = 0.0f;
  for (size_t i = 0; i < _bufferCount; ++i) {
    sum += _sampleBuffer[i];
  }

  return sum / static_cast<float>(_bufferCount);
}

void UltrasonicSensor::setAverageSamples(size_t sampleCount) {
  _averageSamples = (sampleCount == 0) ? 1 : sampleCount;
  _bufferIndex = 0;
  _bufferCount = 0;
}

float UltrasonicSensor::readDistanceCm() {
  // Garante pulso inicial limpo
  digitalWrite(_trigPin, LOW);
  delayMicroseconds(2);

  // Envia pulso de 10µs
  digitalWrite(_trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(_trigPin, LOW);

  unsigned long duration = pulseIn(_echoPin, HIGH, _timeoutUs);
  if (duration == 0) {
    // Timeout: nada foi detectado (superfície muito distante ou falha)
    return computeAverage(NAN);
  }

  // Velocidade do som: 340 m/s = 0,034 cm/µs
  // O tempo retornado inclui ida e volta, por isso a divisão por 2.
  const float distanceCm = (duration * 0.034f) / 2.0f;
  return computeAverage(distanceCm);
}
