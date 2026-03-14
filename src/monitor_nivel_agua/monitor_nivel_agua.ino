#include <Arduino.h>
#include <string.h>

#include "config.h"
#include "ConfigStorage.h"
#include "GateController.h"
#include "Indicator.h"
#include "SerialConsole.h"
#include "Telemetry.h"
#include "SerialTelemetry.h"
#include "SdTelemetry.h"
#include "UltrasonicSensor.h"
#include "WaterLevel.h"

// Componentes do sistema
UltrasonicSensor g_ultrasonic(PIN_TRIG, PIN_ECHO, ECHO_TIMEOUT_US, DISTANCE_AVERAGE_SAMPLES);
Indicator g_indicator(PIN_LED_GREEN, PIN_LED_RED, BLINK_INTERVAL_MS);
GateController g_gate(PIN_SERVO, SERVO_ANGLE_CLOSED, SERVO_ANGLE_OPEN);
SerialConsole g_console(Serial);
SerialTelemetry g_serialTelemetry(Serial);
SdTelemetry g_sdTelemetry(SD_CS_PIN, "nivel.csv");

unsigned long g_lastLoopMs = 0;
WaterLevelState g_lastState = WaterLevelState::Unknown;

SystemConfig g_config;
SystemConfig g_configLast = {};
size_t g_lastAverageSamples = 0;

static bool configChanged(const SystemConfig& a, const SystemConfig& b) {
  return memcmp(&a, &b, sizeof(SystemConfig)) != 0;
}

static WaterLevelState evaluateLevel(float waterHeightCm, const SystemConfig& cfg) {
  if (isnan(waterHeightCm)) {
    return WaterLevelState::Unknown;
  }

  if (waterHeightCm <= cfg.levelCriticalCm) {
    return WaterLevelState::Critical;
  }

  if (waterHeightCm <= cfg.levelAlertCm) {
    return WaterLevelState::Alert;
  }

  return WaterLevelState::Safe;
}

static void logStatus(float distanceCm, float waterHeightCm, WaterLevelState state) {
  Serial.print("Distância (sensor->água): ");
  if (isnan(distanceCm)) {
    Serial.print("ERRO");
  } else {
    Serial.print(distanceCm, 1);
    Serial.print(" cm");
  }

  Serial.print("  |  Nível de água: ");
  if (isnan(waterHeightCm)) {
    Serial.print("ERRO");
  } else {
    Serial.print(waterHeightCm, 1);
    Serial.print(" cm");
  }

  Serial.print("  |  Estado: ");
  switch (state) {
    case WaterLevelState::Safe:
      Serial.println("Seguro");
      break;
    case WaterLevelState::Alert:
      Serial.println("Alerta");
      break;
    case WaterLevelState::Critical:
      Serial.println("Crítico");
      break;
    default:
      Serial.println("Desconhecido");
      break;
  }
}

void setup() {
  Serial.begin(9600);

  g_ultrasonic.begin();
  g_indicator.begin();
  g_gate.begin();
  g_serialTelemetry.begin();
  if (g_config.sdLoggingEnabled) {
    g_sdTelemetry.begin();
  }

  // Tenta carregar configuração salva no EEPROM; caso contrário, usa valores padrão.
  ConfigStorage storage;
  if (!storage.load(g_config)) {
    g_config = {
      .reservoirHeightCm = RESERVOIR_HEIGHT_CM,
      .levelSafeCm = LEVEL_SAFE_CM,
      .levelAlertCm = LEVEL_ALERT_CM,
      .levelCriticalCm = LEVEL_CRITICAL_CM,
      .distanceAverageSamples = DISTANCE_AVERAGE_SAMPLES,
      .sdLoggingEnabled = ENABLE_SD_LOGGER,
    };
  }

  g_lastAverageSamples = g_config.distanceAverageSamples;
  g_console.begin(g_config);

  Serial.println("Sistema de Monitoramento de Nível de Água Inicializado");
  Serial.println("--------------------------------------------------");
}

void loop() {
  const unsigned long now = millis();
  static float lastDistanceCm = NAN;
  static float lastWaterHeightCm = NAN;

  if (now - g_lastLoopMs >= LOOP_INTERVAL_MS) {
    g_lastLoopMs = now;

    // Se a média de amostras foi alterada via console, aplicamos no sensor.
    if (g_config.distanceAverageSamples != g_lastAverageSamples) {
      g_ultrasonic.setAverageSamples(g_config.distanceAverageSamples);
      g_lastAverageSamples = g_config.distanceAverageSamples;
    }

    lastDistanceCm = g_ultrasonic.readDistanceCm();
    lastWaterHeightCm = isnan(lastDistanceCm)
                          ? NAN
                          : constrain(g_config.reservoirHeightCm - lastDistanceCm, 0.0f, g_config.reservoirHeightCm);

    g_lastState = evaluateLevel(lastWaterHeightCm, g_config);

    g_gate.update(g_lastState);
    logStatus(lastDistanceCm, lastWaterHeightCm, g_lastState);

    g_serialTelemetry.publish(lastDistanceCm, lastWaterHeightCm, g_lastState);
    if (g_config.sdLoggingEnabled) {
      g_sdTelemetry.publish(lastDistanceCm, lastWaterHeightCm, g_lastState);
    }
  }

  g_indicator.update(g_lastState, now);
  g_console.update(g_config, g_lastState, lastDistanceCm, lastWaterHeightCm);

  // Se a configuração mudou via console, persiste no EEPROM.
  if (configChanged(g_config, g_configLast)) {
    ConfigStorage storage;
    storage.save(g_config);
    g_configLast = g_config;
  }
}
