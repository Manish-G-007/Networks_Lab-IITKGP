/****************************

G.Maneesh Kumar ,18CS10020

*****************************/

// Client side implementation of TCP client-server model 


// We are receiving the data in B sized blocks(which are known to both client and server).

// we are ensuring that each time we are reading B sized block.
// Also we are using MSG_WAITALL flag for blocking the process until it gets B sized data.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h> 

#define MAX 1000
#define B 20 
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
	int sockfd, connfd; 
	struct sockaddr_in servaddr, cli; 

	// socket create and verification 
	sockfd = socket(AF_INET, SOCK_STREAM, 0); 
	printf("\n"); 
	if (sockfd == -1) 
	{ 
		perror(RED"Socket creation failed..."RESET);  
		exit(0); 
	} 
	else
	{
		printf(GRN"Socket successfully created..\n"RESET); 
	}

    memset(&servaddr, 0, sizeof(servaddr));
	// assign IP, PORT 
	servaddr.sin_family = AF_INET; 
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY); //inet_addr("127.0.0.1"); 
	servaddr.sin_port = htons(PORT); 

	// connect the client socket to server socket 
	if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr)) != 0) 
	{ 
		printf(RED"connection with the server failed...\n"RESET); 
		exit(0); 
	} 
	else
	{
		printf(MAG"connected to the server..\n"RESET); 
	}

	char buff[MAX]; 
	int sta;
	memset(buff,0,sizeof(buff));
    printf(BLU "\n Enter the file name : "RESET);
    scanf("%s",buff);
	sta=send(sockfd, buff, sizeof(buff),0); 

	int fd1 = open("response.txt",  O_WRONLY | O_CREAT | O_TRUNC, 0644);
	// Check whether open worked properly or not 
	if (fd1 < 0)  
	{ 
		printf(RED"\n\"Error in opening File\"\n"RESET);
	}
	else
	{   
		memset(buff,0,sizeof(buff));
		sta=recv(sockfd, buff, 1,0);
		int FSIZE;
		// checking whether file is present or not
		if(buff[0]=='E')
		{
			printf(RED"\n\"File Not Found\"\n"RESET);
			close(fd1);
		}
		else if(buff[0]=='L')
		{
			// receiving file size
			sta=recv(sockfd, &FSIZE, sizeof(FSIZE),0);
			int temp_fn=FSIZE/B,temp_fd=FSIZE%B,last=0,no_of_blocks=temp_fn;
			// receiving data in B sized blocks
			while (temp_fn--) 
			{ 
				memset(buff,0,sizeof(buff));		
				sta=recv(sockfd, buff, B,MSG_WAITALL);
				write(fd1,buff,sta);
				last=sta;
			}
			// receiving data of block size FSIZE%B
			if(temp_fd>0)
			{
				no_of_blocks++;
				memset(buff,0,sizeof(buff));
				last=recv(sockfd, buff,temp_fd,MSG_WAITALL);
				write(fd1,buff,last);				
			}
			close(fd1);
			printf(CYN"\nThe file transfer is successful. Total number of blocks received = %d, Last block size = %d\n"RESET,no_of_blocks,last);
			printf(GRN"\nClient Exit...\n\n"RESET);
		}
			
	}
	close(sockfd); 
} 
