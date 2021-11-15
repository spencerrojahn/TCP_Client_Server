#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>


#define SERVER_PORT 5432
#define MAX_LINE 256

int main(int argc, char *argv[]) {

	FILE *fp;

	// Address resolution
	struct hostent *hp;
	struct sockaddr_in sin;
	char *host;
	char buf[MAX_LINE];
	int s;
	int len;

	if (argc == 2) {
		host = argv[1];
	} else {
		fprintf(stderr, "usage: simplex-talk host\n");
		exit(1);
	}

	// Convert a character string into peer's IP address
	hp = gethostbyname(host);
	if (!hp) {
		fprintf(stderr, "usage: simplex-talk: unknown host: %s\n", host);
		exit(1);
	}

	// We need to pull the first IP address from the DNS result and turn it into the socket address
	// Zero the socket
	bzero((char *)&sin, sizeof(sin));
	// Internet protocol
	sin.sin_family = AF_INET;
	// Copy the address into the socket structure
	bcopy(hp->h_addr, (char*)&sin.sin_addr, hp->h_length);
	// Set up and make sure server port is compatible
	sin.sin_port = htons(SERVER_PORT);

	
	if ((s = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
		perror("simplex-talk: socket");
		exit(1);
	}

	// Active open to the socket
	if (connect(s, (struct sockaddr *)&sin, sizeof(sin)) < 0) {
		perror("simplex-talk: connect");
		close(s);
		exit(1);
	}

	// We now have a live TCP connection
	// Get and send lines of text
	while (fgets(buf, sizeof(buf), stdin)) {
		buf[MAX_LINE-1] = '\0';
		len = strlen(buf) +1;
		send(s, buf, len, 0);

	}
	close(s);

	return 0;


}