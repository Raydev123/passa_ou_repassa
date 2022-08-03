///Links possiveis para revisao de termos utilizados durante o programa:
/// https://www.embarcados.com.br/maquina-de-estado/
/// https://www.kennethkuhn.com/electronics/debounce.c
/// https://stackoverflow.com/questions/5256599/what-are-file-descriptors-explained-in-simple-terms
/// https://pubs.opengroup.org/onlinepubs/9699919799/functions/poll.html
/// https://man7.org/linux/man-pages/man2/poll.2.html
/// https://pubs.opengroup.org/onlinepubs/009604599/functions/read.html
/// https://pubs.opengroup.org/onlinepubs/007904975/functions/write.html
/// https://pubs.opengroup.org/onlinepubs/007904875/functions/open.html
/// http://www.br-c.org/doku.php?id=lseek

#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

//Debounce parameters 
//Numbers can be tweaked according to each hardware specifications
#define DEBOUNCE_TIME       0.3
#define SAMPLE_FREQUENCY    10
#define MAXIMUM         (DEBOUNCE_TIME * SAMPLE_FREQUENCY)

//GPIO Led/Botoes
//Defina os Leds conforme as portas a serem utilizadas
#define Led1 43
#define Led2 44
#define Led_Start 89
#define Botao1 45
#define Botao2 46
#define Botao_Start 88

//Global variables:
void (*PointerToFunction)(); // Pointer to the functions (states) of the state machine. 
                             // It points to the function runs in the given time
int Player1_Points;          // Collects points from Player 1 and represents the index of tempo_1
int Player2_Points;          // Collects points from Player 2 and represents the index of tempo_2;
int tempo_1[10];             //Armazena[10] o tempo que o jogador 1 demorou para apertar o botao
int tempo_2[10];             //Armazena[10] o tempo que o jogador 2 demorou para apertar o botao
//Pressionar o botao = 1
// debounce integrator
unsigned int integrator;     // Will range from 0 to the specified MAXIMUM
//https://www.kennethkuhn.com/electronics/debounce.c for more information on the debouncer used in this program

//Function prototypes:
void Starting_State(void);       //function representing the initial state of the state machine
void Game_Running_State(void);   //function representing the state after Play Push-Button is pressed
void Player_1_Score_State(void); //function representing the state after Push-Button 1 is pressed
void Player_2_Score_State(void); //function representing the state after Push-Button 2 is pressed


//State functions
void Starting_State(void)
{
    ///Espera a entrada do Botao_Start, com uso de debounce code
    ///Concluindo a leitura do Botao_Start, vai para o proximo estado:
    ///  PointerToFunction = Game_Running_State;
    fprintf(stderr, "\n\n\t ----Starting_State----\n");  
    fprintf(stderr, "\nScore jogador 1: %d  | Score jogador 2: %d pontos",Player1_Points,Player2_Points);
    int value_int;
    char value_char[1] = "";
    int fd;
    // int n_value; //descomentar caso precise inverter os valores de input; o default ・de input = 1 caso o botao esteja pressionado    
    unsigned int output;      

    //Desligatodos os Leds  
    fd = open("/sys/class/gpio/gpio89/value",O_WRONLY);
    write(fd,"1",1);
    close(fd);    
    fd = open("/sys/class/gpio/gpio43/value",O_WRONLY);
    write(fd,"1",1);
    close(fd);
    fd = open("/sys/class/gpio/gpio44/value",O_WRONLY);
    write(fd,"1",1);
    close(fd);
    fprintf(stderr, "\nLeds desligados");  
    //Espera o botao ser acionado
    while(1){
        fd = open("/sys/class/gpio/gpio88/value",O_RDONLY);
        lseek(fd,0,SEEK_SET);
        read(fd,value_char,1);
        close(fd);
        value_int = atoi(value_char);
        if (value_int == 0)
        {
            if (integrator > 0)
                integrator--;
        }
        else if (integrator < MAXIMUM)
            integrator++;

        if (integrator == 0)
            output = 0;
        else if (integrator >= MAXIMUM)
        {
            output = 1;
            integrator = MAXIMUM;  /* defensive code if integrator got corrupted */
            break;
        }

    }
    if (output == 1)
    {
        fprintf(stderr, "\nStart acionado");  
        fprintf(stderr, "\n\n\t ----Game Running State----\n");  
        PointerToFunction = Game_Running_State;
    }

}

void Game_Running_State(void)
{
    /// Liga o Led de Play e espera pela entrada dos Botao1 e Botao2
    /// Caso o botao tenha sido pressionado antes da largada, indica-se a queimada de largada;
    /// Com o Botao acionado, leva ao proximo estado:
    ///     PointerToFunction = Player_X_Score_State;

    int fd;
    clock_t start,diff; //variavel para armazenar o tempo que levaram para apertar o botao
    char value_char1[1] ="";
    char value_char2[1] ="";
    int value_int1;
    int value_int2;

    ///Caso algum dos jogadores tenha pressionado antes do tempo
    ///o LED do respectivo jogador pisca com uma frequecia de 10Hz indicando a queimada de largada
    ///     retorna para o Starting_State

    //get_value(botao1);
    fd = open("/sys/class/gpio/gpio45/value",O_RDONLY);
    lseek(fd,0,SEEK_SET);
    read(fd,value_char1,1);
    close(fd);
    value_int1 = atoi(value_char1);
    if(value_int1) 
    {
        fprintf(stderr, "\nJogador 1 queimou");  
        for (int i = 0; i < 10; i++)
        {
            fd = open("/sys/class/gpio/gpio43/value",O_WRONLY);
            lseek(fd,0,SEEK_SET);
            write(fd,"0",1);
            close(fd);
            usleep(100000);
            fd = open("/sys/class/gpio/gpio43/value",O_WRONLY);
            lseek(fd,0,SEEK_SET);
            write(fd,"1",1);
            close(fd);
            usleep(100000);
        }
        
        PointerToFunction = Player_2_Score_State;
    }
    
    fd = open("sys/class/gpio/gpio46/value",O_RDONLY);
    lseek(fd,0,SEEK_SET);
    read(fd,value_char2,1);
    close(fd);
    value_int2 = atoi(value_char2);
    if(value_int2) 
    {
        fprintf(stderr, "\nJogador 2 queimou");  
        for (int i = 0; i < 10; i++)
        {
            fd = open("/sys/class/gpio/gpio44/value",O_WRONLY);
            lseek(fd,0,SEEK_SET);
            write(fd,"0",1);
            close(fd);
            usleep(100000);
            fd = open("/sys/class/gpio/gpio44/value",O_WRONLY);
            lseek(fd,0,SEEK_SET);
            write(fd,"1",1);
            close(fd);
            usleep(100000);
        }
        PointerToFunction = Player_1_Score_State;
    }
    if(value_int1 == value_int2)
    {
    	//desliga_led(0, Led_Start); //Liga o led do Start, indicando OK para os jogadores
    	fd = open("/sys/class/gpio/gpio89/value",O_WRONLY);
    	lseek(fd,0,SEEK_SET);
    	write(fd,"0",1);
    	close(fd);
    	fprintf(stderr, "\nLed Start ligado - Jogo comecou");  
    	start = clock();    //Comeca a amostragem de tempo

    	while(1)
    	{
        fd = open("/sys/class/gpio/gpio45/value",O_RDONLY);
        if(fd == -1) fprintf(stderr, "Erro leitura botao1");  
        lseek(fd,0,SEEK_SET);
        read(fd,value_char1,1);
        close(fd);
        value_int1 = atoi(value_char1);
        if(value_int1) 
        {   
          fprintf(stderr, "\nPonto jogador 1");  
          diff = 1000 * (clock() - start) / CLOCKS_PER_SEC; //Armazena o tempo, em ms, que levaram para apertar o botao 
          tempo_1[Player1_Points] = (int)diff; //Armazena o tempo do Player_1
          PointerToFunction = Player_1_Score_State;
          break;
        }

        fd = open("/sys/class/gpio/gpio46/value",O_RDONLY);
        if(fd == -1) fprintf(stderr, "Erro leitura botao2");  
        lseek(fd,0,SEEK_SET);
        read(fd,value_char2,1);
        close(fd);
        value_int2 = atoi(value_char2);
        if(value_int2) 
        {   
          fprintf(stderr, "\nPonto jogador 2");  
          diff = 1000 * (clock() - start) / CLOCKS_PER_SEC; //Armazena o tempo, em ms, que levaram para apertar o botao 
          tempo_2[Player2_Points] = (int)diff; //Armazena o tempo do Player_2
          PointerToFunction = Player_2_Score_State;
          break;
        }
      }
    }
}

void Player_1_Score_State(void)
{
    ///Adiciona ponto ao jogador X, liga o led do jogador X e volta para o estado inicial:
    ///  PointerToFunction = Starting_State;

    int fd;
    char value_char[1] = "";
    int value_int;
    unsigned int output;      
    Player1_Points++;
    fprintf(stderr, "\n\n\t ----Player 1 Score State----\n");  
    fprintf(stderr, "\nLed do jogador 1 acionado");  
    fprintf(stderr, "\nPontos adicionados ao jogador 1");  
    fd = open("sys/class/gpio/gpio43/value",O_WRONLY);
    lseek(fd,0,SEEK_SET);
    write(fd,"0",1);
    close(fd);
    fprintf(stderr, "\nEsperando reset");
    while(1)
    {
        fd = open("sys/class/gpio/gpio88/value",O_RDONLY);
        lseek(fd,0,SEEK_SET);
        read(fd,value_char,1);
        close(fd);
        value_int = atoi(value_char);
        if (value_int == 0)
        {
            if (integrator > 0)
                integrator--;
        }
        else if (integrator < MAXIMUM)
            integrator++;

        if (integrator == 0)
            output = 0;
        else if (integrator >= MAXIMUM)
        {
            output = 1;
            integrator = MAXIMUM;  /* defensive code if integrator got corrupted */
            break;
        }

    }
    if (output == 1)
    {
        fprintf(stderr, "\nReset acionado");  
        PointerToFunction = Starting_State;
    }
}

void Player_2_Score_State(void)
{
    ///Adiciona ponto ao jogador X, liga o led do jogador X e volta para o estado inicial:
    /// PointerToFunction = Starting_State;

    int fd;
    char value_char[1] = "";
    int value_int;
    unsigned int output;      
    Player2_Points++;
    fprintf(stderr, "\n\n\t ----Player 2 Score State----\n");  
    fprintf(stderr, "\nLed do jogador 2 acionado");  
    fprintf(stderr, "\nPontos adicionados ao jogador 2");   
    fd = open("sys/class/gpio/gpio44/value",O_WRONLY);
    lseek(fd,0,SEEK_SET);
    write(fd,"0",1);
    close(fd);
    fprintf(stderr, "\nEsperando reset");
    while(1){
        fd = open("sys/class/gpio/gpio88/value",O_RDONLY);
        lseek(fd,0,SEEK_SET);
        read(fd,value_char,1);
        close(fd);
        value_int = atoi(value_char);
        if (value_int == 0)
        {
            if (integrator > 0)
                integrator--;
        }
        else if (integrator < MAXIMUM)
            integrator++;

        if (integrator == 0)
            output = 0;
        else if (integrator >= MAXIMUM)
        {
            output = 1;
            integrator = MAXIMUM;  /* defensive code if integrator got corrupted */
            break;
        }

    }
    if (output == 1)
    {
        fprintf(stderr, "\nReset acionado");  
        PointerToFunction = Starting_State;
    }
}

int main(int argc, char *argv[])
{
    int fd;
    //Inicializacao de variveis globais e do PointerToFunction
    Player1_Points = 0;
    Player2_Points = 0;
    integrator = 0;
    PointerToFunction = Starting_State; //points to the initial state. 
                                        //Never forget to inform the initial state 
                                        //(otherwise, in this specific case, fatal error may occur/ hard fault).

    //Configura o Botao_Start: Checa export,direction=in,edge=rising

    fd = open("/sys/class/gpio/export", O_WRONLY);
    write(fd,"88",2);
    close(fd);

    fd = open("/sys/class/gpio/gpio88/direction", O_WRONLY);
    write(fd, "in", 2);
    close(fd);

    fd = open("/sys/class/gpio/gpio88/edge", O_WRONLY);
    write(fd, "rising", 6); 
    close(fd);
    //Configuracoes de botoes e leds

    // configura_led(Led1);
    fd = open("/sys/class/gpio/export", O_WRONLY);
    write(fd,"43",2);
    close(fd);

    fd = open("/sys/class/gpio/gpio43/direction", O_WRONLY);
    write(fd, "out", 2);
    close(fd);

    // configura_led(Led2);
    fd = open("/sys/class/gpio/export", O_WRONLY);
    write(fd,"44",2);
    close(fd);

    fd = open("/sys/class/gpio/gpio44/direction", O_WRONLY);
    write(fd, "out", 2);
    close(fd);


    // configura_botao(Botao1);
    fd = open("/sys/class/gpio/export", O_WRONLY);
    write(fd,"45",2);
    close(fd);

    fd = open("/sys/class/gpio/gpio45/direction", O_WRONLY);
    write(fd, "in", 2);
    close(fd);

    fd = open("/sys/class/gpio/gpio45/edge", O_WRONLY);
    write(fd, "rising", 6); 
    close(fd);

    // configura_botao(Botao2);
    fd = open("/sys/class/gpio/export", O_WRONLY);
    write(fd,"46",2);
    close(fd);

    fd = open("/sys/class/gpio/gpio46/direction", O_WRONLY);
    write(fd, "in", 2);
    close(fd);

    fd = open("/sys/class/gpio/gpio46/edge", O_WRONLY);
    write(fd, "rising", 6); 
    close(fd);

    // configura_led(Led_Start);
    fd = open("/sys/class/gpio/export", O_WRONLY);
    write(fd,"89",2);
    close(fd);

    fd = open("/sys/class/gpio/gpio89/direction", O_WRONLY);
    write(fd, "out", 2);
    close(fd);

    fprintf(stderr, "Programa inicializado");   
    while(1)
    {
        (*PointerToFunction)();  //calls a function pointed out by the pointer to function (thus, calls the current state)
    }
    system("PAUSE"); 
    return 0;
}
