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
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/stat.h>
#include <fcntl.h>

#define ISspace(x) isspace((int)(x))

typedef enum HttpMethod {GET, HEAD, POST, UNKNOWN} HttpMethod;
const char * const http_methods[] = {
	"GET",
	"HEAD",
	"POST",
	"UNKNOWN",
};


int port_nr;
char *ip_addr;

char webpage[] = 
	"HTTP/1.0 200 OK\r\n"
	"Content-Type: text/html; charset=UTF-8\r\n\r\n"
	"<!DOCTYPE html>\r\n"
	"<html><head><title>HTTP Server</title>\r\n"
	"<style>body {backgroung-color: #FFFF00 }</style></head>\r\n"
	"<body><center><h1>Hello World!</h1><br>\r\n"
	"</center></body></html>\r\n";
	
//logs information to a logfile
void write_logfile()
{
	FILE *file;
	file = fopen("logfile.log", "a+");
	time_t ti;
	time(&ti);
	struct tm* t_info;
	char time_buffer[50];
	t_info = localtime(&ti);
	strftime(time_buffer,50,"%Y - %M-%d %H:%M:%S", t_info);
	fprintf(file, "%s : ", ip_addr);
	fprintf(file, "%s : " , time_buffer);
	fprintf(file, "%d : ", port_nr);
	fclose(file);

	

}

#define BUFFER_SIZE 10000

int main(int argc, char *argv[])
{
	struct sockaddr_in server_addr, client_addr;
	socklen_t sin_len = sizeof(client_addr);
	int fd_server, fd_client;
	char buf[BUFFER_SIZE];
	int on = 1;

	if(argc == 1) 
	{
		printf("Disabled warning !!");
	}

	port_nr = strtol(argv[1], NULL, 10);

	fd_server = socket(AF_INET, SOCK_STREAM, 0);
	if(fd_server < 0)
	{
		perror("socket");
		exit(1);
	}
	memset(&server_addr, 0, sizeof(server_addr));
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
		printf("Waiting.....\n");
		fd_client = accept(fd_server, (struct sockaddr *) &client_addr, &sin_len);
		ip_addr = inet_ntoa(client_addr.sin_addr);

		if(fd_client == -1) 
		{
			perror("Connection failed.....\n");
			continue;
		}

		printf("Got client connection......\n");
		read(fd_client, buf, 9999);
		for(int i = 0; i < 1; i++) 
		{
			printf("%s\n", buf);
		}

		write(fd_client, webpage, sizeof(webpage) - 1);
		write_logfile();
	}
	close(fd_server);
	return 0;
}