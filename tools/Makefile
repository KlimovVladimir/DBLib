CC = gcc

INCLUDES = -I./../lib/include
LIBS = -L./../lib -ldb

CFLAGS = -fPIC -Wall -Wextra -W -O2 -g $(INCLUDES)

.PHONY: all storedump dbutil testutil clean

all: storedump dbutil testutil


storedump: storedump/storedump.c
	$(CC) $(CFLAGS) storedump/storedump.c -o storedump/storedump $(LIBS)

dbutil: dbutil/dbutil.c
	$(CC) $(CFLAGS) dbutil/dbutil.c -o dbutil/dbutil $(LIBS)

testutil: testutil/testutil.c
	$(CC) $(CFLAGS) testutil/testutil.c -o testutil/testutil $(LIBS)

install:
	cp storedump/storedump  /usr/local/bin/
	cp dbutil/dbutil        /usr/local/bin/
	cp testutil/testutil    /usr/local/bin/

clean:
	rm -rf testutil/testutil
	rm -rf storedump/storedump
	rm -rf dbutil/dbutil