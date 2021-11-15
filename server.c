#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>

// If its less than 1024, need privledges
// Over 1024 you should be fine, but there is a list of what each port is mainly used for
#define SERVER_PORT 5432

// Max number of files that can access the buffer
#define MAX_PENDING 5

// For Input/Output
#define MAX_LINE 256

int main(int argc, char**argv) {
	(void) argc;
	(void) argv;

	// socket address for making the server sockets
	struct sockaddr_in sin;
	char buf[MAX_LINE];
	int buf_len;
	socklen_t addr_len;
	int s, new_s;

	// Building the address data structure
	// Zeros out the entire socket data structure
	bzero((char *)&sin, sizeof(sin));

	// Address family is just Internet
	sin.sin_family = AF_INET;

	// We are going to accept any incoming connection
	// Use this to specify who you are willing to accept
	sin.sin_addr.s_addr = INADDR_ANY;

	// "Host to Network Short"
	// This is where we set up the endianess
	sin.sin_port = htons(SERVER_PORT);

	// Setting up the socket descriptor
	// Grabs us a free socket at the kernel level
	if ((s = socket(PF_INET, SOCK_STREAM, 0)) < 0){
		perror("simplex-talk: socket");
		exit(1);
	}

	// Bind the socket to a (local) address
	if ((bind(s, (struct sockaddr *)&sin, sizeof(sin))) < 0){
		perror("simplex-talk: bind");
		exit(1);
	}
	// Set up buffers for incoming connections
	listen(s, MAX_PENDING);

	// Wait for connection, then receive and print text
	while(1) {
		printf("Server awaiting new connection...\n");
		// The accept system call will block until it esablishes a connection
		// New socket is created for the connection
		if ((new_s = accept(s, (struct sockaddr *)&sin, &addr_len)) < 0){
			perror("simplex-talk: accept");
			exit(1);
		}
		// inet_ntoa - turns the address structure into printable output
		printf("Connection establishedd from client with IP: %s\n", inet_ntoa(sin.sin_addr));
		// Want to see which port the client is using
		// Need to use client port number to get back to their process
		// Client port number is sort of like client process ID
		printf("Client port is %hu\n", ntohs(sin.sin_port));

		// Receive data from the new socket until the client terminates conneciton
		while ((buf_len = recv(new_s, buf, sizeof(buf), 0))) {
			fputs(buf, stdout);	
		}
		close(new_s);
	}
	return 0;
}