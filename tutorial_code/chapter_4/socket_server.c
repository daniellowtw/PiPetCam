
#include "pipetcam.h"

int pid;

void doprocessing (int sock)
{
    int n;
    char buffer[256];
    bzero(buffer, 256);

    n = read(sock,buffer,255);
    if (n < 0)
    {
        perror("ERROR reading from socket");
    }

    if (strcmp(buffer, "background") == 0){
        capture_background();
        n = write(sock,"Doing Background",16);
    }
    else if (strcmp(buffer, "process") == 0){
        doProcessing();
        n = write(sock,"Doing Processing",16);
    }
    else{
        printf("%s %s\n", "OH dear, not one of the actions",buffer[0], buffer[1]);
        n = write(sock,"Action not recognised",21);
    }

    // printf("Here is the message: %s\n",buffer);
    if (n < 0) 
    {
        perror("ERROR writing to socket");
    }
}

void* start(void *parm )
{
    printf("%s\n", "SERVER RUNNING");
    int sockfd, newsockfd, portno, clilen;
    char buffer[256];
    struct sockaddr_in serv_addr, cli_addr;
    int  n;

    /* First call to socket() function */
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
    {
        perror("ERROR opening socket");
        return;
    }
    /* Initialize socket structure */
    bzero((char *) &serv_addr, sizeof(serv_addr));
    portno = 5001;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);
 
    /* Now bind the host address using bind() call.*/
    if (bind(sockfd, (struct sockaddr *) &serv_addr,
                          sizeof(serv_addr)) < 0)
    {
         perror("ERROR on binding");
         return;
    }
    /* Now start listening for the clients, here 
     * process will go in sleep mode and will wait 
     * for the incoming connection
     */
    listen(sockfd,5);
    clilen = sizeof(cli_addr);
    while (1) 
    {
        newsockfd = accept(sockfd, 
                (struct sockaddr *) &cli_addr, &clilen);
        if (newsockfd < 0)
        {
            perror("ERROR on accept");
            return;
        }
        /* Create child process */
        pid = fork();
        if (pid < 0)
        {
            perror("ERROR on fork");
	    return;
        }
        if (pid == 0)  
        {
            /* This is the client process */
            close(sockfd);
            doprocessing(newsockfd);
            return;
        }
        else
        {
            close(newsockfd);
        }
    } /* end of while */
}

void upload_coord(int x,int y){
    char str[256], buf[256];
    sprintf(buf, "curl -o /dev/null http://127.0.0.1:8887/addpoint?x=%d&y=%d", x, y);
    system(buf);
}

int main( int argc, char **argv[] )
{
    // printf("%c\n", argv[1]);
    if (argc == 2 && argv[1] == '1')
    {
            printf("%s\n", "DONE TOGGLE");
    }
            toggleVisual();

    pthread_t t1, t2 ; // declare 2 threads.
    // pthread_create( &t1, NULL, start,NULL); // create a thread running function1
    // pthread_create( &t2, NULL, start_main_program,NULL); // create a thread running function2
    start();
    start_main_program();
    // Because all created threads are terminated when main() finishes, we have
    // to give the threads some time to finish. Unfortunately for function1, main()
    // will give only 1 second, but function1 needs at least 2 seconds. So function1 will
    // probably be terminated before it can finish. This is a BAD way to manage threads.
    while(1){}

    return 0;
}
