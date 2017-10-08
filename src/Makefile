CC = gcc
CPPFLAGS =
CFLAGS = -std=c11 -D_XOPEN_SOURCE=700 -O2 -Wall -Wextra -Wformat=2
CFLAGS += `pkg-config --cflags --libs glib-2.0`
LDFLAGS =
LOADLIBES =
LDLIBS =
OBJFLAGS = -c
SHELL := /bin/bash
LIBRARIES = -lev2citrusleaf -levent -lssl -lrt -lcrypto -lpthread

SERVER_PORT := 1088

.DEFAULT: all
.PHONY: all post clean

#all: $(CC) $(CFLAGS)  httpd.c -o httpd
all: httpd

clean:
	rm -f *.o

distclean: clean
	rm -f httpd
