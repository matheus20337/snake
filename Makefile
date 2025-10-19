TARGET := snake

SRC = $(wildcard *.c)
OBJ = $(patsubst %.c, %.o, $(SRC))

CC := gcc

CFLAGS := -Wall -Wextra -Werror -g
LDFLAGS := -lncurses

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) $^ $(LDFLAGS) -o $@

%.o:%.c
	$(CC) -c $(CFLAGS) $<

.PHONY: clean
clean:
	rm -rv $(TARGET) $(OBJ)
