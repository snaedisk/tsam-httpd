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
"HTTP/1.1 200 OK\r\n"
"Content-Type: text/html; charset=UTF-8\r\n\r\n"
"<!DOCTYPE html>\r\n"
"<html><head><title>HTTP Server</title>\r\n"
"<style>body {backgroung-color: #FFFF00 }</style></head>\r\n"
"<body><center><h1>Hello World!</h1><br>\r\n"
"</center></body></html>\r\n";

void cat(int fd_client, FILE *resource) 
{
	char buff[1024];

	fgets(buff, sizeof(buff), resource);

	while(!feof(resource))
	{
		send(fd_client, buff, strlen(buff), 0);
		fgets(buff, sizeof(buff), resource);
	}
}

void bad_request(int fd_client) 
{
	char buff[1024];

	sprintf(buff, "HTTP/1.1 400 Bad Request\r\n");
	send(fd_client, buff, sizeof(buff), 0);
	sprintf(buff, "Content-Type: text/html\r\n");
	send(fd_client, buff, sizeof(buff), 0);
	sprintf(buff, "\r\n");
	send(fd_client, buff, sizeof(buff), 0);
	sprintf(buff, "<p>Got bad request, ");
	send(fd_client, buff, sizeof(buff), 0);
	sprintf(buff, "e.g., malformed request syntax, invalid request message framing, or deceptive request routing.\r\n");
	send(fd_client, buff, sizeof(buff), 0);
}

void not_found_request(int fd_client)
{
	char buff[1024];

	sprintf(buff, "HTTP/1.1 404 NOT FOUND\r\n");
	send(fd_client, buff, sizeof(buff), 0);
	sprintf(buff, "Server: httpd\r\n");
	send(fd_client, buff, sizeof(buff), 0);
	sprintf(buff, "Content-Type: text/html\r\n");
	send(fd_client, buff, sizeof(buff), 0);
	sprintf(buff, "\r\n");
	send(fd_client, buff, sizeof(buff), 0);
	sprintf(buff, "<html><head><title>Not Found</title></head>\r\n");
	send(fd_client, buff, sizeof(buff), 0);
	sprintf(buff, "<body><p>The server could not fulfill your request.\r\n");
	send(fd_client, buff, sizeof(buff), 0);
	sprintf(buff, "</body></html>\r\n");
	send(fd_client, buff, sizeof(buff), 0);
}

void unimplemented_request(int fd_client)
{
	char buff[1024];

	sprintf(buff, "HTTP/1.1 501 Method not implemented\r\n");
	send(fd_client, buff, sizeof(buff), 0);
	sprintf(buff, "Server: httpd\r\n");
	send(fd_client, buff, sizeof(buff), 0);
	sprintf(buff, "Content-Type: text/html\r\n");
	send(fd_client, buff, sizeof(buff), 0);
	sprintf(buff, "\r\n");
	send(fd_client, buff, sizeof(buff), 0);
	sprintf(buff, "<html><head><title>Method Not Implemented</title></head>\r\n");
	send(fd_client, buff, sizeof(buff), 0);
	sprintf(buff, "<body><p>Http request method not supported.\r\n");
	send(fd_client, buff, sizeof(buff), 0);
	sprintf(buff, "</body></html>\r\n");
	send(fd_client, buff, sizeof(buff), 0);	
}

void ok_request(int fd_client, const char *filename) 
{
	char buff[1024];
	//(void)filename;

	strcpy(buff, "HTTP/1.1 200 OK\r\n");
	send(fd_client, buff, strlen(buff), 0);
	strcpy(buff, "Server: httpd\r\n");
	send(fd_client, buff, strlen(buff), 0);
	sprintf(buff, "Content-Type: text/html\r\n");
	send(fd_client, buff, sizeof(buff), 0);
	strcpy(buff, "\r\n");
	send(fd_client, buff, sizeof(buff), 0);
}

void serve_file(int fd_client, const char *filename)
{
	FILE *resource = NULL;
	int numchars = 1; 
	char buff[1024];

	buff[0] = 'A';
	buff[1] = '\0';

	while((numchars > 0) && strcmp("\n", buff))
	{
		//numchars = get_line(client, buff, sizeof(buff));
	}

	resource = fopen(filename, "r");

	if(resource != NULL) 
	{
		ok_request(fd_client, filename);
		cat(fd_client, resource);
	}
	else 
	{
		not_found_request(fd_client);
	}	
	fclose(resource);
}

void write_logfile()
{
	FILE *file;
	file = fopen("logfile.log", "a+");
	time_t timer;
	char time_buffer[50];
	struct tm* time_info;
	time(&timer);
	time_info = localtime(&timer);
	strftime(time_buffer,50,"%Y - %M-%D %H:%M:%S", time_info);
	puts(time_buffer);
	fprintf(file, "%s : " , time_buffer);
	fprintf(file, "%s : ", ip_addr);
	fprintf(file, "%d : ", port_nr);
	fclose(file);

	{
		/* data */
	};

}

#define BUFFER_SIZE 10000

int main(int argc, char *argv[])
{
	struct sockaddr_in server_addr, client_addr;
	socklen_t sin_len = sizeof(client_addr);
	int fd_server, fd_client;
	char buf[BUFFER_SIZE];
	int on = 1;

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

		//handle_http_request(fd_client);
		write_logfile();

	}

	return 0;
}