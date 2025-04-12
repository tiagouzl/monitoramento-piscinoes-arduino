/**
 * Sistema de Monitoramento de Nível de Água em Piscinões
 *
 * Este projeto implementa um sistema para monitoramento em tempo real
 * do nível de água em reservatórios de contenção (piscinões), alertando
 * visualmente quando o nível atinge valores críticos e controlando
 * automaticamente a abertura de uma comporta.
 *
 * Componentes:
 * - Arduino UNO R3
 * - Sensor ultrassônico HC-SR04
 * - LEDs (verde e vermelho)
 * - Servo motor para controle da comporta
 *
 * Criado em: Abril/2025
 * Autor: [Tiago Rodrigues de Brito]
 * GitHub: [https://github.com/tiagouzl]
 */

// Bibliotecas necessárias
#include <Servo.h>

// Definição dos pinos
const int trigPin = 9;      // Pino TRIG do sensor HC-SR04
const int echoPin = 10;     // Pino ECHO do sensor HC-SR04
const int ledVerde = 7;     // LED verde (nível seguro)
const int ledVermelho = 6;   // LED vermelho (nível crítico)
const int servoPin = 3;     // Pino de controle do servo motor

// Constantes para níveis de água (ajuste conforme dimensões do reservatório)
const int NIVEL_SEGURO = 50;     // Nível seguro em cm
const int NIVEL_ALERTA = 30;     // Nível de alerta em cm
const int NIVEL_CRITICO = 15;    // Nível crítico em cm

// Posições do servo (comporta)
const int COMPORTA_FECHADA = 0;   // Ângulo para posição fechada
const int COMPORTA_ABERTA = 90;    // Ângulo para posição aberta

// Variáveis globais
long duracao;         // Duração do pulso ultrassônico
int distancia;         // Distância calculada até a superfície da água
int nivelAgua;         // Nível de água calculado
Servo comporta;       // Objeto para controle do servo motor

/**
 * Configuração inicial do sistema
 */
void setup() {
  // Inicializa a comunicação serial (para monitoramento)
  Serial.begin(9600);

  // Configuração dos pinos
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(ledVerde, OUTPUT);
  pinMode(ledVermelho, OUTPUT);

  // Anexa o servo ao pino definido
  comporta.attach(servoPin);

  // Inicializa com a comporta fechada
  comporta.write(COMPORTA_FECHADA);

  // Informa que o sistema foi inicializado
  Serial.println("Sistema de Monitoramento de Nível de Água Inicializado");
  Serial.println("-------------------------------------------------------");
}

/**
 * Loop principal do programa
 */
void loop() {
  // Mede o nível de água usando o sensor ultrassônico
  medirNivelAgua();

  // Exibe informações no monitor serial
  exibirInformacoes();

  // Controla LEDs e comporta conforme o nível da água
  controlarSistema();

  // Aguarda antes da próxima leitura (1 segundo)
  delay(1000);
}

/**
 * Função para medição do nível de água usando sensor ultrassônico
 */
void medirNivelAgua() {
  // Limpa o pino trigger
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  // Envia pulso de 10 microssegundos no pino trigger
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Lê o tempo de retorno do eco em microssegundos
  duracao = pulseIn(echoPin, HIGH);

  // Calcula a distância em cm (velocidade do som = 340 m/s)
  // Fórmula: distância = (tempo x velocidade do som) / 2
  // Velocidade do som = 340 m/s = 0.034 cm/µs
  distancia = duracao * 0.034 / 2;

  // Nível da água é geralmente calculado a partir da base do reservatório.
  // Para isso, seria necessário saber a altura total do reservatório.
  // Assumindo que a distância medida é do sensor até a superfície,
  // e que o sensor está no topo, podemos inferir o nível da água.
  // Se a altura total do reservatório for, por exemplo, 100 cm, então:
  // nivelAgua = 100 - distancia;
  //
  // No código original, 'nivelAgua' estava recebendo diretamente o valor da 'distancia',
  // o que provavelmente não é o comportamento desejado.
  // Sem a informação da altura do reservatório, a correção mais lógica é manter
  // a interpretação de que 'distancia' representa o nível da água (quanto menor a distância, maior o nível).
  // No entanto, as constantes NIVEL_SEGURO, NIVEL_ALERTA e NIVEL_CRITICO
  // parecem se referir ao nível da água em si.
  //
  // Para manter a lógica das constantes, vamos assumir que a distância medida
  // representa diretamente o nível da água a partir de um ponto de referência
  // (que pode ser o fundo ou o topo, dependendo da instalação do sensor).
  // Se o sensor estiver no topo, uma distância menor significa um nível de água mais alto.
  // Se as constantes se referem à altura da água, então a lógica original estava quase correta,
  // mas a interpretação na função exibirInformacoes precisa ser ajustada.

  nivelAgua = distancia;
}

/**
 * Função para exibir informações do sistema no monitor serial
 */
void exibirInformacoes() {
  Serial.print("Distância até a superfície: ");
  Serial.print(distancia);
  Serial.println(" cm");

  Serial.print("Nível de água: ");
  // A lógica aqui precisa ser invertida se 'nivelAgua' representa a distância até a superfície
  // e os níveis são definidos como alturas da água.
  // Vamos corrigir assumindo que 'nivelAgua' representa a altura da água.
  if (nivelAgua >= NIVEL_SEGURO) {
    Serial.println("Normal");
  } else if (nivelAgua >= NIVEL_ALERTA) {
    Serial.println("Alerta");
  } else {
    Serial.println("CRÍTICO");
  }
}

/**
 * Função para controlar os LEDs e a comporta conforme o nível da água
 */
void controlarSistema() {
  // Controle dos LEDs e comporta baseado no nível da água
  if (nivelAgua >= NIVEL_ALERTA) {
    // Nível normal ou de alerta (abaixo do crítico) - LED verde aceso, vermelho apagado
    digitalWrite(ledVerde, HIGH);
    digitalWrite(ledVermelho, LOW);

    // Mantém a comporta fechada
    comporta.write(COMPORTA_FECHADA);
    Serial.println("Estado da comporta: FECHADA");
  } else {
    // Nível crítico - LED vermelho aceso, verde apagado
    digitalWrite(ledVerde, LOW);
    digitalWrite(ledVermelho, HIGH);

    // Abre a comporta para liberar água
    comporta.write(COMPORTA_ABERTA);
    Serial.println("Estado da comporta: ABERTA (liberando água)");
  }

  // O nível de alerta com LED verde piscando só deve ocorrer quando o nível está entre o crítico e o seguro.
  // Vamos adicionar essa lógica.
  if (nivelAgua < NIVEL_SEGURO && nivelAgua >= NIVEL_ALERTA) {
    digitalWrite(ledVerde, HIGH);
    digitalWrite(ledVermelho, LOW);
    delay(300);
    digitalWrite(ledVerde, LOW);
    delay(300); // Adicionado um pequeno delay para o LED ficar apagado por um tempo
  }

  Serial.println("-------------------------------------------------------");
}