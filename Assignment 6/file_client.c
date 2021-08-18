/****************************

G.Maneesh Kumar ,18CS10020

*****************************/

// We are using '\0' for checking empty file.

// Client side implementation of TCP client-server model 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
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

// checking for delimiter
int del_ch(char ch)
{
	if(ch == ' ' || ch == '\t' || ch == ':' || ch == ';' || ch == ',' || ch == '.' || ch == '\n')
		return 1;
	return 0;
}

int main() 
{ 
	int sockfd, connfd; 
	struct sockaddr_in servaddr, cli; 

	// socket create and verification 
	sockfd = socket(AF_INET, SOCK_STREAM, 0); 
	printf("\n"); 
	if (sockfd == -1) 
	{ 
		// printf("socket creation failed...\n");
		perror(RED"Socket creation failed..."RESET);  
		exit(0); 
	} 
	else
		printf(GRN"Socket successfully created..\n"RESET); 

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
		printf(MAG"connected to the server..\n"RESET); 

	char buff[MAX]; 
	int f=1,del_f=0,sta,no_of_words=0,no_of_bytes=0; 
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
		for (;;) 
		{ 
			memset(buff,0,sizeof(buff));
			sta=recv(sockfd, buff, sizeof(buff),0);
			if (sta == 0) 
			{ 
				printf(GRN"\nClient Exit...\n"RESET);
				// Check if server closed connection saying file not found
				if(f==1)
				{
					printf(RED"\n\"File Not Found\"\n"RESET);
					close(fd1);
					break; 
				}
				if(del_f==0)
				{
					no_of_words++;
				}
				close(fd1);
				printf(MAG"\nBytes: %d  Words: %d\n"RESET,no_of_bytes,no_of_words ); 
				break; 
			}
			// checking for empty file		
			if(buff[0]=='\0')
			{
				printf(CYN"\n\"-----EMPTY FILE---\"\n"RESET);
				close(fd1); 
				break; 				
			}
			write(fd1,buff,strlen(buff));
			no_of_bytes+=sta;

			// Finding no of words
			if ( !del_ch(buff[0]) )
			{ 
				del_f = 0;
			}
			int i=0,n=strlen(buff);
			 while(i < n)
			 {

				while( i < n && !del_ch(buff[i]) ) i++;
				if( i < n && del_f == 0) no_of_words += 1;

				del_f = 0; 

				while( i < n && del_ch(buff[i]) ) i++;
			 }

			 if (del_ch(buff[i-1]))
			 { 
				del_f = 1;
			 }

			f=0;
		}
			
	}
	close(sockfd); 
} 
