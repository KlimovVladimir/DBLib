CC = gcc

INCLUDES = -I./../lib/include
LIBS = -L./../lib -ldb

CFLAGS = -fPIC -Wall -Wextra -W -O2 -g $(INCLUDES)


all: hello  

hello: test/hello.c
	$(CC) $(CFLAGS) test/hello.c -o hello $(LIBS)

install:
	cp hello /usr/local/bin/

clean:
	rm -rf hello