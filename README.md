Trabalho MK 2022 - Sistemas Embarcados

Eric Ryuta Nakao 10746922

Guilherme Henrique Santana Rosa 10873299

Rayron Costa e Silva 10820741

# Passa-ou-Repassa

## Introdução

No presente trabalho, tem-se como objetivo o desenvolvimento de um jogo realizado em programas de palco, o qual consiste em apresentar perguntas para cada participante/equipe e passar o direito de resposta a quem apertar mais rápido seu respectivo botão.

## Envelope de trabalho

### Requisitos do sistema:
- Mais de um botão (um para cada participante/equipe)
- Indicações visuais tanto de início do jogo quanto para apontar qual botão foi apertado primeiro e por qual participante/equipe

### Funcionalidades do sistema:
- Indicação visual do início do jogo
- Indicação visual do primeiro botão pressionado
- Indicação visual de um dos botões terem sido pressionados antes da indicação de largada pelo Led de Inicio
- Armazenamento da diferença de tempo entre a indicação de largada e o pressionar do botão pelo participante
- Dispensa do uso de um computador "host"

### Descrição da arquitetura

![Image of Architecture](https://github.com/guilh-0453/passa_ou_repassa/blob/main/System_Architecture.jpg)
  1. 3 Entradas - botões a serem pressionados (Início, participante/equipe 1 e participante/equipe 2)
  2. 3 Saídas  - LEDs (Início, participante/equipe 1, participante/equipe 2)

### State Machine for the Software implementation of the Game
![MaquinadeEstado](https://github.com/guilh-0453/passa_ou_repassa/blob/main/State_Machine.png)
