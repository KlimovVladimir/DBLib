TARGET_LIB = libdb.so

CC = gcc

INCLUDES = -I./include
SRCS = src/timsort.c src/libdb.c
OBJS = $(SRCS:.c=.o)

CFLAGS = -fPIC -Wall -Wextra -O2 -g $(INCLUDES)
LDFLAGS = -shared
RM = rm -f

.PHONY: all
all: ${TARGET_LIB}

$(TARGET_LIB): $(OBJS)
	$(CC) ${LDFLAGS} -o $@ $^

$(SRCS:.c=.d):%.d:%.c
	$(CC) $(CFLAGS) -MM $< >$@

include $(SRCS:.c=.d)

.PHONY: install
install:
	cp ${TARGET_LIB} /usr/local/lib/

.PHONY: clean
clean:
	-${RM} ${TARGET_LIB} ${OBJS} $(SRCS:.c=.d)