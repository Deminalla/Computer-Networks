
#include <stdio.h>
#include <string.h>

#include <stdio.h>
#include <stdlib.h>
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
	int socket_fd;
	struct addrinfo hints, *s;
	struct addrinfo *results; // will point to the results
	char client_message[1024] = { 0 };
	char server_message[1024] = { 0 };

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;     
	hints.ai_socktype = SOCK_STREAM; 

	if ((getaddrinfo(HOST, PORT, &hints, &results)) != 0) {
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
    }

	if (s == NULL)  {
		perror("Client: can't open socket");
        exit(1);
    }

	freeaddrinfo(results);
	
	printf("Enter message: ");
	gets(client_message); // so it reads until new line and not until whitespace like scanf

	send(socket_fd, client_message, strlen(client_message), 0);

	int data = recv(socket_fd, server_message, sizeof(server_message), 0);
	printf("Message from server: %s\n", server_message);

	close(socket_fd);
	return 0;
}
