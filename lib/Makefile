TARGET_LIB = libdb.so

CC = gcc

INCLUDES = -I./include
SRCS = src/timsort.c src/libdb.c
OBJS = $(SRCS:.c=.o)

CFLAGS = -fPIC -Wall -Wextra -O2 -g $(INCLUDES)
LDFLAGS = -shared
RM = rm -f

all: ${TARGET_LIB}

$(TARGET_LIB): $(OBJS)
	$(CC) ${LDFLAGS} -o $@ $^

$(SRCS:.c=.d):%.d:%.c
	$(CC) $(CFLAGS) -MM $< >$@

include $(SRCS:.c=.d)

install:
	cp ${TARGET_LIB} /usr/local/lib/

clean:
	-${RM} ${TARGET_LIB} ${OBJS} $(SRCS:.c=.d)