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
3. Ajuste as constantes de nível (NIVEL_SEGURO, NIVEL_ALERTA, NIVEL_CRITICO) conforme as dimensões do seu reservatório
4. Monitore o funcionamento através do Serial Monitor

## Expansões Possíveis

- Adicionar módulo WiFi para envio de dados para a nuvem
- Implementar um sistema de alertas por email ou SMS
- Desenvolver um aplicativo para monitoramento remoto
- Adicionar mais sensores para monitoramento de qualidade da água

## Licença

Este projeto está licenciado sob a Licença MIT - veja o arquivo LICENSE para detalhes.

## Contato

[Tiago Rodrigues de Brito] - [tiago.uzl@gmail.com]