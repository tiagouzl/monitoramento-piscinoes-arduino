#ifndef MONITOR_NIVEL_AGUA_CONFIG_H
#define MONITOR_NIVEL_AGUA_CONFIG_H

#include <stddef.h>
#include <stdint.h>

// ------------------------------------------------------------
// Pinagem
// ------------------------------------------------------------
constexpr uint8_t PIN_TRIG = 9;          // Pino TRIG do sensor HC-SR04
constexpr uint8_t PIN_ECHO = 10;         // Pino ECHO do sensor HC-SR04
constexpr uint8_t PIN_LED_GREEN = 7;     // LED verde (nível seguro)
constexpr uint8_t PIN_LED_RED = 6;       // LED vermelho (nível crítico)
constexpr uint8_t PIN_SERVO = 3;         // Pino de controle do servo motor

// ------------------------------------------------------------
// Geometria do reservatório (ajuste conforme instalação)
// ------------------------------------------------------------
// Altura total (em cm) do reservatório medida do sensor até o fundo.
// O cálculo do nível de água é feito a partir desta base.
constexpr float RESERVOIR_HEIGHT_CM = 100.0f;

// ------------------------------------------------------------
// Limiares de nível de água (em cm) - definir com base na altura do reservatório
// ------------------------------------------------------------
constexpr float LEVEL_SAFE_CM = 50.0f;       // Nível seguro (baixo risco)
constexpr float LEVEL_ALERT_CM = 30.0f;      // Nível de alerta (atenção)
constexpr float LEVEL_CRITICAL_CM = 15.0f;   // Nível crítico (ação imediata)

// ------------------------------------------------------------
// Servo (comporta) - ângulos
// ------------------------------------------------------------
constexpr uint8_t SERVO_ANGLE_CLOSED = 0;    // Comporta totalmente fechada
constexpr uint8_t SERVO_ANGLE_OPEN = 90;     // Comporta totalmente aberta

// ------------------------------------------------------------// Telemetria / Logging
// ------------------------------------------------------------
// Para usar gravação em cartão SD, habilite e configure o pino CS abaixo.
constexpr bool ENABLE_SD_LOGGER = false;
constexpr uint8_t SD_CS_PIN = 10;

// ------------------------------------------------------------// Filtros e temporizações
// ------------------------------------------------------------
constexpr size_t DISTANCE_AVERAGE_SAMPLES = 5;          // Média móvel para estabilidade de leitura (1 = desativado)
constexpr unsigned long LOOP_INTERVAL_MS = 1000UL;      // Intervalo do loop principal
constexpr unsigned long BLINK_INTERVAL_MS = 500UL;      // Intervalo do piscar de LED
constexpr unsigned long ECHO_TIMEOUT_US = 30000UL;      // Timeout do pulseIn (30ms ~ 5m)

#endif // MONITOR_NIVEL_AGUA_CONFIG_H
