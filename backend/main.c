#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>

#include "backend.h"

#define PORT      8080
#define NCLIENTS  16

#define CMD_STOP  "stop"
#define CMD_LIST  "list"
#define CMD_VIEW  "view"
#define CMD_POST  "post"

thread *node_at_path(thread *node, const char *path)
{
	char label[256];
	int i, j;
	node_t *snode;

	for (i = 0; path[i] != '\0' && !isspace(path[i]); ++i)
	{
		for (j = i; path[i] != '/' && path[i] != '\0' && !isspace(path[i]); ++i)
			label[i - j] = path[i];

		label[i - j] = '\0';

		for (j = 0; label[j] != '\0'; ++j)
			if (!isdigit(label[j]))
				break;

		for (snode = node->sub_threads->head; snode != NULL; snode = snode->next)
		{
			if (snode->thr->type == Comment || snode->thr->type == Post)
			{
				if (label[j] == '\0')
					if (snode->thr->epoch == atol(label))
					{
						node = snode->thr;

						break;
					}
			} else { // Folder
				if (strcmp(snode->thr->content, label) == 0)
				{
					node = snode->thr;

					break;
				}
			}
		}

		// could not match this path to a thread path
		if (snode == NULL)
			return NULL;

		// continue if there are more subdirectories
		if (path[i] == '/')
			continue;

		// path[i] is '\0'.
		// decrement (i) so the loop check breaks
		--i;
	}

	return node;
}

void list_ll(ll_t *list, char *reply)
{
	char line[256];
	node_t *snode;

	reply[0] = '\0';

	for (snode = list->head; snode != NULL; snode = snode->next)
	{
		switch (snode->thr->type)
		{
		case Comment:
		case Post:
			sprintf(line, "%d %s %ld\n", snode->thr->type, snode->thr->author, snode->thr->epoch);

			break;
		case Folder:
			sprintf(line, "%d %s %s\n", snode->thr->type, snode->thr->author, snode->thr->content);

			break;
		}

		strcat(reply, line);
	}
}

void cmd_list(thread *root, const char *path, char *reply)
{
	thread *node;

	node = node_at_path(root, path);

	if (node == NULL || node->sub_threads == NULL)
		return;

	reply[0] = '\0';

	list_ll(node->sub_threads, reply);
}

void cmd_view(thread *root, const char *path, char *reply)
{
	thread *node;

	node = node_at_path(root, path);

	if (node == NULL || node->type != Post)
		return;

	strcpy(reply, node->content);
}

thread *cmd_post(thread *root, const char *path, char *reply)
{
	thread *node, *post;

	node = node_at_path(root, path);

	if (node == NULL)
		return NULL;

	post = create_thread(Post, "", "", node);

	if (post == NULL)
		return NULL;

	add_subthread(node, post);

	// store the epoch of this post into the reply
	sprintf(reply, "%ld", post->epoch);

	return post;
}

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
	char buffer[1025] = { 0 },   // buffer to hold the bytes received from the client
	     cmd[5],
	     reply[1025];
	thread *root, *post;

	root = load_database();

	// CMPT/Hello

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

	for (cont = 1, post = NULL; cont;)
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
			buffer[nbytes] = '\0';

			printf("%s\n", buffer);

			// no data was read; connection was probably closed
			if (nbytes == 0)
				break;

			// fill content of post
			if (post != NULL)
			{
				strcpy(post->content, buffer);
				strcpy(reply, "success");

				printf("Replied:\n%s\n", reply);

				post = NULL;

				send(client_fd, reply, strlen(reply), 0);

				continue;
			}

			for (i = 0; i < 4; ++i)
				cmd[i] = buffer[i];

			cmd[i] = '\0';

			// skip space between the command and the command arguments
			for (; isspace(buffer[i]); ++i)
				;

			// default reply is 'fail'
			strcpy(reply, "failure");


			// stop command -- stop backend
			if (strcmp(cmd, CMD_STOP) == 0)
			{
				strcpy(reply, "Goodbye.\n");
				cont = 0;
			} else


			// list command -- list posts under a certain path
			if (strcmp(cmd, CMD_LIST) == 0)
				cmd_list(root, &buffer[i], reply);
			else
			
			// view command -- view post content
			if (strcmp(cmd, CMD_VIEW) == 0)
				cmd_view(root, &buffer[i], reply);
			else

			// post command -- create a new post
			if (strcmp(cmd, CMD_POST) == 0)
			{
				post = cmd_post(root, &buffer[i], reply);

				if (post != NULL)
				{
					// skip the path
					for (; !isspace(buffer[i]); ++i)
						;

					// skip the space separating path and author
					for (; isspace(buffer[i]); ++i)
						;

					// read the author
					for (j = 0; buffer[i] != '\0'; ++i)
						post->author[j++] = buffer[i];

					post->author[j] = '\0';
				}
			}


			// print out the data sent from the client
			printf("Replied:\n%s\n", reply);

			// send a response to the client
			send(client_fd, reply, strlen(reply), 0);
		}

		close(client_fd);
	}

	shutdown(server_fd, SHUT_RDWR);

	return 0;
}
