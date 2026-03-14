#include "SerialConsole.h"

SerialConsole::SerialConsole(Stream& stream)
    : _stream(&stream), _bufferIdx(0) {
}

void SerialConsole::begin(const SystemConfig& config) {
  _bufferIdx = 0;
  printHelp();
  printConfig(config);
}

void SerialConsole::update(SystemConfig& config, WaterLevelState currentState, float distanceCm, float waterLevelCm) {
  while (_stream->available()) {
    const int available = _stream->readBytes(&_buffer[_bufferIdx], 1);
    if (available <= 0) {
      break;
    }

    const char c = _buffer[_bufferIdx];
    if (c == '\r') {
      continue;
    }

    if (c == '\n' || _bufferIdx + 1 >= kBufferSize) {
      _buffer[_bufferIdx] = '\0';
      processLine(String(_buffer));
      _bufferIdx = 0;
      printStatus(currentState, distanceCm, waterLevelCm);
      return;
    }

    _bufferIdx++;
  }
}

void SerialConsole::processLine(const String& line, SystemConfig& config) {
  const String trimmed = line.trim();
  if (trimmed.length() == 0) {
    return;
  }

  const int firstSpace = trimmed.indexOf(' ');
  const String command = (firstSpace < 0) ? trimmed : trimmed.substring(0, firstSpace);
  const String args = (firstSpace < 0) ? String() : trimmed.substring(firstSpace + 1);

  if (command.equalsIgnoreCase("help") || command.equalsIgnoreCase("h")) {
    printHelp();
    return;
  }

  if (command.equalsIgnoreCase("status") || command.equalsIgnoreCase("s")) {
    printStatus(WaterLevelState::Unknown, NAN, NAN);
    return;
  }

  if (command.equalsIgnoreCase("config") || command.equalsIgnoreCase("c")) {
    printConfig(config);
    return;
  }

  if (command.equalsIgnoreCase("set")) {
    const int argSpace = args.indexOf(' ');
    if (argSpace < 0) {
      _stream->println("Uso: set <param> <valor>");
      return;
    }

    const String name = args.substring(0, argSpace);
    const String valueStr = args.substring(argSpace + 1);
    const float value = valueStr.toFloat();

    if (name.equalsIgnoreCase("reservoir") || name.equalsIgnoreCase("height")) {
      config.reservoirHeightCm = value;
      _stream->println("Reservatório atualizado.");
      return;
    }

    if (name.equalsIgnoreCase("safe")) {
      config.levelSafeCm = value;
      _stream->println("Nível seguro atualizado.");
      return;
    }

    if (name.equalsIgnoreCase("alert")) {
      config.levelAlertCm = value;
      _stream->println("Nível alerta atualizado.");
      return;
    }

    if (name.equalsIgnoreCase("critical")) {
      config.levelCriticalCm = value;
      _stream->println("Nível crítico atualizado.");
      return;
    }

    if (name.equalsIgnoreCase("average")) {
      const size_t sampleCount = static_cast<size_t>(value);
      config.distanceAverageSamples = (sampleCount == 0) ? 1 : sampleCount;
      _stream->println("Número de amostras de média atualizado.");
      return;
    }

    if (name.equalsIgnoreCase("sd") || name.equalsIgnoreCase("sdlog")) {
      config.sdLoggingEnabled = (value > 0.0);
      _stream->println("Registro em SD atualizado.");
      return;
    }

    _stream->print("Parâmetro desconhecido: ");
    _stream->println(name);
    return;
  }

  _stream->print("Comando desconhecido: ");
  _stream->println(command);
}

void SerialConsole::printHelp() {
  _stream->println("Comandos disponíveis:");
  _stream->println("  help            - mostra este menu");
  _stream->println("  status          - mostra status atual");
  _stream->println("  config          - mostra a configuração atual");
  _stream->println("  set <param> <v> - altera configuração (reservoir/safe/alert/critical/average/sd)");
  _stream->println();
}

void SerialConsole::printStatus(WaterLevelState state, float distanceCm, float waterLevelCm) {
  _stream->print("[STATUS] ");
  _stream->print("Dist: ");
  if (isnan(distanceCm)) {
    _stream->print("ERRO");
  } else {
    _stream->print(distanceCm, 1);
    _stream->print(" cm");
  }
  _stream->print(" | Água: ");
  if (isnan(waterLevelCm)) {
    _stream->print("ERRO");
  } else {
    _stream->print(waterLevelCm, 1);
    _stream->print(" cm");
  }
  _stream->print(" | Estado: ");

  switch (state) {
    case WaterLevelState::Safe:
      _stream->println("Seguro");
      break;
    case WaterLevelState::Alert:
      _stream->println("Alerta");
      break;
    case WaterLevelState::Critical:
      _stream->println("Crítico");
      break;
    default:
      _stream->println("Desconhecido");
      break;
  }
}

void SerialConsole::printConfig(const SystemConfig& config) {
  _stream->println("[CONFIG] ");
  _stream->print("  Reservatorio: ");
  _stream->print(config.reservoirHeightCm, 1);
  _stream->println(" cm");

  _stream->print("  Seguro:      ");
  _stream->print(config.levelSafeCm, 1);
  _stream->println(" cm");

  _stream->print("  Alerta:      ");
  _stream->print(config.levelAlertCm, 1);
  _stream->println(" cm");

  _stream->print("  Critico:     ");
  _stream->print(config.levelCriticalCm, 1);
  _stream->println(" cm");

  _stream->print("  Média (amostras): ");
  _stream->println(config.distanceAverageSamples);

  _stream->print("  SD log:      ");
  _stream->println(config.sdLoggingEnabled ? "ON" : "OFF");
}
