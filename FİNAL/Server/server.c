/******************************************/
/*      121044055                         */
/*    Merve SAKOGLU                       */
/*  System Programming Final              */
/*                                        */
/******************************************/
#include<stdlib.h>
#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<string.h>
#include<unistd.h>
#include<unistd.h>
#include<time.h>
#include<signal.h>

#define MAX 4000
#define MAX_CLIENT 10

int client_accept;

void usage();
void help_function();
void handle(int sig);
void listServer_function(int port,char argv[MAX],int client_accept);
int accept_function(int server_sock);
int socket_function(int port_number);

int main (int argc, char **argv)
{
 	int client_accept,number,server_socket;
	pid_t pid;
 	socklen_t size_clientr;
 	char buffer[MAX];
	char *temp1,*temp2;
	int port;
	FILE *input;
	char port_number[MAX];
	int data;
	time_t currentTime;
 	struct sockaddr_in clientr,serverr;

 	if (argc != 2){						
 	 	usage();
 		exit(1);
 	}
 	/*SİGNAL*/
	signal(SIGPIPE, SIG_IGN);
        signal(SIGINT, handle);

	server_socket = socket(AF_INET , SOCK_STREAM ,0);
	if (server_socket == -1)
    	{
        	perror("Could not create socket!!!");
        }

 	serverr.sin_family = AF_INET;
 	serverr.sin_addr.s_addr = htonl(INADDR_ANY);
 	if(atoi(argv[1]) <= 1024){
		perror("Port number must be greater than 1024");
		exit(2);
 	}
 	serverr.sin_port = htons(atoi(argv[1]));
	
	if( bind(server_socket,(struct sockaddr *)&serverr , sizeof(serverr)) < 0)
    	{
		perror("Bind Error!!!");
		return 1;
    	}

 	listen (server_socket, MAX_CLIENT);
	printf("waiting for connections...\n");

 	while(1){
		currentTime = time( NULL );
		fputs (ctime (&currentTime), stdout);
		size_clientr = sizeof(clientr);
  
		client_accept = accept(server_socket, (struct sockaddr *)&clientr, (socklen_t*)&size_clientr);

		if (client_accept < 0)
	        {
			perror("Client accept failed!!!");
			return 1;
		}
		printf("Connection accepted...\n");
		
  		/*
		*Child created for client requests
		*/

  		if ((pid = fork ()) == 0 ){
			close (server_socket);
			port = 1024;				
  			while ((number = recv(client_accept, buffer, MAX,0)) > 0)  {
   
				   temp1 = buffer;
				   temp2 = strtok(temp1," ");

				   if(strcmp("help\n",buffer)==0){
					
					help_function();

				   }if(strcmp("listServer\n",buffer)==0){
					
					listServer_function(port, argv[1],client_accept);
	
   				   }if(strcmp("lsClients\n",buffer)==0){

				   }
			
			}
	       } 
}

 	//close(client_accept);
}
void help_function(){

	printf("============================================\n");
	printf("HOW TO USE THE MULTİPLE FİLE SHARİNG SERVER\n");
	printf("============================================\n\n");
	printf("listLocal\n");
	printf("=========\n");
	printf("* To list the local files in the directory client program started\n");
	printf("listServer\n");
	printf("=========\n");
	printf("* To list the files in the current scope of the server-client\n");
	printf("lsClients\n");
	printf("=========\n");
	printf("* To lists the clients currently connected to the server with their respective clientid\n");
	printf("sendFile <filename> <clientid>\n");
	printf("=========\n");
	printf("* Send the file <filename> (if file exits from local directory to the client with client");
	printf("id clientid.If no client id is given the file is send to the servers local directory.\n");
	printf("SAMPLE TEST\n");
	printf("============\n\n");
	usage();

}
void listServer_function(int port,char argv[MAX],int client_accept){
	int data;
	FILE *input;
	char temp[MAX];
	char port_number[MAX];
			
	port=port+1;
	if(port==atoi(argv)){
		port=port+1;
	}
	sprintf(port_number,"%d",port);
	data = socket_function(port);			
	send(client_accept, port_number,MAX,0);				
	data = accept_function(data);	
 			
	input = popen("ls", "r");
				
	while(fgets(temp, sizeof(temp), input)!=NULL){
		send(data,"1",MAX,0);
		send(data, temp, MAX, 0);
		
	}
	send(data,"0",MAX,0);
	//pclose(input);

}
int socket_function(int port_number)
{
	struct sockaddr_in serverr;
	int client_socket;

	client_socket = socket(AF_INET , SOCK_STREAM , 0);
	if (client_socket == -1)
    	{
        	perror("Could not create socket!!!\n");
        }

	memset(&serverr, 0, sizeof(serverr));
	serverr.sin_family = AF_INET;
 	serverr.sin_addr.s_addr= htonl(INADDR_ANY);;
 	serverr.sin_port =  htons(port_number); 

	if ((bind (client_socket, (struct sockaddr *) &serverr, sizeof(serverr))) < 0){
		perror("Socket error!!!");
		exit (0);
	}
	listen(client_socket, 1);

	return client_socket;
}
int accept_function(int server_sock)
{
	socklen_t size;
	int data_accept;
	struct sockaddr_in datacliaddr;

	size = sizeof(datacliaddr);
	if ((data_accept = accept (server_sock, (struct sockaddr *) &size, &size)) < 0){
		perror("Accept error!!!");
		exit (0);
	}

	return(data_accept);
}
void handle(int sig){

    printf("^^^Ctrl + C (SIGINT) CATCH^^^\n");
    close(client_accept);
    signal(SIGINT, SIG_DFL);
    kill(getpid(),SIGINT);

    exit (sig);
}
void usage(){
	printf("\n------------\n");
	printf("Usage : \n");
	printf("make\n");
	printf("cd Server\n");
	printf("./server <port number>\n");
	printf("\n------------\n");
}
 


