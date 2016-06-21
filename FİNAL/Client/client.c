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
#include<arpa/inet.h>
#include<unistd.h>
#include<time.h>
#include<signal.h>
#include<netdb.h>
#include<netinet/in.h>

#define MAXROW 4000

int client_socket;

void usage();
void help_function();
void handle(int sig);
void listServer(int client_socket,char argv[MAXROW],int create_sock,char control[MAXROW]);
int socket_function(int port,char *arr);

int main(int argc, char **argv)
{
	 int client_socket,byte;
	 struct sockaddr_in serverr;
	 char *temp1,*temp2;
	 int port1,create_sock;
	 char control[MAXROW]="1", files[MAXROW];
	 char port[MAXROW];
	 pid_t pid;
	 char sendCommand[MAXROW], recvline[MAXROW];

	 if (argc != 3) {
	 	usage();
	  	exit (0);
	 }
	/*SİGNAL*/
	signal(SIGPIPE, SIG_IGN);
        signal(SIGINT, handle);
	 /*
	 *Create a socket for client
	 */
	
	client_socket = socket(AF_INET , SOCK_STREAM , 0);
	if (client_socket == -1)
    	{
        	perror("Could not create socket!!!\n");
		exit (0);
        }
	memset(&serverr, 0, sizeof(serverr));
	serverr.sin_family = AF_INET;
 	serverr.sin_addr.s_addr= inet_addr(argv[1]);
 	serverr.sin_port =  htons(atoi(argv[2])); 

	if (connect(client_socket , (struct sockaddr *)&serverr , sizeof(serverr)) < 0)
        {
		perror("Connect error");
		exit (0);
        }
	
	
	printf("Enter the command :");
	
	 while (fgets(sendCommand, MAXROW, stdin) != NULL) {
		
		/*Uzaktan baglanan bilgisayar aldıgım komutu gonderdim*/
	  	send(client_socket, sendCommand, MAXROW, 0);
	  	
		temp2 = sendCommand;
	  	temp1 = strtok(temp2," ");
	   
		if(strcmp("help\n",sendCommand)==0){

			help_function();

		}if(strcmp("listServer\n",sendCommand)==0){

			listServer(client_socket,argv[1], create_sock, control);
	
		}if (strcmp("listLocal\n",sendCommand)==0){

		   	system("ls");
			printf("\n");
	        }if(strcmp("lsClients\n",sendCommand)==0){


		}
	 }

	 
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
	printf("id clientid.If no client id is given the file is send to the servers local directory.\n\n");
	printf("SAMPLE TEST\n");
	printf("============\n\n");
	usage();

}
void listServer(int client_socket,char argv[MAXROW],int create_sock,char control[MAXROW]){
	
	char port[MAXROW],files[MAXROW];
	int port1;

	recv(client_socket, port, MAXROW,0);				
	port1 = atoi(port);
	create_sock = socket_function(port1,argv);
	while(strcmp("1",control)==0){ 	
			
		recv(create_sock,control,MAXROW,0);
		if(strcmp("0",control)==0)			
			break;
		recv(create_sock, files, MAXROW,0);
		printf("%s",files);
	}
	

}
int socket_function(int port,char *arr){

	struct sockaddr_in serverr;
	int client_socket;

	client_socket = socket(AF_INET , SOCK_STREAM , 0);
	if (client_socket < 0)
    	{
        	perror("Could not create socket!!!\n");
        }

	memset(&serverr, 0, sizeof(serverr));
	serverr.sin_family = AF_INET;
 	serverr.sin_addr.s_addr= inet_addr(arr);
 	serverr.sin_port =  htons(port); 

	if (connect(client_socket , (struct sockaddr *)&serverr , sizeof(serverr)) < 0)
        {
		perror("Connect error");
		exit (1);
        }

	return client_socket;

}
void handle(int sig){

    printf("^^^Ctrl + C (SIGINT) CATCH^^^\n");
    close(client_socket);
    signal(SIGINT, SIG_DFL);
    kill(getpid(),SIGINT);

    exit (sig);
}
void usage(){
	printf("\n------------\n");
	printf("Usage : \n");
	printf("cd Client\n");
	printf("./client <ip adress> <port number>\n");
	printf("\n------------\n");
}
