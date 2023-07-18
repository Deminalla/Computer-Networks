#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <ctype.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>


#define PORT "20000"
// #define HOST "::1"
#define HOST "127.0.0.1"

int main(int argc, char const* argv[])
{
	int socket_fd, client_socket;
	struct addrinfo hints, *s;  // hints are params we give
	struct addrinfo *results; 
	struct sockaddr_storage client_address;
	char message[1024] = { 0 };

	memset(&hints, 0, sizeof(hints)); // make sure the struct is empty
	hints.ai_family = AF_UNSPEC;     // IPv4 or IPv6
	hints.ai_socktype = SOCK_STREAM; // TCP stream sockets, type of communication
	hints.ai_flags = AI_PASSIVE;     // fill in the IP for me


	if ((getaddrinfo(HOST, PORT, &hints, &results)) != 0) {
		perror("Getaddrinfo failed");
    	exit(1);
	}
	
	// loop through all the results and bind to the first we can
	for(s = results; s != NULL; s = s->ai_next) {
		// Creating socket
		// The server-side code keeps the address information of both the server and the client 
		// in a variable of type sockaddr_in, which is a struct
        if ((socket_fd = socket(s->ai_family, s->ai_socktype, s->ai_protocol)) == -1) {
			perror("Server: socket failed");
            continue;
        }
		else {
			printf("Socket created successfully\n");
		}

		// Bind the socket to the address and port number specified in s
        if (bind(socket_fd, s->ai_addr, s->ai_addrlen) == -1) {
            perror("Bind failed");
			close(socket_fd);
            continue;
        }
		else{
			printf("Done with binding\n");
		}
		
		// Turn on the socket to listen for incoming connections
		if (listen(socket_fd, 1) < 0) {
			// The backlog (in this case 1), defines the maximum length to which the queue of pending connections for sockfd may grow. 
			// If a connection request arrives when the queue is full, the client may receive an error with an indication of ECONNREFUSED
			perror("Listen failed");
			close(socket_fd);
		}
		printf("\nListening for incoming connections.....\n");

        break;
    }

	if (s == NULL)  {
        perror("Server: can't open socekt\n");
        exit(1);
    }

	freeaddrinfo(results);

	// Extracts the first connection request on the queue of pending connections for the listening socket
	// Creates a new connected socket, and returns a new file descriptor referring to that socket
	// The server-side code stops and waits at accept() until a client calls connect()
	int client_size = sizeof(client_address);
	client_socket = accept(socket_fd, (struct sockaddr*)&client_address, &client_size);
	if (client_socket < 0) {
		perror("Accept failed");
		exit(1);
	}
	printf("Client connected\n");

	// receiving data from client for 1024 bytes
	int data = recv(client_socket, message, sizeof(message), 0); 
	if (data < 0) {
		perror("Coudln't receive");
		exit(1);
	}
	printf("Message from client: %s\n", message);

	int i = 0;
	char message_cap[1024] = { 0 };
	while (message[i]) {
        message_cap[i] = toupper(message[i]);
        i++;
    }

	if (send(client_socket, message_cap, strlen(message_cap), 0) < 0 ){
		perror("Can't send\n");
		exit(1);
	}

	
	close(client_socket); // closing the connected socket
	close(socket_fd); // closing the listening socket

	return 0;
}
