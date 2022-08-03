/* poll_input.c

    Licensed under GNU Gener/*al Public License v2 or later.
*/
#include <poll.h>
#include <fcntl.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>


#define Botao1 3
#define Botao2 4
#define Botao_Start 88

int main(int argc, char *argv[])
{
    while(1)
    {
        char path[50];
        int fd;
        int ret;
        int nfds, num_open_fds;
        struct pollfd *pfds;
        char buf[30] = "";
        num_open_fds = nfds = argc - 1;
        pfds = calloc(nfds, sizeof(struct pollfd));
        for (int j = 0; j < nfds; j++) 
        {
            pfds[j].fd = open(argv[j + 1], O_RDONLY);
            printf("Opened \"%s\" on fd %d\n", argv[j + 1], pfds[j].fd);

            pfds[j].events = POLLIN;
        }

        /* Keep calling poll() as long as at least one file descriptor is
            open. */

        while (num_open_fds > 0) 
        {
            int ready;

            printf("About to poll()\n");
            ready = poll(pfds, nfds, -1);
            printf("Ready: %d\n", ready);

            /* Deal with array returned by poll(). */

            for (int j = 0; j < nfds; j++) 
            {
                if (pfds[j].revents != 0) 
                {
                    if (pfds[j].revents & POLLIN) 
                    {
                        ssize_t s = read(pfds[j].fd, buf, sizeof(buf));
                        fprintf(stderr, "\nbuf:%s",buf);
                        if(!strcmp(buf,"botao_1\n"))
                        {
                            fd = open("sys/class/gpio/gpio45/value",O_WRONLY);
                            if(fd == -1) fprintf(stderr, "\nErro leitura botao1");  
                            lseek(fd,0,SEEK_SET);
                            fprintf(stderr, "\nBotao 1 toggle fd = %d",fd); 
                            write(fd,"1",1);
                            usleep(300000);
                            lseek(fd,0,SEEK_SET);
                            write(fd,"0",1);
                            fprintf(stderr, "\nBotao 1 desligado fd = %d",fd);
                            close(fd);
                        }
                        else if(!strcmp(buf,"press_1\n"))
                        {
                            fd = open("sys/class/gpio/gpio45/value",O_WRONLY);
                            if(fd == -1) fprintf(stderr, "Erro leitura botao1");  
                            lseek(fd,0,SEEK_SET);
                            printf("Botao 1 pressionado fd = %d\n",fd); 
                            write(fd,"1",1);
                            close(fd);
                        }
                        else if(!strcmp(buf,"soltar_1\n"))
                        {
                            fd = open("sys/class/gpio/gpio45/value",O_WRONLY);
                            if(fd == -1) fprintf(stderr, "Erro leitura botao1");  
                            lseek(fd,0,SEEK_SET);
                            printf("Botao 1 solto fd = %d\n",fd); 
                            write(fd,"0",1);
                            close(fd);
                        }
                        else if(!strcmp(buf,"botao_2\n"))
                        {
                            fd = open("sys/class/gpio/gpio46/value",O_WRONLY);
                            if(fd == -1) fprintf(stderr, "Erro leitura botao2");  
                            lseek(fd,0,SEEK_SET);
                            printf("Botao 2 toggle fd = %d\n",fd);
                            write(fd,"1",1);
                            usleep(300000);
                            lseek(fd,0,SEEK_SET);
                            write(fd,"0",1);
                            close(fd);
                        }
                        else if(!strcmp(buf,"press_2\n"))
                        {
                            fd = open("sys/class/gpio/gpio46/value",O_WRONLY);
                            if(fd == -1) fprintf(stderr, "Erro leitura botao2");  
                            lseek(fd,0,SEEK_SET);
                            printf("Botao 2 pressionado fd = %d\n",fd); 
                            write(fd,"1",1);
                            close(fd);
                        }
                        else if(!strcmp(buf,"soltar_2\n"))
                        {
                            fd = open("sys/class/gpio/gpio46/value",O_WRONLY);
                            if(fd == -1) fprintf(stderr, "Erro leitura botao2");  
                            lseek(fd,0,SEEK_SET);
                            printf("Botao 2 solto fd = %d\n",fd); 
                            write(fd,"0",1);
                            close(fd);
                        }
                        else if(!strcmp(buf,"botao_start\n"))
                        {
                            fd = open("sys/class/gpio/gpio88/value",O_WRONLY);
                            lseek(fd,0,SEEK_SET);
                            printf("Botao Start acionado fd = %d\n",fd);
                            write(fd,"1",1);
                            usleep(300000);
                            lseek(fd,0,SEEK_SET);
                            write(fd,"0",1);
                            close(fd);
                        }
                    } 
                    else 
                    {                /* POLLERR | POLLHUP */
                        printf("    closing fd %d\n", pfds[j].fd);
                        num_open_fds--;
                    }
                }
            }
        }

    }
}
