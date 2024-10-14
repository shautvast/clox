TARGET := ./target
SRC := ./src
CC := clang -c -std=c17 -Wall -Wextra -pedantic -Werror

SRCS := $(shell find $(SRC) -name '*.c')
OBJS := $(SRCS:%=$(TARGET)/%.o)

$(TARGET)/lox: $(TARGET)/lox.c.o $(TARGET)/parser.c.o $(TARGET)/scanner.c.o $(TARGET)/tokens.c.o $(TARGET)/utils.c.o
	clang $(TARGET)/lox.c.o -L$(TARGET) -lparser.c.o -ltokens.c.o -lscanner.c.o -lutils.c.o -o $(TARGET)/lox

$(TARGET)/utils.c.o: $(SRC)/utils.c
	$(CC) $< -o $@

$(TARGET)/tokens.c.o: $(SRC)/tokens.c
	$(CC) $< -o $@

$(TARGET)/scanner.c.o: $(SRC)/scanner.c
	$(CC) $< -o $@

$(TARGET)/parser.c.o: $(SRC)/parser.c
	$(CC) $< -o $@

$(TARGET)/lox.c.o: $(SRC)/lox.c $(TARGET)
	$(CC) $< -o $@

$(TARGET):
	mkdir -p $(TARGET)

clean:
	rm -rf $(TARGET)/*
