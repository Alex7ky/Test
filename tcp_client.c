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

#define BUFFER_SIZE 1024

FILE *open_file(char *file_name) {
	FILE *file_ptr;
	
	if((file_ptr = fopen(file_name, "w+")) == NULL) {
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

	if (connect(tcp_socket_fd, (struct sockaddr *) &server_address, sockaddr_size) == -1) {
		fprintf(stderr, "[ERROR #5] Failed to connect socket: %s\n", strerror(errno));

		return EXIT_FAILURE;
	}

	char buffer[BUFFER_SIZE] = {0};
	size_t write_bytes;
	
	int read_bytes;
	long sz = 0, count = 0, count_packet = 0, size_last_packet = 0;
	recv(tcp_socket_fd, &sz, sizeof(long), 0);
	
	count_packet = sz / 1024;
	size_last_packet = sz - (count_packet * 1024);
	
	printf("%ld %ld\n", sz, count_packet);
	
	do {
		bzero(buffer, BUFFER_SIZE);

		read_bytes = recv(tcp_socket_fd, buffer, BUFFER_SIZE, 0);
		
		if (read_bytes < 0) {
			fprintf(stderr, "[ERROR #7] Failed receiving message: %s\n", strerror(errno));
			
			break;
		}

		if (count == count_packet) {
			write_bytes = fwrite(buffer, size_last_packet, sizeof(char), file_ptr);
			
			break;
		}
		
		write_bytes = fwrite(buffer, BUFFER_SIZE, sizeof(char), file_ptr);
		count++;
	} while (1);

	fclose(file_ptr);
	
	if (close(tcp_socket_fd) == -1) {
		fprintf(stderr, "[ERROR #8] Failed to close socket: %s\n", strerror(errno));

		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
