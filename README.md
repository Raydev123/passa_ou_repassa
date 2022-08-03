Trabalho MK 2022 - Sistemas Embarcados

Eric Ryuta Nakao 10746922

Guilherme Henrique Santana Rosa 10873299

Rayron Costa e Silva 10820741

# Passa-ou-Repassa

## Introdução

No presente trabalho, tem-se como objetivo o desenvolvimento de um jogo realizado em programas de palco, o qual consiste em apresentar perguntas para cada participante/equipe e passar o direito de resposta a quem apertar mais rápido seu respectivo botão.

## Arquitetura e componentes do Sistema

- Cada jogador possui os seguintes:
      Um botão para pressionar;
      Um Led que indica que pontuou;
- O apresentador (quem aperta o botão) possui os seguintes:
      Um botão para iniciar o jogo;
      Um Led que indica o inicio do jogo;

### Funcionalidades do sistema:
- Indicação visual do início do jogo com o Led do apresentador;
- Indicação visual do primeiro botão pressionado com os Leds dos jogadores;
- Indicação visual de um dos botões terem sido pressionados antes da indicação de largada pelo Led de Inicio:
    - Indica a queimada de largada pelo jogador. Nesse caso, o outro jogador recebe o ponto da rodada;
    - O jogo retorna ao estado inicial, esperando o apresentador apertar o botao;
- Armazenamento da pontuacao dos jogadores;
    - A pontuacao de cada jogador fica armazenado na variavel PlayerX_Score;
- Armazenamento da diferenca de tempo entre a indicação de largada e o pressionar do botão pelo participante
    - De inicio, o armazenamento é dado por um array de 10 indices (pode ser alterado conforme necessidade do apresentador) que armazenam o tempo

### Descrição da arquitetura 

![Image of Architecture](https://github.com/guilh-0453/passa_ou_repassa/blob/main/System_Architecture.jpg)
  1. 3 Entradas - botões a serem pressionados (Início, participante/equipe 1 e participante/equipe 2)
  2. 3 Saídas  - LEDs (Início, participante/equipe 1, participante/equipe 2)

### State Machine for the Software implementation of the Game
![MaquinadeEstado](https://github.com/guilh-0453/passa_ou_repassa/blob/main/State_Machine.png)


### Simulacao na maquina host

Foi feito possivel a simulacao do programa na maquina host para facilitar a prototipagem do codigo main:
- O input de botoes foi feito no programa botao_simulacao.c:
    - O botao_simulacao.c deve receber como argumento(argv) um fifo(ou similar) para input de dados
    > Exemplo:
    > ./botao_simulacao fifo_exemplo (abrindo o programa com o fifo como argumento) 
    > 
    > echo "comando" (sem aspas) > fifo_exemplo (input do comando no programa pelo fifo_exemplo)
- O "comando" indicado acima representa os comandos possiveis de serem passados para o botao_simulacao.c, sendo estes:
    - botao_x (indica que o jogador x pressionou e soltou o botao - uso normal do botao);
    - press_x (mostra que o jogador x manteve pressionado o botao - uso para verificar a queimada de largada);
    - soltar_x (solta o botao do jogador x - usado como "reset" do press_x);
- Dado o input de botoes pelo simulador, o programa altera seu estado atual, conforme a maquina de estados;
- Obs: Para rodar o simulador junto do programa sao necessarias as pastas(sys/gpio/gpioXX/)
    
Exemplo do programa rodando com o uso do simulador:
    
- Starting_State:
    - Mostra o placar atual;
    - Desliga os leds;
    
    ![image](https://user-images.githubusercontent.com/109694851/182676215-258c681b-be6d-4b34-a8b7-d60d8af89cf9.png)
    
- Game_Running_State, após o apresentador apertar o botao (echo botao_start > fifo_exemplo)
    - Liga o Led do apresentador, mostrando que o jogo comecou;
    
    ![image](https://user-images.githubusercontent.com/109694851/182676804-0e77d4aa-19f5-4eed-a785-490d857023e5.png)
    
- Player_X_State, após um dos jogadores apertar o botão (echo botao_x > fifo_exemplo)
    - Liga o Led do jogador x, mostrando que pontuou;
    - Adiciona ponto ao jogador x;
    - Retorna ao Starting_State, aguardando o apresentador apertar o botao;
    
    ![image](https://user-images.githubusercontent.com/109694851/182677405-4c522f96-9477-481f-9df5-b826dc5aa9fe.png)
    
Exemplo do programa com o jogador 1 queimando a largada:
    
- Starting_State: Igual ao exemplo anterior;
    
- Game_Running_State:
    - Antes do apresentador apertar o botao, o jogador 1 já havia apertado (echo press_1 > fifo_exemplo);
    - Como o jogador apertou o botao antes do apresentador, houve a queimada de largada do jogador 1;
    
        ![image](https://user-images.githubusercontent.com/109694851/182679749-2d9a0745-c7cb-4365-8588-c500c7fbe798.png)
        
- Player_2_State, após a queimada de largada pelo jogador 1
    - O jogador 2 recebe a pontuacao da rodada;
    - O jogo retorna ao Starting_State;
    
        ![image](https://user-images.githubusercontent.com/109694851/182679400-1e295a19-2697-44b3-a6db-6d283f8dc40f.png)

    
    

