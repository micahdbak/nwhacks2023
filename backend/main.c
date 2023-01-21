#include <netinet/in.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define PORT      8080
#define NCLIENTS  1

int main(void)
{
	int server_fd,
	    new_socket,
	    valread,
	    opt = 1,
	    addrlen = sizeof(struct sockaddr_in);
	struct sockaddr_in address;
	char buffer[1024] = { 0 },
	     *hello = "Hello from server";

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
	               SO_REUSEADDR, &opt,
	               sizeof(opt)))
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
		perror("listen");

		exit(EXIT_FAILURE);
	}

	printf("Server started\n");

	if ((new_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen)) < 0)
	{
		perror("accept");

		exit(EXIT_FAILURE);
	}

	printf("Connection made\n");

	while (1)
	{
		printf("Start of loop\n");

		valread = read(new_socket, buffer, 1024);

		if (valread == 0)
			break;

		printf("%s\n", buffer);

		send(new_socket, hello, strlen(hello), 0);

		printf("Hello message sent\n");
	}

	close(new_socket);
	shutdown(server_fd, SHUT_RDWR);

	return 0;
}
