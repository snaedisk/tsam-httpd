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

typedef enum HttpMethod {GET, HEAD, POST, UNKNOWN} HttpMethod;

const char * const http_methods[] = {
	"GET",
	"HEAD",
	"POST",
	"UNKNOWN",
};

int main(int argc, char *argv[])
{
	return 0;
}
