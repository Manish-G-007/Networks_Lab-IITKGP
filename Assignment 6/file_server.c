/****************************

G.Maneesh Kumar ,18CS10020

*****************************/

// We are creating ITERTIVE SERVER. So,we are keeping SERVER in INFINTE LOOP.
// And to stop it you need to use ctrl+z/+c and restart terminal.

// We are sending '\0' for checking empty file in client.

// Server side implementation of TCP client-server model 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>

#define MAX 100 
#define PORT 8080 
#define SA struct sockaddr 

// Colours for printing in terminal
#define RED   "\x1B[31m"
#define GRN   "\x1B[32m"
#define YEL   "\x1B[33m"
#define BLU   "\x1B[34m"
#define MAG   "\x1B[35m"
#define CYN   "\x1B[36m"
#define WHT   "\x1B[37m"
#define RESET "\x1B[0m"

int main() 
{ 
	int sockfd, connfd, len; 
	struct sockaddr_in servaddr, cli; 

	// socket create and verification 
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	printf("\n"); 
	if (sockfd == -1) 
	{ 
		perror(RED"Socket creation failed\n"RESET);
		exit(0); 
	} 
	else
		printf(GRN"Socket successfully created..\n"RESET); 
	memset(&servaddr, 0, sizeof(servaddr)); 

	// assign IP, PORT 
	servaddr.sin_family = AF_INET; 
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY); 
	servaddr.sin_port = htons(PORT); 

	// Binding newly created socket to given IP and verification 
	if ((bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0) { 
		printf(RED"Socket bind failed...\n"RESET); 
		exit(0); 
	} 
	else
		printf(GRN"Socket successfully binded..\n"RESET); 

	// Now server is ready to listen and verification 
	if ((listen(sockfd, 5)) != 0) { 
		printf(RED"Listen failed...\n"RESET); 
		exit(0); 
	} 
	else
		printf(GRN"Server listening..\n"); 
	len = sizeof(cli); 
    printf(MAG"\n------------------------- TCP Server is succesfully Stared -------------------- \n\n"RESET);
	// infinite loop
	for (;;) 
	{ 
	// Accept the data packet from client and verification 
		connfd = accept(sockfd, (SA*)&cli, &len); 
		if (connfd < 0) 
		{ 
			printf(RED"server acccept failed...\n"RESET);
			// continue; 
			// exit(0); 
		} 
		else
		{
			printf(YEL "server acccept the client...\n"RESET); 
			char buff[MAX]; 
			int n,sta,f=0; 
			memset(buff,0,sizeof(buff)); 
			// read the message from client and copy it in buffer 
			sta=recv(connfd, buff, sizeof(buff),0); 
			// print buffer which contains the client requested file 
			printf(CYN"File requested by client: %s\n"RESET, buff); 
		    int fd1 = open(buff, O_RDONLY); 
		    if (fd1 < 0)  
		    { 
		        perror("c1");
		        close(connfd); 
		        continue;
		    }
		    memset(buff,0,sizeof(buff));
		    // reading the file in chunks of size of buff. 
			while((n=read(fd1,buff,sizeof(buff))) !=0)
			{
				f++;
				sta=send(connfd, buff, n,0);
				memset(buff,0,sizeof(buff));
			}
			// If empty file then send '\0' to client 
			if(f==0)
			{
				char empf[1];
				empf[0]='\0';
				sta=send(connfd, empf, sizeof(buff),0);				
			}
			close(fd1);
			close(connfd);
		}
		printf("\n");  
	}

	close(sockfd); 
} 
