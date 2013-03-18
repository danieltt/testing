/* A simple server in the internet domain using TCP
   The port number is passed as an argument */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#define BUF_SIZE 1460
void error(const char *msg)
{
    perror(msg);
    exit(1);
}


int main(int argc, char *argv[])
{
     int sockfd, newsockfd, portno,b_rx;
     socklen_t clilen;
     struct timeval t1, t2;
     double elapsedTime;
     char buffer[BUF_SIZE];
     struct sockaddr_in serv_addr, cli_addr;
     int n;
     if (argc < 2) {
         fprintf(stderr,"ERROR, no port provided\n");
         exit(1);
     }
     sockfd = socket(AF_INET, SOCK_STREAM, 0);
     if (sockfd < 0) 
        error("ERROR opening socket");
     bzero((char *) &serv_addr, sizeof(serv_addr));
     portno = atoi(argv[1]);
     serv_addr.sin_family = AF_INET;
     serv_addr.sin_addr.s_addr = INADDR_ANY;
     serv_addr.sin_port = htons(portno);
     if (bind(sockfd, (struct sockaddr *) &serv_addr,
              sizeof(serv_addr)) < 0) 
              error("ERROR on binding");
     listen(sockfd,5);
     clilen = sizeof(cli_addr);
     while(1){
    	  /* time stamps */
    	 b_rx=0;


    	 newsockfd = accept(sockfd,
    			 (struct sockaddr *) &cli_addr,
    			 &clilen);
    	 gettimeofday(&t1, NULL);
    	 if (newsockfd < 0)
    		 error("ERROR on accept");
    	 bzero(buffer,1);
    	 do{
    		 n = read(newsockfd,buffer,BUF_SIZE);
    		 b_rx+=n;
    		 if (n < 0) error("ERROR reading from socket");
    		 n = write(newsockfd,buffer,n);
    	 }while (n > 0);
    	 close(newsockfd);
    	 /*time stamp end*/
    	 gettimeofday(&t2, NULL);

    	 elapsedTime = (t2.tv_sec - t1.tv_sec) * 1000.0;      // sec to ms
    	 elapsedTime += (t2.tv_usec - t1.tv_usec) / 1000.0;   // us to ms
    	 printf("Size: %d kB, Time: %f ms, BW: %f Mb/s\n",b_rx/1024, elapsedTime, b_rx*8/elapsedTime/1000);
     }
     close(sockfd);
     return 0; 
}

