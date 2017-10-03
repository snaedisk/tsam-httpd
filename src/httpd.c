#include <sys/socket.h>
#include <sys/types.h>
#include <sys/select.h>
#include <ctype.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <glib.h>
#include <stdbool.h>
#include <time.h>

// from video...
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/stat.h>
#include <fcntl.h>

typedef enum HttpMethod {GET, HEAD, POST, UNKNOWN} HttpMethod;

const char * const http_methods[] = {
	"GET",
	"HEAD",
	"POST",
	"UNKNOWN",
};

char webpage[] = 
"HTTP/1.1 200 OK\r\n"
"Content-Type: text/html; charset=UTF-8\r\n\r\n"
"<!DOCTYPE html>\r\n"
"<html><head><title>ShellWaveX</title>\r\n"
"<style>body {backgroung-color: #FFFF00 }</style></head>\r\n"
"<body><center><h1>Hello World!</h1><br>\r\n"
"</center></body></html>\r\n";
int main(int argc, char *argv[])
{
	struct sockaddr_in server_addr, client_addr;
	socklen_t sin_len = sizeof(client_addr);
	int fd_server, fd_client;
	char buf[2048];
	// holds the file descriptor
	int fding;
	int on = 1;

	fd_server = socket(AF_INET, SOCK_STREAM, 0);
	if(fd_server < 0)
	{
		perror("socket");
		exit(1);
	}

	setsockopt(fd_server, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(int));

	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = INADDR_ANY;
	server_addr.sin_port = htons(1088);

	if(bind(fd_server, (struct sockaddr *) &server_addr, sizeof(server_addr)) == -1) 
	{
		perror("bind");
		close(fd_server);
		exit(1);
	}

	if(listen(fd_server, 10) == -1) 
	{
		perror("listen");
		close(fd_server);
		exit(1);
	}

	while(1)
	{
		fd_client = accept(fd_server, (struct sockaddr *) &client_addr, &sin_len);

		if(fd_client == -1) 
		{
			perror("Connection failed.....\n");
			continue;
		}

		printf("Got client connection......\n");

		if(!fork()) //child process returns 0 
		{
			// child process
			close(fd_client); 
			memset(buf, 0, 2048);
			read(fd_client, buf, 2047);

			printf("%s\n", buf);

			if(!strncmp(buf, "GET /poopemoji.jpg", 16))
			{
				fding = open("favicon.ico", O_RDONLY);
				sendfile(fd_client, fding, NULL, 90000);
				close(fding);
			}
			else 
				write(fd_client, webpage, sizeof(webpage) - 1);

			close(fd_client);
			printf("closing....\n");
			exit(0);
		}
		// parent process
		close(fd_client);
	}







	return 0;
}
