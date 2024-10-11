TARGET := ./target
SRC := ./src
CC := clang -c -std=c17 -Wall -Wextra -pedantic -Werror

SRCS := $(shell find $(SRC) -name '*.c')
OBJS := $(SRCS:%=$(TARGET)/%.o)

$(TARGET)/lox: $(TARGET)/lox.c.o $(TARGET)/tokens.c.o
	clang $(TARGET)/lox.c.o -L$(TARGET) -ltokens.c.o -o $(TARGET)/lox

$(TARGET)/tokens.c.o: $(SRC)/tokens.c
	$(CC) $< -o $@

$(TARGET)/lox.c.o: $(SRC)/lox.c $(TARGET)
	$(CC) $< -o $@

$(TARGET):
	mkdir -p $(TARGET)

clean:
	rm -rf $(TARGET)/*
