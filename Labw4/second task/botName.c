
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

char *ok = "ok";
char *bad = "bad";
char ISPEK[3][10] = {"ISPEK1", "ISPEK2", "ISPEK3"};

int main(int argc, char const* argv[])
{
	int socket_fd;
	struct addrinfo hints, *s;
	struct addrinfo *results; // will point to the results
    char client_names[30][100] = {0};
    int client_warnings[30] = {0};
    char blocked_client[30][100] = {0};
    char curses[3][10] = {"demon", "gun", "hate"};

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;     
	hints.ai_socktype = SOCK_STREAM; 

    char port[10] = {0};
    printf("Port number: ");
	scanf("%s", port);

    char host[20] = {0};
    printf("Host number: ");
	scanf("%s", host);

	if ((getaddrinfo(host, port, &hints, &results)) != 0) {
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
	
    char bot_message[1024] = { 0 };
    char server_message[1024] = { 0 };  


    recv(socket_fd, server_message, sizeof(server_message), 0);
    printf("Message from server about name: %s", server_message); // ATSIUSKVARDA

    memset(server_message, '\0', sizeof(server_message));

    char name[3] = "BOT";
    send(socket_fd, name, 3, 0);
    recv(socket_fd, server_message, sizeof(server_message), 0);
    printf("Message from server about name: %s", server_message); // VARDASOK

    memset(server_message, '\0', sizeof(server_message));

    while(1){
        recv(socket_fd, server_message, sizeof(server_message), 0);
        printf("Message from server: %s\n", server_message);

        if(strncmp(server_message, "new_client ", 11) == 0){
            int mess_len = strlen(server_message) - 11; // starting from 12th position
            char name[100] = {0};
            int len = 11;

            strncpy(name, server_message + len, strlen(server_message));

            printf("Client name: %s\n", name);
            
            int blocked = 0; // true, false
            for (int i = 0; i <= 30; i++)
			{
                // printf("searchig for blocked \n");
				if(strcmp(blocked_client[i], name) == 0)
				{
                    blocked = 1;
                    send(socket_fd, bad, strlen(bad), 0);
					break;
				}
                else if (blocked_client[i][0] == 0){
                    break;
                }
			}

            if(blocked == 0){
              for (int i = 0; i <= 30; i++){
                // printf("searchig for adding \n");
                if(client_names[i][0] == 0 || (strcmp(client_names[i], name) == 0))
                    {
                        strcpy(client_names[i], name);
                        printf ("Client with name %s is saved in index %d\n", name, i);
                        send(socket_fd, ok, strlen(ok), 0);
                        break;
                    }
                }  
            }

        }

        else{
            int s = 0;
            for(int l=0; strlen(server_message); l++)  
            {
                if(server_message[l]== ' ')
                {
                    s = l;
                    break;
                }
            }

            char name[100] = {0};
            strncpy(name, server_message, s);


            char message[1024];
            strncpy(message, server_message + s+1, strlen(server_message));

            printf("Client name: %s\n", name);

            printf("Message from client: %s\n", message);
            
            int warning_sent = 0; // true, false
            for(int k=0; k<3; k++){
                if(strstr(message, curses[k])!= NULL){
                    int j;
                    for (int i = 0; i <= 30; i++)
                    {
                        if(strcmp(client_names[i], name)==0)
                        {
                            j = i;
                            break;
                        }
                        else if (client_names[i][0] == 0){
                            break;
                        }
                    }
                    
                    int nr_warning = client_warnings[j];
                    strcpy(bot_message, ISPEK[nr_warning]);

                    strcat(bot_message, " ");
                    strcat(bot_message, name);
                    printf("message review: %s\n", bot_message);
                    send(socket_fd, bot_message, strlen(bot_message), 0);
                    client_warnings[j]++;
                    if(client_warnings[j] == 3){
                        for (int i = 0; i <= 30; i++)
                        {
                            if(blocked_client[i][0] == 0)
                            {
                                strcpy(blocked_client[i], name);
                                printf ("Client with name %s is saved in blocked %d\n", name, i);
                                break;
                            }
                        }
                    }
                    warning_sent = 1;
                    break;
                }
            }
            if(warning_sent == 0){
                strcpy(bot_message, ok);
                printf("message review: %s\n", bot_message);
                send(socket_fd, bot_message, strlen(bot_message), 0);
            }
        }

        memset(bot_message, '\0', sizeof(bot_message));
        memset(server_message, '\0', sizeof(server_message));
    }

	close(socket_fd);
	return 0;
}
