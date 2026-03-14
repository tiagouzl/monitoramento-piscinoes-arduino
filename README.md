# Sistema de Monitoramento de Nível de Água em Piscinões

## Descrição

Este projeto implementa um sistema de monitoramento do nível de água em reservatórios de contenção de enchentes (piscinões) utilizando Arduino e conceitos de Internet das Coisas (IoT).

O sistema é capaz de:
- Monitorar em tempo real o nível de água através de um sensor ultrassônico
- Fornecer alertas visuais através de LEDs verde e vermelho
- Controlar automaticamente uma comporta através de um servo motor
- Registrar e exibir as informações no monitor serial

## Componentes

- Arduino UNO R3
- Sensor ultrassônico HC-SR04
- LEDs verde e vermelho
- Resistores de 330Ω
- Servo motor
- Protoboard e cabos de conexão

## Como Usar

1. Monte o circuito conforme o diagrama fornecido
2. Carregue o código no Arduino usando a IDE
3. Ajuste as constantes de nível e a altura do reservatório no arquivo `src/monitor_nivel_agua/config.h`:
   - `RESERVOIR_HEIGHT_CM` (altura total medida do sensor até o fundo)
   - `LEVEL_SAFE_CM`, `LEVEL_ALERT_CM`, `LEVEL_CRITICAL_CM` (níveis de operação)
4. Monitore o funcionamento através do Serial Monitor

### Console interativo (Serial)
Você pode enviar comandos via Serial Monitor (9600 baud):

- `help` → mostra comandos disponíveis
- `status` → mostra último status lido
- `config` → exibe configuração ativa
- `set <param> <valor>` → altera configuração (reservoir/safe/alert/critical/average)

Exemplo: `set alert 25` (altera nível de alerta para 25 cm)

> Os valores configurados via `set` são salvos na EEPROM e persistem após desligar/religar o Arduino.

### Telemetria e registro
- O sistema também publica uma linha JSON por ciclo no Serial (útil para loggers externos).
- É possível habilitar registro em cartão SD alterando `ENABLE_SD_LOGGER` para `true` em `src/monitor_nivel_agua/config.h` ou em runtime via `set sd 1`.
  - O arquivo gravado será `nivel.csv` e conterá: `ts,dist_cm,water_cm,state`

## Expansões Possíveis

- Adicionar módulo WiFi para envio de dados para a nuvem
- Implementar um sistema de alertas por email ou SMS
- Desenvolver um aplicativo para monitoramento remoto
- Adicionar mais sensores para monitoramento de qualidade da água

## Licença

Este projeto está licenciado sob a Licença MIT - veja o arquivo LICENSE para detalhes.

## Contato

[Tiago Rodrigues de Brito] - [tiago.uzl@gmail.com]