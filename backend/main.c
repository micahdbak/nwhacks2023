#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>

#include "thread.h"

#define PORT      8080
#define NCLIENTS  16

#define CMD_STOP  "stop"
#define CMD_LIST  "list"

int main(void)
{
	int server_fd,  // file descriptor to the server socket
	    client_fd,  // file descriptor to the currently considered client socket
	    sockopt,    // socket options
	    addrlen,    // length of the address datatype
	    nbytes,     // number of bytes received from the client
	    cont,       // whether or not the server should continue to accept connections
	    i, j;
	struct sockaddr_in address;  // address of the server socket
	char buffer[1024] = { 0 },   // buffer to hold the bytes received from the client
	     cmd[5],
	     label[256],
	     reply[1024],
	     line[256];
	thread *root, *node;
	node_t *snode;
	time_t id_cmp;

	int date_tmp[4] = { 2023, 1, 21, 0 };

	// allocate the root thread
	root = create_thread(Folder, "Root", "admin", date_tmp);

	sockopt = 1;
	addrlen = sizeof(struct sockaddr_in);

	// create socket file descriptor
	server_fd = socket(AF_INET, SOCK_STREAM, 0);

	// if could not create a file descriptor for the socket
	if (server_fd < 0)
	{
		perror("socket()");

		exit(EXIT_FAILURE);
	}

	// set connection settings
	if (setsockopt(server_fd, SOL_SOCKET,
	               SO_REUSEADDR, &sockopt,
	               sizeof(sockopt)))
	{
		perror("setsockopt()");

		exit(EXIT_FAILURE);
	}

	// set address settings: host and port
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(PORT);

	// bind to the predetermined host and port
	if (bind(server_fd, (struct sockaddr*)&address,
                 sizeof(address)) < 0)
	{
		perror("bind()");

		exit(EXIT_FAILURE);
	}

	// set how many clients can connect to the port
	if (listen(server_fd, NCLIENTS) < 0)
	{
		perror("listen()");

		exit(EXIT_FAILURE);
	}

	printf("Backend started.\n");

	for (cont = 1; cont;)
	{
		if ((client_fd = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen)) < 0)
		{
			perror("accept()");

			exit(EXIT_FAILURE);
		}

		printf("Accepted a connection.\n");

		for (;;)
		{
			nbytes = read(client_fd, buffer, 1024);

			// no data was read; connection was probably closed
			if (nbytes == 0)
				break;

			for (i = 0; i < 4; ++i)
				cmd[i] = buffer[i];

			cmd[i] = '\0';

			printf("Got command %s\n", cmd);

			// skip space between the command and the command arguments
			for (; isspace(buffer[i]); ++i)
				;

			strcpy(reply, "fail");


			// stop command -- stop backend
			if (strcmp(cmd, CMD_STOP) == 0)
				cont = 0;


			// list command -- list posts under a certain path
			if (strcmp(cmd, CMD_LIST) == 0)
			{
				node = root;

				for (; buffer[i] != '\0'; ++i)
				{
					for (j = i; buffer[i] != '/'; ++i)
						label[i - j] = buffer[i];

					label[i - j] = '\0';

					for (j = 0; label[j] != '\0'; ++j)
						if (!isdigit(label[j]))
							break;

					// path is a post; list comments
					if (label[j] == '\0')
					{
						for (snode = node->ll_t->head; snode != NULL; snode = snode->next)
						{
							if (snode->thr->id == id_cmp)
							{
								reply[0] = '\0';

								sprintf(line, "%d %s %ld", snode->thr->type, snode->thr->author, snode->thr->id);

								break;
							}
						}

						break;
					}
				}
			}


			// print out the data sent from the client
			printf("Received: %s\n", buffer);

			// send a response to the client
			send(client_fd, reply, strlen(reply), 0);
		}

		close(client_fd);
	}

	shutdown(server_fd, SHUT_RDWR);

	return 0;
}
