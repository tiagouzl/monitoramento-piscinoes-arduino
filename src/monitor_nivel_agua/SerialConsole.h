#ifndef MONITOR_NIVEL_AGUA_SERIAL_CONSOLE_H
#define MONITOR_NIVEL_AGUA_SERIAL_CONSOLE_H

#include <Arduino.h>
#include "WaterLevel.h"

struct SystemConfig {
  float reservoirHeightCm;
  float levelSafeCm;
  float levelAlertCm;
  float levelCriticalCm;
  size_t distanceAverageSamples;
  bool sdLoggingEnabled;
};

class SerialConsole {
public:
  SerialConsole(Stream& stream);

  /// Inicializa a console (apenas imprime help inicial)
  void begin(const SystemConfig& config);

  /// Deve ser chamada em loop para processar comandos recebidos
  void update(SystemConfig& config, WaterLevelState currentState, float distanceCm, float waterLevelCm);

private:
  void processLine(const String& line, SystemConfig& config);
  void printHelp();
  void printStatus(WaterLevelState state, float distanceCm, float waterLevelCm);
  void printConfig(const SystemConfig& config);

  Stream* _stream;
  static constexpr size_t kBufferSize = 80;
  char _buffer[kBufferSize];
  size_t _bufferIdx;
};

#endif // MONITOR_NIVEL_AGUA_SERIAL_CONSOLE_H
