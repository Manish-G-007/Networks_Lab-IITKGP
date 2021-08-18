/****************************

G.Maneesh Kumar ,18CS10020

*****************************/

#include <bits/stdc++.h>
#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>


#define MAXLINE 1024	// Maximum size we can send
#define TIMEOUT 120		// time-out value 120 seconds

// Colours for printing in terminal
#define RED   "\x1B[31m"
#define GRN   "\x1B[32m"
#define YEL   "\x1B[33m"
#define BLU   "\x1B[34m"
#define MAG   "\x1B[35m"
#define CYN   "\x1B[36m"
#define WHT   "\x1B[37m"
#define RESET "\x1B[0m"


using namespace std;

typedef struct user_detail
{
    struct sockaddr_in sa; 		 // structure for storing info of ip,port,family..,etc.
    int socket_fd;      		 // socket used to chat with a particular user
    long last_interaction_time;  // last interaction time chat with this user
}user_detail;


/********************************************************************************************

We have used property that no two users can have same username in the peer to peer chat app.

so, we defined a hash with username as identifying key for identifying the ip info of user.

********************************************************************************************/
unordered_map<string,struct user_detail>user_info; 


// returns the username if exists in the user_info otherwise return "NOT_USER" 
string check_user(struct sockaddr_in client,int port)
{
	char query[INET_ADDRSTRLEN];
	inet_ntop(AF_INET, (struct sockaddr*) &(client.sin_addr), query, INET_ADDRSTRLEN);
    for(auto itr=user_info.begin();itr!=user_info.end();itr++)
    {
        char ip4[INET_ADDRSTRLEN];  // space to hold the IPv4 string
        inet_ntop(AF_INET, &((itr->second).sa.sin_addr), ip4, INET_ADDRSTRLEN);

        if(!strcmp(ip4,query)  && port==ntohs((itr->second).sa.sin_port))
        {
        		return itr->first;
        }
    }
    return "NOT_USER";
}


// thread-function for checking timeout duration value of user
void *check_timeout(void *param)
{
    int i;
    while(1)
    {
        for(auto itr=user_info.begin();itr!=user_info.end();itr++)
        {
            if((itr->second).socket_fd != -1)
            {
                if((time(NULL)-(itr->second).last_interaction_time)>TIMEOUT)
                {
                    cout<<(itr->first);
                    printf(" timed-out. Closing connection ...\n");
                    close((itr->second).socket_fd);
                    (itr->second).socket_fd = -1;
                }
            }
        }
    }
    pthread_exit(0);
}

int main(int argc, char **argv)
{
    if (argc != 2) 
    {
        printf("usage: %s <port>\n", argv[0]);
        exit(1);
    }
    int PORT = atoi(argv[1]),no_users;

    // Reading the user info from user_in.txt
    FILE *fp;
    fp = fopen("user_in.txt","r");
    fscanf(fp,"%d\n",&no_users);
    struct sockaddr_in temp;
    printf("\n -----------------\n");
    printf("| User Info table |\n");
    printf(" ------------------\n");
    for(int i=0;i<no_users;i++)
    {
        char name_temp[1000];
        char cstr[1000];
        int temp_port;
        fscanf(fp,"%s",name_temp);
        fscanf(fp,"%s",cstr);
        fscanf(fp,"%d",&(temp_port));
        printf("%s\t%s\t%d\n",name_temp,cstr,temp_port);
        string ft=name_temp;
        bzero(&temp, sizeof(temp));
        temp.sin_family = AF_INET;
        inet_pton(AF_INET, cstr, &(temp.sin_addr));
        temp.sin_port = htons(temp_port);
        user_info[ft].sa=temp;
        user_info[ft].socket_fd = -1;
    }

    fclose(fp);
    cout<<endl<<endl<<endl;


	pthread_t tid;

    int listenfd,connfd,nready,maxfdp1,connd_temp;
    char buffer[MAXLINE];
    socklen_t len;
    struct sockaddr_in cliaddr, servaddr,temp_clieaddr;
    char message[MAXLINE];
  
    /* create listening TCP socket */
    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    printf("\n"); 
    if (listenfd == -1) 
    { 
        perror(RED " Socket creation failed\n " RESET);
        exit(0); 
    } 
    else
    {
        printf(GRN "  Socket successfully created..\n " RESET);
    }
    int optval = 1;
  	setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,(const void *)&optval , sizeof(int));
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(PORT);


    // Binding newly created socket to given IP and verification 
    if (bind(listenfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) != 0) 
    { 
        printf(RED " Socket bind failed...\n " RESET); 
        exit(0); 
    } 
    else
    {
        printf(GRN " Socket successfully binded..\n " RESET); 
    }

    // Now server is ready to listen and verification 
    if (( listen(listenfd, 10)) != 0) 
    { 
        printf(RED " Listen failed...\n " RESET); 
        exit(0); 
    } 
    else
    {
        printf(GRN " Server listening..\n " RESET); 
    }

    fd_set rfds,currfds;
    struct timeval tv;
    tv.tv_sec = 43200;
    tv.tv_usec = 0;

    // clear the descriptor set
    FD_ZERO(&rfds);
    FD_SET(listenfd, &rfds);
    FD_SET(STDIN_FILENO, &rfds);
    // get maxfd
    maxfdp1 = listenfd + 1;

    printf(MAG " \n-------- P2P chat is succesfully Started!! enjoy with chat ------ \n\n" RESET);

    bool present=false;
    for(auto itr=user_info.begin();itr!=user_info.end();itr++)
    {
        if(ntohs((itr->second).sa.sin_port) == PORT)
        {
             cout<<"\033[1;3m"<<"Welcome "<<itr->first<<" .You can start chatting "<<"\033[0m"<<endl;
             present=true;
        }
    }
    if(!present)
    {
    	printf("\nUsername found in user_in.txt\n");
    	exit(1);
    }
    printf(RED " \nEnter 'quit' to exit.\n " RESET);
    printf(CYN " \nMessage format: username/<msg>\n " RESET);
    printf("\n");
    pthread_create(&tid,NULL,check_timeout,NULL);
    
    while(1)
    {
  
        // select the ready descriptor
        nready = select(maxfdp1, &rfds, NULL, NULL, &tv);

        if (nready == -1) 
        {
                perror("select"); // error occurred in select()
                break;
        } 
        else if (nready == 0) // timeout for the server
        {
                printf("Timeout occurred! Usage time > 12 hrs not allowed.\n");
                break;
        }  
        else 
        {
                if (FD_ISSET(listenfd, &rfds)) 
                {
                    len = sizeof(cliaddr);
                    connfd = accept(listenfd, (struct sockaddr*)&cliaddr, &len); // accepting the connection
                    int nport;
                    read(connfd,&nport,sizeof(nport));                           // getting port for identifying the user 
                    string index = check_user(cliaddr,nport);
                    if(index=="NOT_USER")
                    {
                        printf("\n Unknown user attempting to connect...denied\n");
                        close(connfd);
                    }
                    else
                    {
                        user_info[index].last_interaction_time = time(NULL);       // initialise last interaction time
                        user_info[index].socket_fd = connfd;                      // Add this file descriptor to user_info 
                    }
                }

                else if(FD_ISSET(STDIN_FILENO, &rfds))
                {
                    bzero(buffer, sizeof(buffer));

                    int temlen=-1;
					do
					{
						if(temlen<MAXLINE)
						{
							read(STDIN_FILENO,&buffer[++temlen],1);				// read from stdin
						}
						else break;
					}while(buffer[temlen]!='\n');

					buffer[temlen+1]='\0';
					if(strcmp(buffer,"quit\n")==0)
					{															// if user enters quit then exit
						printf("bye!\n");
						close(listenfd);
						exit(0);
					}

                    int xy=0,z=0,flag=0;
                    string name_u="";
                    while(xy<strlen(buffer) && buffer[xy]!='/')
                    {
                        name_u+=buffer[xy++];
                    }
                    xy++;

                    auto itre=user_info.find(name_u);
                    if(itre==user_info.end())
                    {
                        printf("\n User not in user_in.txt list.\n\n");
                    }
                    else
                    {          
	                        if(user_info[name_u].socket_fd==-1)
	                        {

	                            connd_temp = socket(AF_INET, SOCK_STREAM, 0);
	                            user_info[name_u].last_interaction_time = time(NULL);         // update latest interaction time with user   
	                            user_info[name_u].socket_fd = connd_temp;					  // add this to user_info

	                            len = sizeof(user_info[name_u].sa);
	                            if(connect(connd_temp, (struct sockaddr*)&(user_info[name_u].sa), len)<0)
	                            {
	                                 printf("\nERROR connecting!! User is not online\n");   
	                            }

								    int nteport=PORT;
	                                write(connd_temp,&nteport,sizeof(nteport));
	                        }
	                        char user_ip_add[INET_ADDRSTRLEN];
							inet_ntop(AF_INET, (struct sockaddr*) &(user_info[name_u].sa.sin_addr), user_ip_add, INET_ADDRSTRLEN);
	                        cout<<YEL<<"Messege send to "<<name_u<<" with ip:"<< user_ip_add<<RESET;
	                        cout<<" \n\n";
	                        bzero(message, sizeof(message));
	                        z=0;
	                        while(xy<strlen(buffer))
	                        {
	                            message[z++]=buffer[xy++];
	                        }
	                        message[z]='\0';
	                        
	                        int len1=-1;
							do
							{
								write( user_info[name_u].socket_fd,&message[++len1],1);	// send msg to user
							}while(message[len1]!='\0');
                    }
                }

                else
                {

                   for(auto itr=user_info.begin();itr!=user_info.end();itr++)
        			{
                        // find which user who sent the msg
                        if(FD_ISSET((itr->second).socket_fd,&rfds))
                        {   

                            bzero(buffer, sizeof(buffer));

                            string temp_write;
                            (itr->second).last_interaction_time = time(NULL);				// update last interaction time with user

							int len=-1,b_read=0;
							do{
								b_read = read((itr->second).socket_fd,&buffer[++len],1);	// read the msg
							}while(buffer[len]!='\0');

							if(b_read==0)
							{																// when connection is closed 0 bytes will be read
								cout<<(itr->first)<<" connection closed \n";
								(itr->second).socket_fd = -1;
							}
							else
							{
								cout<<endl<<(itr->first)<<"  : "<<buffer<<endl;           		// Output the sender and message 
							}
							break;
                        }
                    }
                }       
        }

        //select is destroying so,again intialize all the active socketfd's
        FD_ZERO(&rfds);
        FD_SET(STDIN_FILENO,&rfds);
        FD_SET(listenfd,&rfds);

        for(auto itr=user_info.begin();itr!=user_info.end();itr++)
        {
            if((itr->second).socket_fd != -1)
            {
                 FD_SET((itr->second).socket_fd,&rfds);
                 maxfdp1=max(maxfdp1,((itr->second).socket_fd)+1);
            }
        }
    }

    // join the previous thread so that after all connections are lost then our user will exit();
    pthread_join(tid,NULL);
            
        return 0;
}