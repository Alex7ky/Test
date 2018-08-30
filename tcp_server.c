#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <strings.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

#define MAX_QUEUE_CONNECTIONS 5
#define BUFFER_SIZE 1024

void reverse_string(char *input, char *output);

FILE *open_file(char *file_name) {
	FILE *file_ptr;
	
	if((file_ptr = fopen(file_name, "r+")) == NULL) {
		printf("[ERROR #12] Ошибка при открытии файла.\n");
	}
	
	return file_ptr;
}

int main(int argc, char *argv[]) 
{
	FILE *file_ptr;
	
	if (argc != 4) {
		fprintf(stderr, "[ERROR #1] Non-worm number of arguments\n");

		return EXIT_FAILURE;
	}

	struct hostent *host = gethostbyname(argv[1]);

	if (host == NULL) {
		fprintf(stderr, "[ERROR #2] Invalid address (Server IP)\n");

		return EXIT_FAILURE;
	}

	int server_port = atoi(argv[2]);

	if (server_port <= 0) {
		fprintf(stderr, "[ERROR #3] Invalid host port number\n");

		return EXIT_FAILURE;
	}
	
	file_ptr = open_file(argv[3]);
	
	if (file_ptr == NULL) {

		return EXIT_FAILURE;
	}

	int tcp_socket_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (tcp_socket_fd == -1) {
		fprintf(stderr, "[ERROR #4] Failed to create socket: %s\n", strerror(errno));

		return EXIT_FAILURE;
	}

	socklen_t sockaddr_size = sizeof(struct sockaddr);
	struct sockaddr_in server_address;

	bzero(&server_address, sockaddr_size);

	server_address.sin_family = AF_INET;
	server_address.sin_addr = *((struct in_addr *)host->h_addr_list[0]);
	server_address.sin_port = htons(server_port);

	if (bind(tcp_socket_fd, (struct sockaddr *) &server_address, sockaddr_size) == -1) {
		fprintf(stderr, "[ERROR #5] Failed to bind socket: %s\n", strerror(errno));

		return EXIT_FAILURE;
	}

	if (listen(tcp_socket_fd, MAX_QUEUE_CONNECTIONS) == -1) {
		fprintf(stderr, "[ERROR #6] Failed to listen socket: %s\n", strerror(errno));

		return EXIT_FAILURE;
	}

	socklen_t client_sockaddr_size = sizeof(struct sockaddr);
	struct sockaddr_in client_address;
	int client_sock_fd;

	client_sock_fd = accept(tcp_socket_fd, (struct sockaddr *) &client_address, &client_sockaddr_size);

	if (client_sock_fd == -1) {
		fprintf(stderr, "[ERROR #7] Failed to accept socket: %s\n", strerror(errno));

		return EXIT_FAILURE;
	}
	
	int send_bytes;
	char buffer[BUFFER_SIZE] = {0};
	size_t read_bytes;
	long sz = 0, count = 0;
	
	fseek(file_ptr, 0L, SEEK_END);
	sz = ftell(file_ptr);
	printf("%ld\n", sz);
	fseek(file_ptr, 0L, SEEK_SET);
	
	send(client_sock_fd, &sz, sizeof(long), 0);
	
	do {
		bzero(buffer, BUFFER_SIZE);

		read_bytes = fread(buffer, sizeof(char),  BUFFER_SIZE, file_ptr);
		
		send_bytes = send(client_sock_fd, buffer, BUFFER_SIZE, 0);
		
		if (send_bytes < 0) {
			fprintf(stderr, "[ERROR #8] Failed receiving message", strerror(errno));
			
			break;
		}
		
		if (feof(file_ptr)) {
			break;
		}

	} while (1);

	fclose(file_ptr);
	
	if (close(tcp_socket_fd) == -1) {
		fprintf(stderr, "[ERROR #10] Failed to close socket server: %s\n", strerror(errno));

		return EXIT_FAILURE;
	}

	if (close(client_sock_fd) == -1) {
		fprintf(stderr, "[ERROR #11] Failed to close socket client: %s\n", strerror(errno));

		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

void reverse_string(char *input, char *output) {

	int len_input_str = strlen(input);

	for (int i = 0; i < len_input_str; i++) {
		output[i] = input[len_input_str - i - 1];
	}

	output[len_input_str] = '\0';

	return;
}
