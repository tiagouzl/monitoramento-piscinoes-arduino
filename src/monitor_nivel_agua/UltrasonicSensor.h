#ifndef MONITOR_NIVEL_AGUA_ULTRASONIC_SENSOR_H
#define MONITOR_NIVEL_AGUA_ULTRASONIC_SENSOR_H

#include <Arduino.h>

/**
 * Leitura de distância (cm) a partir do sensor ultrassônico HC-SR04.
 *
 * O valor retornado é a distância entre o sensor e a superfície medida.
 * Para calcular o nível de água (altura), é necessário usar a altura do reservatório.
 */
class UltrasonicSensor {
public:
  UltrasonicSensor(uint8_t trigPin, uint8_t echoPin, unsigned long timeoutUs, size_t averageSamples = 1);

  /// Inicializa os pinos (deve ser chamado em setup)
  void begin();

  /// Lê a distância (cm). Retorna NaN em caso de erro (timeout).
  float readDistanceCm();

  /// Ajusta o número de amostras usadas na média móvel (1 = desativado)
  void setAverageSamples(size_t sampleCount);

private:
  float computeAverage(float newSample);

  uint8_t _trigPin;
  uint8_t _echoPin;
  unsigned long _timeoutUs;

  size_t _averageSamples;
  float _sampleBuffer[16];
  size_t _bufferIndex;
  size_t _bufferCount;
};

#endif // MONITOR_NIVEL_AGUA_ULTRASONIC_SENSOR_H
