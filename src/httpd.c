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

#define BUFFER_SIZE 10000

int port_nr;
char *ip_addr;
char *str;
char *url;

char webpage_start[] = 
"HTTP/1.0 200 OK\r\n"
"Content-Type: text/html; charset=UTF-8\r\n\r\n"
"<!DOCTYPE html>\r\n"
"<html><head><title>HTTP Server</title>\r\n"
"<style>body {backgroung-color: #FFFF00 }</style></head>\r\n"
"<body><center><h1>Hello World!\r\n";

char webpage_end[] = 
"</h1><br>\r\n"
"</body></html>\r\n";

void write_logfile()
{
	FILE *file;
	time_t timer = time(NULL);
	char responseCode[30];
	//char time_buffer[50];
	//struct tm* time_info;
	
	file = fopen("logfile.log", "a+");
	if(file == NULL)
	{
		printf("Error\n");
	}
	
	/*time(&timer);
	time_info = localtime(&timer);
	strftime(time_buffer, 50, "%Y - %M-%d %H:%M:%S", time_info);
	puts(time_buffer);*/

	//fprintf(file, "%s : " , time_buffer);
	fprintf(file, "%s", ctime(&timer));
	fprintf(file, "%s : ", ip_addr);
	fprintf(file, "%d : ", port_nr);

	fprintf(file, " : %s", str);
	fprintf(file, " : %s", url);

	if(strcmp(str, "GET") == 0 || strcmp(str, "POST") == 0 || strcmp(str, "HEAD") == 0)
	{
		strcat(responseCode, "200 OK");
	}
	else
	{
		strcat(responseCode, "400 error");
	}
	fprintf(file, " : %s", responseCode);
	fclose(file);

	{
		/* data */
	};

}

int main(int argc, char *argv[])
{
	struct sockaddr_in server_addr, client_addr;
	socklen_t sin_len = sizeof(client_addr);
	int fd_server, fd_client;
	char buf[BUFFER_SIZE];
	int on = 1;

	/*for(int i = 0; i < argc; i++) 
	{
		printf("%s\n", argv[i]);
	}*/
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
		FILE *file;

		printf("Waiting.....\n");
		fd_client = accept(fd_server, (struct sockaddr *) &client_addr, &sin_len);
		ip_addr = inet_ntoa(client_addr.sin_addr);

		if(fd_client == -1) 
		{
			perror("Connection failed.....\n");
			continue;
		}

		file = fdopen(fd_client, "r");
		//read the first line and check what request we are suppose to handle

		printf("Got client connection......\n");
		read(fd_client, buf, 9999);

		str = fgets(buf, sizeof(buf), file);
		fprintf(stderr, "Request line from client: %s\n", str);
		//Get the request
		str = strtok(str, "\r\n");
		fprintf(stderr, "Request: \n");

		//Get the url from client
		url = strtok(NULL, "\r\n");
		if(url == NULL)
		{
			fprintf(stderr, "URL is NULL.....\n");
		}
		if(url[0] == '/')
		{
			url = &url[1];
		}
		fprintf(stderr, "URL: %s\n", url);
		//Finished getting info from client
		
		write_logfile();

		//handle different types of requests
		char buffer[2048];

		if(strcmp(str, "GET") == 0) 
		{
			strcpy(buffer, webpage_start);
			printf("Inside GET...");
			strcat(buffer, ip_addr);

			/****************/

			strcat(buffer, webpage_end);
			printf("Sending: %s\n", buffer);

			send(fd_client, buffer, strlen(buffer), 0);
		}
		else if(strcmp(str, "POST") == 0)
		{
			printf("Trying post.....\n");

			char** split_buffer = g_strsplit(buf, "\r\n\r\n", 2);
			strcpy(buffer, webpage_start);
			strcat(buffer, split_buffer[1]);
			strcat(buffer, webpage_end);

			printf("POST request: sending.....\n");
			send(fd_client, buffer, strlen(buffer), 0);
		}
		else if(strcmp(str, "HEAD") == 0) 
		{
			/**************************/
			send(fd_client, buffer, strlen(buffer), 0);
		}
	}
	close(fd_server);
	return 0;
}
