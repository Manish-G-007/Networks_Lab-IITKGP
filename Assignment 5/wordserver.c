/****************************

G.Maneesh Kumar ,18CS10020

*****************************/

// We are creating ITERTIVE SERVER. So,we are keeping SERVER in INFINTE LOOP.
// And to stop it you need to use ctrl+z and restart terminal.

// WE are Assuming that file always contain END keyword(AS in question it only says to check hello is present at beginning (or) not).



// Server side implementation of UDP client-server model 
#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h> 
// The header file in.h contains constants and structures needed for internet domain addresses.   

#define PORT     8080 
#define MAXLINE 1024 
  
// Colours for printing in terminal
#define RED   "\x1B[31m"
#define GRN   "\x1B[32m"
#define YEL   "\x1B[33m"
#define BLU   "\x1B[34m"
#define MAG   "\x1B[35m"
#define CYN   "\x1B[36m"
#define WHT   "\x1B[37m"
#define RESET "\x1B[0m"

// Driver code 
int main() 
{ 
    int sockfd; // File descriptors
    char buffer[MAXLINE]; // To store the received messege 
    struct sockaddr_in servaddr, cliaddr; 
      
    // Creating socket file descriptor 
    if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) 
    { 
    	perror(RED"socket creation failed"RESET); 
        exit(EXIT_FAILURE); 
    } 
      
    memset(&servaddr, 0, sizeof(servaddr)); 
    memset(&cliaddr, 0, sizeof(cliaddr)); 
      
    // Filling server information 
    servaddr.sin_family    = AF_INET; // IPv4 
    servaddr.sin_addr.s_addr = INADDR_ANY; 
    servaddr.sin_port = htons(PORT); 
      
    // Bind the socket with the server address 
    if ( bind(sockfd, (const struct sockaddr *)&servaddr,sizeof(servaddr)) < 0 ) 
    { 
        perror(RED"bind failed"RESET); 
        exit(EXIT_FAILURE); 
    } 
      
    int len, n; 
  
    len = sizeof(cliaddr);  //len is value/result used in recvfrom
  	printf(MAG"\n------------------------- Server is succesfully Stared ----------------------- \n\n"RESET);
    while(1)
    {
		memset(buffer,0,strlen(buffer)); // Making buffer empty.
		n = recvfrom(sockfd, (char *)buffer, MAXLINE,MSG_WAITALL, ( struct sockaddr *) &cliaddr,&len);
        if(n==-1) // Check whether received (or) not
        {
            printf("failed in receiving\n");
        }
        else
        {
            buffer[n] = '\0'; 
        } 
	  	printf(YEL "\nClient requested file :%s\n" RESET,buffer);
		FILE *fp;
		fp = fopen(buffer, "r+");
		// Check whether file exist in local directory or not
	  	if(fp==NULL)
	  	{
	  		char *fnf="FILE_NOT_FOUND";
	  		perror(RED"\nFILE_NOT_FOUND"RESET);
	  		// If file doesn't exists sending  FILE_NOT_FOUND
	    	sendto(sockfd, (const char *)fnf, strlen(fnf),MSG_CONFIRM, (const struct sockaddr *) &cliaddr, len);  
	    }
	    else
	    {
	    	char *fop="FILE_OPENED";
	    	int flag=1;
	    	// If file exists sending  FILE_OPENED
	    	sendto(sockfd, (const char *)fop, strlen(fop),MSG_CONFIRM, (const struct sockaddr *) &cliaddr, len); // it is for 78 line mapping that if condition is not true then also client is still pause in receiving.
		    char dataToBeRead[1024];
		    int i=0;
	    	char word[] = "word0";
	    	char *begining="HELLO";
	    	// Reading File line by line.
		    while(fgets ( dataToBeRead, 1024, fp ) != NULL ) 
		     { 
		     	int loo = strlen(dataToBeRead);
				if (dataToBeRead[loo - 1] == '\n')
				{
				   // Parsing the string by removing the end line.
				   dataToBeRead[loo - 1] = '\0';
				}
			    memset(buffer,0,strlen(buffer));
				n = recvfrom(sockfd, (char *)buffer, MAXLINE,MSG_WAITALL, ( struct sockaddr *) &cliaddr,&len);
            	if(n==-1)
            	{
            	    printf("Failed in receiving\n");
            	}
           		 else
            	{
                	buffer[n] = '\0'; 
           		}
				word[4]=i+'0';
				// Checking whether wordi request from client is same as the present read word line or not.
				if(strcmp(buffer,word)==0)
				{
		        	sendto(sockfd, (const char *)dataToBeRead, strlen(dataToBeRead),MSG_CONFIRM, (const struct sockaddr *) &cliaddr, len);
		        	if(strcmp(dataToBeRead,begining)!=0 && i==0)
	            	{
	            		// Checking whether the file is in correct format or not.
	                	printf(RED "WRONG_FILE_FORMAT\n"RESET );
	                	flag=0;
	                	break;
	           		}  
		     	}
		     	else
		     	{
	  				char *wordwf="WRONG_WORD_REQUEST";
	  				printf(RED "WRONG_WORD_REQUEST\n" RESET);
	    			sendto(sockfd, (const char *)wordwf, strlen(wordwf),MSG_CONFIRM, (const struct sockaddr *) &cliaddr, len);  
		     		flag=0;
		     		break;
		     	}

		     	i++;
		     }
		     if(flag==1)
		     {
		     	printf(CYN "File sent from server successfully.\n" RESET);
		     } 
		          
		    fclose(fp); // Closing file.
	    }
	}
     close(sockfd); // Closing the socket.
    return 0; 
} 