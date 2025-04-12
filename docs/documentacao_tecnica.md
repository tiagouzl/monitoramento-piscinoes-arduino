# Documentação Técnica - Sistema de Monitoramento de Nível de Água em Piscinões

## 1. Introdução

Este documento fornece uma descrição técnica detalhada do Sistema de Monitoramento de Nível de Água em Piscinões desenvolvido com a plataforma Arduino e conceitos de Internet das Coisas (IoT). O sistema tem como objetivo principal monitorar o nível de água em reservatórios de contenção (piscinões) e tomar ações preventivas automatizadas para evitar enchentes em áreas urbanas.

## 2. Descrição do Problema

As enchentes são um problema recorrente em grandes centros urbanos, resultando em danos materiais, riscos à saúde pública e transtornos à mobilidade urbana. Os piscinões são reservatórios projetados para reter temporariamente o excesso de água da chuva, minimizando os impactos das inundações. No entanto, seu gerenciamento eficiente depende do monitoramento contínuo dos níveis de água e da operação oportuna das comportas de escoamento.

## 3. Objetivos do Sistema

- Monitorar em tempo real o nível de água nos piscinões
- Fornecer indicações visuais do estado atual do reservatório
- Automatizar a operação de comportas para liberação controlada da água
- Permitir a expansão futura para integração com sistemas de alertas remotos

## 4. Arquitetura do Sistema

### 4.1 Componentes de Hardware

| Componente | Descrição | Função |
|------------|-----------|--------|
| Arduino UNO R3 | Microcontrolador baseado no ATmega328P | Unidade central de processamento |
| HC-SR04 | Sensor ultrassônico | Medição do nível de água |
| LEDs | Indicadores luminosos (verde e vermelho) | Sinalização visual do estado do sistema |
| Servo Motor | Atuador | Controle da comporta de liberação de água |
| Resistores 330Ω | Componentes passivos | Limitação da corrente para os LEDs |
| Protoboard e cabos | Componentes para montagem | Conexão entre os componentes |

### 4.2 Diagrama de Blocos

```
+----------------+     +----------------+     +----------------+
|                |     |                |     |                |
|  Sensor de     |---->|  Arduino UNO   |---->|  Indicadores   |
|  Nível (HC-SR04)|     |  (Controlador) |     |  (LEDs)        |
|                |     |                |     |                |
+----------------+     +----------------+     +----------------+
                             |
                             v
                       +----------------+
                       |                |
                       |  Atuador       |
                       |  (Servo Motor) |
                       |                |
                       +----------------+
```

### 4.3 Diagrama de Fluxo de Dados

```
+-------------+     +----------------+     +------------------+
| Leitura do  |---->| Processamento  |---->| Acionamento dos  |
| sensor      |     | e tomada de    |     | LEDs indicadores |
+-------------+     | decisão        |     +------------------+
                    |                |
                    +----------------+
                           |
                           v
                    +----------------+
                    | Controle da    |
                    | comporta       |
                    +----------------+
```

## 5. Funcionamento Detalhado

### 5.1 Princípio de Medição

O sensor ultrassônico HC-SR04 é posicionado na parte superior do reservatório, apontado para a superfície da água. Ele emite um pulso ultrassônico que reflete na superfície da água e retorna ao sensor. O tempo decorrido entre a emissão e a recepção do pulso é proporcional à distância entre o sensor e a superfície da água.

A distância é calculada pela fórmula:
```
distância = (tempo × velocidade do som) ÷ 2
```

Onde:
- Tempo é medido em microssegundos
- Velocidade do som é aproximadamente 340 m/s ou 0,034 cm/µs
- A divisão por 2 é necessária porque o tempo medido inclui o percurso de ida e volta do sinal

### 5.2 Níveis de Operação

O sistema opera com três níveis principais:

1. **Nível Normal** (distância > NIVEL_ALERTA)
   - LED verde aceso continuamente
   - Comporta fechada
   - Nenhuma ação necessária

2. **Nível de Alerta** (NIVEL_CRITICO < distância ≤ NIVEL_ALERTA)
   - LED verde piscando
   - Comporta fechada
   - Sistema em estado de vigilância aumentada

3. **Nível Crítico** (distância ≤ NIVEL_CRITICO)
   - LED vermelho aceso continuamente
   - Comporta aberta automaticamente
   - Liberação de água para evitar transbordamento

### 5.3 Controle da Comporta

O servo motor é utilizado para simular a operação de uma comporta real. Ele possui duas posições principais:

- **0 graus**: Comporta totalmente fechada
- **90 graus**: Comporta totalmente aberta

A transição entre estes estados é controlada automaticamente pelo sistema com base nos níveis de água detectados.

## 6. Configuração e Calibração

### 6.1 Parâmetros de Configuração

O sistema possui três parâmetros principais que podem ser ajustados conforme as características específicas do reservatório:

```cpp
const int NIVEL_SEGURO = 50;    // Nível seguro em cm
const int NIVEL_ALERTA = 30;    // Nível de alerta em cm  
const int NIVEL_CRITICO = 15;   // Nível crítico em cm
```

### 6.2 Calibração do Sensor

Para calibrar o sensor:
1. Posicione o sensor na altura máxima do reservatório
2. Meça a distância até o fundo do reservatório (reservatório vazio)
3. Meça a distância até o nível de água máximo desejado
4. Ajuste as constantes NIVEL_SEGURO, NIVEL_ALERTA e NIVEL_CRITICO no código

## 7. Expansões Propostas

### 7.1 Conectividade IoT

Para transformar o sistema em uma solução IoT completa, recomenda-se:

1. **Adição de módulo de comunicação**:
   - ESP8266 (Wi-Fi)
   - ESP32 (Wi-Fi + Bluetooth)
   - Módulo LoRa (para comunicação de longo alcance)

2. **Integração com plataformas de IoT**:
   - ThingSpeak
   - Blynk
   - AWS IoT
   - Google Cloud IoT

3. **Funcionalidades adicionais**:
   - Envio de notificações por e-mail ou SMS
   - Dashboard de monitoramento em tempo real
   - Armazenamento de dados históricos
   - Análise preditiva de níveis com base em dados históricos e previsões meteorológicas

### 7.2 Robustez para Implementação Real

Para implementação em ambiente real:

1. **Proteção física**:
   - Encapsulamento à prova d'água (IP67 ou superior)
   - Proteção contra vandalismo
   - Sistema de alimentação ininterrupta

2. **Redundância**:
   - Múltiplos sensores para maior precisão
   - Sistema de backup para falhas de energia
   - Comunicação por múltiplos canais

3. **Manutenção**:
   - Sistema de auto-diagnóstico
   - Alertas de manutenção preventiva
   - Calibração automática periódica

## 8. Limitações do Protótipo Atual

- O protótipo atual é adequado apenas para demonstração e fins educacionais
- O sensor HC-SR04 tem alcance limitado (2cm a 400cm) e pode ser afetado por ondulações na superfície da água
- O servo motor utilizado não possui torque suficiente para operar comportas reais
- Não há proteção contra condições climáticas ou umidade
- O sistema não possui redundância para falhas de componentes

## 9. Considerações Finais

Este sistema representa uma prova de conceito viável para o monitoramento automatizado de piscinões utilizando tecnologias acessíveis e de baixo custo. A implementação em escala real exigiria componentes mais robustos e uma arquitetura mais resiliente, mas os princípios fundamentais demonstrados aqui permanecem válidos.

A evolução para uma solução IoT completa permitiria não apenas o monitoramento e controle autônomo local, mas também a integração com sistemas mais amplos de gerenciamento urbano, contribuindo para a visão de cidades inteligentes e resilientes.

## 10. Referências

1. GUBBI, Jayavardhana et al. Internet of Things (IoT): A vision, architectural elements, and future directions. Future Generation Computer Systems, v. 29, n. 7, p. 1645–1660, 2013.

2. ZANELLA, Andrea et al. Internet of Things for Smart Cities. IEEE Internet of Things Journal, v. 1, n. 1, p. 22–32, 2014.

3. Arduino Official Documentation. Disponível em: https://www.arduino.cc/reference/

4. HC-SR04 Datasheet. Disponível em: https://cdn.sparkfun.com/datasheets/Sensors/Proximity/HCSR04.pdf

5. Servo Motor. Disponível em: https://www.servodatabase.com/servo/servo-motor/servo-9g-2500kv-2000rpm-0-5s-0-25a-micro-metal-gear-motor-for-arduino-r3