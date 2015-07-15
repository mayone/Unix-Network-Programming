#include <stdio.h>	// printf()
#include <stdlib.h>	// atoi(), exit()
#include <string.h>	// memset(), bzero()
#include <unistd.h>	// read(), write()
#include <netdb.h>	// gethostbyname()
//#include <sys/socket.h>
//#include <netinet/in.h>	// struct sockaddr_in
#include <arpa/inet.h>	// struct sockaddr_in, inet_addr, inet_pton

#define BUFLEN 1024
#define EVER ;;

int socketInit(const char *hostname, unsigned short port);

int main(int argc, char const *argv[])
{
	int sockfd;

	int recv_len;
	char recv_buf[BUFLEN];

	if (argc != 3) {
		printf("Usage: %s <hostname> <port>\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	sockfd = socketInit(argv[1], atoi(argv[2]));

	// Clear buffers
	memset(recv_buf, 0, sizeof(recv_buf));

	for(EVER) {
		recv_len = read(sockfd, recv_buf, sizeof(recv_buf));
		if (recv_len == 0) {
			printf("Server close the connection\n");
			exit(EXIT_SUCCESS);
		}
		else if (recv_len < 0) {
			printf("ERROR: Read from the server error\n");
			exit(EXIT_FAILURE);
		}
		else {
			fputs(recv_buf, stdout);
		}
	}

	close(sockfd);

	return 0;
}

int socketInit(const char *hostname, unsigned short port)
{
	int sockfd;
	struct sockaddr_in servaddr;
	struct hostent *host;

	// Initialize sockaddr
	memset(&servaddr, 0, sizeof(servaddr));

	// Family domain
	servaddr.sin_family = PF_INET;
	// IP address
	host = gethostbyname(hostname);
	if (host == NULL) {
		printf("ERROR: Get host by name error\n");
		exit(EXIT_FAILURE);
	}
	memcpy(&servaddr.sin_addr, host->h_addr, host->h_length);
	// Port number
	servaddr.sin_port = htons(port);

	// Create socket
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		printf("ERROR: Create socket error\n");
		exit(EXIT_FAILURE);
	}

	// Connect to the server specified in the servaddr
	if (connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0)	{
		printf("ERROR: Connect to the server error\n");
		exit(EXIT_FAILURE);
	}

	return sockfd;
}