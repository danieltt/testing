#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <sys/time.h>
#define BUF_SIZE 1460

void error(const char *msg)
{
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[])
{
    int sockfd, portno, n, t_size, b_tx=0;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    struct timeval t1, t2;
    double elapsedTime;

    char buffer[BUF_SIZE];
    if (argc < 4) {
       fprintf(stderr,"usage %s hostname port size\n", argv[0]);
       exit(0);
    }
    portno = atoi(argv[2]);
    t_size= atoi(argv[3]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd < 0) 
        error("ERROR opening socket");
    server = gethostbyname(argv[1]);
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, 
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
    serv_addr.sin_port = htons(portno);
    bzero(buffer,BUF_SIZE);

    /* time stamps */
    gettimeofday(&t1, NULL);

    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
        error("ERROR connecting");

    while (b_tx<t_size){
    	n = write(sockfd,buffer,BUF_SIZE);
    	if (n < 0)
    		error("ERROR writing to socket");
    	b_tx+=n;
    }
    printf("%s\n",buffer);
    shutdown(sockfd,1);
    /*wait FIN from server*/
    n = read(sockfd,buffer,BUF_SIZE);
    if (n < 0) error("ERROR reading from socket");
    close(sockfd);
    /*time stamp end*/
    gettimeofday(&t2, NULL);

    elapsedTime = (t2.tv_sec - t1.tv_sec) * 1000.0;      // sec to ms
    elapsedTime += (t2.tv_usec - t1.tv_usec) / 1000.0;   // us to ms
    printf("Size: %d kB, Time: %f ms, BW: %f Mb/s\n",b_tx/1024, elapsedTime, b_tx*8/elapsedTime/1000);


    return 0;
}
