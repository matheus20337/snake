TARGET := snake

SRC = $(wildcard *.c)
OBJ = $(patsubst %.c, %.o, $(SRC))

CC := gcc

CFLAGS := -Wall -Wextra -Werror -pedantic -std=c99 -g
LDFLAGS :=

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) $^ $(LDFLAGS) -o $@

%.o:%.c
	$(CC) -c $(CFLAGS) $<

.PHONY: clean
clean:
	rm -rv $(TARGET) $(OBJ)
