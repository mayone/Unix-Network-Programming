#include <stdio.h>	// printf()
#include <stdlib.h>	// atoi(), exit()
#include <string.h>	// memset(), bzero()
#include <time.h>
#include <unistd.h>	// read(), write()
#include <arpa/inet.h>	// struct sockaddr_in, inet_addr, inet_pton

#define BUFLEN 1024
#define CONNECTION_LIMIT 5
#define DEFAULT_PORT 1111
#define EVER ;;

int socketInit(unsigned short port);

int main(int argc, char const *argv[])
{
	unsigned short port;

	int listenfd, connfd;
	char send_buf[BUFLEN];

	time_t ticks;

	if (argc != 2) {
		if (argc == 1) {
			port = DEFAULT_PORT;
			printf("Using default port %d\n", port);
		}
		else {
			printf("Usage: %s <port>\n", argv[0]);
			exit(EXIT_FAILURE);
		}
	}
	else {
		port = atoi(argv[1]);
	}

	listenfd = socketInit(port);

	// Main loop
	for(EVER) {
		// Accept a client from the listening fd
		if ((connfd = accept(listenfd, NULL, NULL)) < 0) {
			printf("ERROR: Accept client error\n");
			close(listenfd);
			exit(EXIT_FAILURE);
		}

		ticks = time(NULL);

		//sprintf(send_buf, "%.24s\n", ctime(&ticks));
		snprintf(send_buf, sizeof(send_buf), "%.24s\n", ctime(&ticks));
		write(connfd, send_buf, strlen(send_buf));
		close(connfd);
	}

	close(listenfd);

	return 0;
}

int socketInit(unsigned short port)
{
	int listenfd;
	struct sockaddr_in servaddr;

	// Initialize sockaddr
	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = PF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(port);

	// Create a TCP socket
	if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		printf("ERROR: Create socket error\n");
		exit(EXIT_FAILURE);
	}

	// Bind the sockaddr with a socket (must cast the sockaddr_in to sockaddr)
	if (bind(listenfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0) {
		printf("ERROR: Bind error\n");
		exit(EXIT_FAILURE);
	}

	// Listen
	if (listen(listenfd, CONNECTION_LIMIT) < 0)	{
		printf("ERROR: Cannot listen.\n");
		exit(EXIT_FAILURE);
	}

	return listenfd;
}