#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>
#include  <signal.h>


int sockfd = 0, n = 0;
char recvBuff[1024];
struct sockaddr_in serv_addr;

void SIGhandler(int sig)
{
  signal(SIGUSR1, SIG_IGN);          /* ignore this signal       */
  send_message();
  signal(SIGUSR1, SIGhandler);     /* reinstall the handler    */
}

void send_message(){
    char buffer[256] = "blahblahblah";
    // printf("Please enter the message: ");
    /* Send message to the server */
    // bzero(buffer,256);
    // fgets(buffer,255,stdin);
    n = write(sockfd,buffer,strlen(buffer));
}

int main(void)
{
    signal(SIGUSR1, SIGhandler);
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Error : Could not create socket \n");
        return 1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(8886);
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        printf("\n Error : Connect Failed \n");
        return 1;
    }

    while (1) {
        memset(recvBuff, '0' , sizeof(recvBuff));
        while ((n = read(sockfd, recvBuff, sizeof(recvBuff) - 1)) > 0)
        {
            recvBuff[n] = 0;
            // printf("%d\n", n);
            if (fputs(recvBuff, stdout) == EOF)
            {
                printf("\n Error : Fputs error");
            }
        }

        if ( n < 0)
        {
            printf("\n Read Error \n");
            return 0;
        }
    }
    return 0;
}

