/***********************************************************************************
* Lauren Egerton
* CS 372 / Spring 2017
* Project 1
* Filename : chatclient.c
* Description : Host B / Client
* Runs with command line: ./chatclient <server-hostname> <port#>
* Cite1 - general resource: http://beej.us/guide/bgnet/output/print/bgnet_USLetter.pdf
* Cite2: My own Client code used for Project 4 in CS 344-OS 
* Cite3: Code snippets from Prof. Brewster's notes/CS 344 used in above program.
 ***********************************************************************************/

#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <dirent.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 

#define MAX     489		// can receive and send up to 489 characters (500 including handle)	

/****** sendMessage: function prototype *******/
void sendMessage(int fd, char promptB[11], char handleB[11]);

/****** receiveMessage: function prototype *******/
void receiveMessage(int fd);

/*	function to display error message 
	param: const char*
	pre: none
	post: error message sent to stderr; program exits with 0
	return: none									       
*/
void error(const char *msg) { perror(msg); exit(0); } 


/***************** MAIN FUNCTION *****************/
int main(int argc, char *argv[]){

	int socketFD, portNumber;					// socket and port no.			
	struct sockaddr_in serverAddress;			// address struct - IP address
	struct hostent* serverHostInfo;				// hostname

	char gotName[20];
	char name[10];								// user's handle
	//char end[] = "quit";								
	// char buffer[MAX];						// input from user
	char prompt[11];							// for Client-user prompt
	char handle[11];							// for sending msg 

	// Check usage and args 
	if (argc != 3){ 							
		fprintf(stderr,"USAGE of %s incorrect\n", argv[0]); 
		fprintf(stderr,"USE: ./chatclient <hostname> <port#>\n");
		exit(1); 
	} 

	// Get user handle
	do{
		memset(gotName, '\0', sizeof(gotName));		// clear name array
		printf("Enter a username >>");				
		fgets(gotName, sizeof(gotName)-1, stdin);	// get input from user - leave \0 
		if(strlen(gotName) > 10){
			printf("!!! Name is too long - try again. !!!\n");
		}
	}while(strlen(gotName) > 10);

	gotName[strcspn(gotName, "\n")] = '\0';			// remove trailing \n that is added by fgets
	strcpy(name, gotName);

	memset(prompt, '\0', sizeof(prompt));			// clear prompt array
	strcpy(prompt, name);
	strcat(prompt, ">> ");

	memset(handle, '\0', sizeof(handle));			// clear handle array
	strcpy(handle, name);
	strcat(handle, ":");

	/// Create network socket (server address struct)
	memset((char*)&serverAddress, '\0', sizeof(serverAddress));		// clear address struct
	portNumber = atoi(argv[2]); 									// get & convert the port number
	serverAddress.sin_family = AF_INET; 							// TCP connection
	serverAddress.sin_port = htons(portNumber); 	
	serverHostInfo = gethostbyname(argv[1]);						// get & convert host name 
	if (serverHostInfo == NULL){ 
		fprintf(stderr, "CLIENT: ERROR, no such host\n"); exit(0); 
	}	
	memcpy((char*)&serverAddress.sin_addr.s_addr, 
		(char*)serverHostInfo->h_addr, serverHostInfo->h_length); 	// copy in the address 

	// Create TCP socket
	socketFD = socket(AF_INET, SOCK_STREAM, 0); 				
	if (socketFD < 0) error("CLIENT: ERROR opening socket");

	// Connect to server
	if (connect(socketFD, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) 
		error("CLIENT: ERROR connecting");

	printf("     Connection started.\n     You chat first!\n");

	// Message Loop - chatting begins here
	while(1){									
		sendMessage(socketFD, prompt, handle);			// client sends message first to Server
		receiveMessage(socketFD);						// receive message from Server
	}

	close(socketFD);
	exit(0);									
}

/*	function to send messages to Host A/Server
	param: int, char[11], char[11]
	pre: TCP socket must be connected (=value of int fd)
	post: user is asked for input, input is checked < 489 char,
	user handle appended to input, sent over TCP socket to Host A/Server.
	If user enters '\quit', socket is closed, program exits with 0
	return: none								       
*/
void sendMessage(int fd, char promptB[11], char handleB[11]){

	char firstTry[1024];								// first input from user
	char message[500];									// final message to send

	memset(message, '\0', sizeof(message));				// clear message
	
	int flag = 0;										// flag set to false

	while(!flag){
		memset(firstTry, '\0', sizeof(firstTry));		// clear buffer
		printf("%s", promptB);
		fgets(firstTry, sizeof(firstTry)-1, stdin);		// trunc last char
		firstTry[strcspn(firstTry, "\n")] = '\0';			// remove \n left by fgets
		if(strlen(firstTry) > 489){
			printf("!!! Message too long - try again. !!!\n");
		}
		else if(strlen(firstTry) <= 489){
			flag = 1;
		}
	}

	if(strcmp(firstTry, "\\quit") == 0){				// user entered "quit"
		int bytes_out = send(fd, "\\quit", 5, 0);		// send message to server so it can close TCP connection
		close(fd);
		exit(0); 
	}
	
	strcpy(message, handleB);
	strcat(message, firstTry);

	int bytes_out = send(fd, message, strlen(message), 0);			//only send 500 char total

	if (bytes_out < 0) error("CLIENT: ERROR writing to socket");	//check transmission
	if (bytes_out < strlen(message)) printf("CLIENT: WARNING: Not all data written to socket!\n");
}

/*	function to receive messages to Host A/Server
	param: int
	pre: TCP socket must be connected (=value of int fd)
	post: received message is saved in buffer <= 500 char
	If message = '\quit', program exits with 0
	return: none								       
*/void receiveMessage(int fd){
		char buff[500];							// clear buffer
		memset(buff, '\0', sizeof(buff));

		int bytes_in = recv(fd, buff, sizeof(buff) - 1, 0); 			// leave '\0' at end

		if (bytes_in < 0) error("CLIENT: ERROR reading from socket");	//check transmission
		if (bytes_in == 0){
			printf("Server has closed the connection\n");
			exit(0);
		} 
		
		printf("%s\n", buff);
}






