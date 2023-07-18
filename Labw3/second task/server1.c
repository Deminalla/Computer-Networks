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


#define PORT_TO_C "20000"
#define PORT_TO_S2 "20001"
// #define HOST "::1"
#define HOST "127.0.0.1"

int main(int argc, char const* argv[])
{
	int socket_fd, client_socket;
	struct addrinfo hints, *s;  // hints are params we give
	struct addrinfo *results; 
	struct sockaddr_storage client_address;
	char message_from_c[6] = { 0 };

	memset(&hints, 0, sizeof(hints)); // make sure the struct is empty
	hints.ai_family = AF_UNSPEC;     // IPv4 or IPv6
	hints.ai_socktype = SOCK_STREAM; // TCP stream sockets, type of communication
	hints.ai_flags = AI_PASSIVE;     // fill in the IP for me


	if ((getaddrinfo(HOST, PORT_TO_C, &hints, &results)) != 0) {
		perror("Getaddrinfo failed");
    	exit(1);
	}
	
	// loop through all the results and bind to the first we can
	for(s = results; s != NULL; s = s->ai_next) {
        if ((socket_fd = socket(s->ai_family, s->ai_socktype, s->ai_protocol)) == -1) {
			perror("Server: socket failed");
            continue;
        }
		else {
			printf("Socket created successfully\n");
		}
        int yes = 1;
        if (setsockopt(socket_fd, SOL_SOCKET, SO_REUSEPORT, &yes,               //leidžia avoidinti "Address already in use"
                sizeof(int)) == -1) {
            perror("setsockopt");
            exit(1);
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
		
		if (listen(socket_fd, 1) < 0) {
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

	int client_size = sizeof(client_address);
	client_socket = accept(socket_fd, (struct sockaddr*)&client_address, &client_size);
	if (client_socket < 0) {
		perror("Accept failed");
		exit(1);
	}
	printf("Client connected\n");

	// receiving data from client for 1024 bytes
	int data = recv(client_socket, message_from_c, sizeof(message_from_c), 0); 
	if (data < 0) {
		perror("Coudln't receive");
		exit(1);
	}
	printf("Message from client: %s\n", message_from_c);

	
	close(client_socket); // closing the connected socket
	close(socket_fd); // closing the listening socket







    printf("\nBecoming client\n");
    
    memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;     
	hints.ai_socktype = SOCK_STREAM; 

	if ((getaddrinfo(HOST, PORT_TO_S2, &hints, &results)) != 0) {
		perror("Getaddrinfo failed");
    	exit(1);
	}
	
	// loop through all the results and connect to the first one we can
	for(s = results; s != NULL; s = s->ai_next) {
        if ((socket_fd = socket(s->ai_family, s->ai_socktype, s->ai_protocol)) == -1) {
			perror("Client: socket failed");
            continue;
        }
		else {
			printf("Socket created successfully\n");
		}

        // Send connection request to server
		if ((connect(socket_fd, s->ai_addr, s->ai_addrlen)) == -1) {
			perror("Connection failed");
			close(socket_fd);
		}
		else{
			printf("Connected with server successfully\n");
		}
        break;
    };
	if (s == NULL)  {
		perror("Client: can't open socket");
        exit(1);
    }

	freeaddrinfo(results);
	
    char message_to_s2[6] = { 0 };
    int i = 0;
	while (message_from_c[i] && i<6) {
        message_to_s2[i] = toupper(message_from_c[i]);
        i++;
    }

    printf("Sending message to S2: %s\n", message_to_s2);
	if (send(socket_fd, message_to_s2, strlen(message_to_s2), 0) < 0 ){
		perror("Can't send\n");
		exit(1);
	}

    close(socket_fd);

	return 0;
}
