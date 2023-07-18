#include <stdio.h>
#include <string.h> 
#include <stdlib.h>
#include <errno.h>
#include <unistd.h> 
#include <arpa/inet.h> 
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/time.h>
	

char name[1024] = {0};
char *ask_name = "ATSIUSKVARDA\n";
char *name_ok = "VARDASOK\n";
char message_chat[2600] = {0};
char message_to_bot[2600] = {0};
	
int main(int argc , char *argv[])
{
	int port;
	int opt = 1;
	int master_socket, addrlen, new_socket, client_socket[30] = {0};
	int	max_clients = 30, activity, i, valread, sd;
	int max_sd;
	int bot_logged_in = 0;
	struct sockaddr_in6 address;
	char client_names[30][100] = {0};
	char buffer[1025]; 
	int bot_socket;
	char *bot_name = "BOT";
		
	//set of socket descriptors
	fd_set readfds;

	printf("Port number: ");
	scanf("%d", &port);
	
	//initialise all client_socket[] to 0 so not checked
	for (i = 0; i < max_clients; i++)
	{
		client_socket[i] = 0;
	}
		
	//create a master socket
	if( (master_socket = socket(AF_INET6, SOCK_STREAM , 0)) == 0)
	{
		perror("socket failed");
		exit(EXIT_FAILURE);
	}
	
	if( setsockopt(master_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&opt,
		sizeof(opt)) < 0 )
	{
		perror("setsockopt");
		exit(EXIT_FAILURE);
	}
	
	//type of socket created
	memset(&address, 0, sizeof(address));
	address.sin6_family = AF_INET6;
	address.sin6_addr = in6addr_any;
	address.sin6_port = htons( port );
		
	if (bind(master_socket, (struct sockaddr *)&address, sizeof(address))<0)
	{
		perror("bind failed");
		exit(EXIT_FAILURE);
	}
	printf("Listener on port %d \n", port);
		
	if (listen(master_socket, 5) < 0)
	{
		perror("listen");
		exit(EXIT_FAILURE);
	}
		
	//accept the incoming connection
	addrlen = sizeof(address);
	puts("Waiting for connections ...");
		
	while(1)
	{
		//clear the socket set
		FD_ZERO(&readfds);
	
		//add master socket to set
		FD_SET(master_socket, &readfds);
		max_sd = master_socket;
			
		//add child sockets to set
		for ( i = 0 ; i < max_clients ; i++)
		{
			sd = client_socket[i];
				
			//if valid socket descriptor then add to read list
			if(sd > 0)
				FD_SET( sd , &readfds);
				
			//highest file descriptor number, need it for the select function
			if(sd > max_sd)
				max_sd = sd;
		}
	
		//wait for an activity on one of the sockets, timeout is NULL, so wait indefinitely
		activity = select( max_sd + 1 , &readfds , NULL , NULL , NULL);
	
		if ((activity < 0) && (errno!=EINTR))
		{
			printf("select error");
		}
			
		//If something happened on the master socket, then its an incoming connection
		if (FD_ISSET(master_socket, &readfds))
		{
			if ((new_socket = accept(master_socket, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0)
			{
				perror("accept");
				exit(EXIT_FAILURE);
			}
			
			printf("New connection, socket fd is %d\n", new_socket);
		
			if(send(new_socket, ask_name, strlen(ask_name), 0) != strlen(ask_name) )
			{
				perror("send");
			}
		
			if (recv(new_socket, name, sizeof(name), 0) < 0) {
				perror("Coudln't receive");
				exit(1);
			}

			char name_fixed[100] = {0};
			int k = 0;

			while(name[k] != '\n' && name[k] != '\0' && name[k] != '\r'){
			name_fixed[k] = name[k];
			k++;
			}
			printf("joined client: %s\n", name_fixed);

			if(send(new_socket, name_ok, strlen(name_ok), 0) != strlen(name_ok) )
			{
				perror("send");
			}
				
			//add new socket to array of sockets
			for (i = 0; i < max_clients; i++)
			{
				if(client_socket[i] == 0)
				{
					if(strcmp(name_fixed, bot_name) == 0){
						bot_socket = new_socket;
						printf("It is BOT\n");
					}
					else{
						client_socket[i] = new_socket;
						strcpy(client_names[i], name_fixed);
						
						// send to bot abt new client
						char bot_message[100] = { 0 };
						strcat(bot_message, "new_client ");
						strcat(bot_message, name_fixed);
						send(bot_socket, bot_message, strlen(bot_message), 0);


						char socket_validation[4] = {0};
						recv(bot_socket, socket_validation, 3, 0);
						printf("%s\n", socket_validation);
						if(strcmp(socket_validation, "bad")==0){
							printf("Blocked client");
							close(new_socket);
							client_socket[i] = 0;
						}
					}
					break;
				}
			}
		}
			
		//else its some IO operation on some other socket
		for (i = 0; i < max_clients; i++)
		{
			sd = client_socket[i];
				
			if (FD_ISSET( sd , &readfds))
			{
				//Check if it was for closing , and also read the incoming message
				if ((valread = read( sd , buffer, 1024)) == 0)
				{
					//Somebody disconnected , get his details and print
					getpeername(sd , (struct sockaddr*)&address , \
						(socklen_t*)&addrlen);
					printf("Host disconnected\n");
						
					//Close the socket and mark as 0 in list for reuse
					close( sd );
					client_socket[i] = 0;
				}
					
				//Echo back the message that came in
				else
				{
					buffer[valread] = '\0';

					char bot_message[1000] = { 0 };
					strcat(bot_message, client_names[i]);
					strcat(bot_message, " ");
					strcat(bot_message, buffer);
					send(bot_socket, bot_message, strlen(bot_message), 0);

					char message_validation[25] = {0};
					recv(bot_socket, message_validation, sizeof(message_validation), 0);
					printf("%s\n", message_validation);

					if(strcmp(message_validation, "ok") == 0){
						strcpy(message_chat, "PRANESIMAS");
						strcat(message_chat, client_names[i]);
						strcat(message_chat, ":");
						strcat(message_chat, buffer);
						for (int j = 0; j < max_clients; j++)
						{
							if(client_socket[j]!= 0){
								send(client_socket[j] , message_chat , strlen(message_chat) , 0 );
							}
						}
					}

					else if (strncmp(message_validation, "ISPEK1", 6) == 0) {
						strcat(message_validation, "\n");
						send(client_socket[i] , message_validation , strlen(message_validation), 0 );
					}

					else if (strncmp(message_validation, "ISPEK2", 6) == 0) {
						strcat(message_validation, "\n");
						send(client_socket[i] , message_validation , strlen(message_validation), 0 );
						printf("Host disconnected\n");
						close(client_socket[i]);
						client_socket[i] = 0; 
					}

					else if (strncmp(message_validation, "ISPEK3", 6) == 0) {
						strcat(message_validation, "\n");
						send(client_socket[i] , message_validation , strlen(message_validation), 0 );

						printf("Host disconnected\n");
						close(client_socket[i] );
						client_socket[i] = 0; 
					}
				}
			}
		}
	}
	return 0;
}
