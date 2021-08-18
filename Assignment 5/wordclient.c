/****************************

G.Maneesh Kumar ,18CS10020

*****************************/

// WE are Assuming that file always contain END keyword(AS in question it only says to check hello is present at beginning (or) not).

// Client side implementation of UDP client-server model 
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
    char word[] = "word0";
    char terminate[]="END";
    char *begining="HELLO";
    char *fnf="FILE_NOT_FOUND";
    char *request_file="text.txt";
    struct sockaddr_in   servaddr; 

    // Creating socket file descriptor 
    if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) 
    { 
        perror(RED"Socket creation failed"RESET); 
        exit(EXIT_FAILURE); 
    } 

    memset(&servaddr, 0, sizeof(servaddr)); 
    
    // Filling server information 
    servaddr.sin_family = AF_INET; 
    servaddr.sin_port = htons(PORT); 
    servaddr.sin_addr.s_addr = INADDR_ANY; 
    
    int n, len;
    // Requesting the file from server
    sendto(sockfd, (const char *)request_file, strlen(request_file), MSG_CONFIRM, (const struct sockaddr *) &servaddr, sizeof(servaddr)); 
    printf(YEL "--->File requested.\n"RESET);
    memset(buffer,0,strlen(buffer));
    n = recvfrom(sockfd, (char *)buffer, MAXLINE, MSG_WAITALL, (struct sockaddr *) &servaddr, &len);
    if(n==-1)
    {
        printf("Failed in receiving\n");
    }
    else
    {
        buffer[n] = '\0'; 
    } 
    // Checking whether file exist or not in server from messege received by server.(FILE_NOT_FOUND or FILE_OPENED)
    if(strcmp(buffer,fnf)==0) 
    {
             printf(RED "FILE_NOT_FOUND\n"RESET );
             exit(EXIT_FAILURE);
    } 
    else
    {
        FILE *fp;
        fp = fopen("response.txt", "w+");
        char *wordwf="WRONG_WORD_REQUEST";
        int flag=1;
        for(int i=0;;i++)
        {  
            word[4]=i+'0';
            // Sending wordi request to server
            sendto(sockfd, (const char *)word, strlen(word), MSG_CONFIRM, (const struct sockaddr *) &servaddr, sizeof(servaddr)); 
            memset(buffer,0,strlen(buffer));
            n = recvfrom(sockfd, (char *)buffer, MAXLINE, MSG_WAITALL, (struct sockaddr *) &servaddr, &len); 
            if(n==-1)
            {
                printf("failed in receiving\n");
            }
            else
            {
                buffer[n] = '\0'; 
            }
            // Checking the word request is correct or not.
            if(strcmp(buffer,wordwf)==0)
            {
                printf(RED "WRONG_WORD_REQUEST\n" RESET);
                flag=0;
                break;
            }
            // Checking whether the file is in correct format or not.
            if(strcmp(buffer,begining)!=0 && i==0)
            {
                printf(RED "WRONG_FILE_FORMAT\n"RESET );
                flag=0;
                break;
            }
            // End the requesting to server after we get END keyword in file.
            if(strcmp(buffer,terminate)==0)
            {
                 break;
            } 
            // Write into response.txt file apart from START and END keywords.
            if(i!=0)
            { 
               
               fprintf(fp,"%s\n", buffer);
            }
        }
        // If flag is 1 then all the data is received from server.
        if(flag==1)
        {
            printf(WHT "Total File Received from server successfully without loss in data.\n" RESET);
        }
        fclose(fp); // Closing file.
    }
    close(sockfd); // Closing the socket.
    return 0; 
} 
