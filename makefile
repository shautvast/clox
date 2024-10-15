TARGET := ./target
SRC := ./src
CC := clang++ -c -std=c++17 -Wall -Wextra -pedantic -Werror

SRCS := $(shell find $(SRC) -name '*.c')
OBJS := $(SRCS:%=$(TARGET)/%.o)

$(TARGET)/lox: $(TARGET)/lox.cpp.o $(TARGET)/parser.cpp.o $(TARGET)/scanner.cpp.o $(TARGET)/tokens.cpp.o
	clang++ $(TARGET)/lox.cpp.o -L$(TARGET) -lscanner.cpp.o -ltokens.cpp.o -o $(TARGET)/lox

$(TARGET)/tokens.cpp.o: $(SRC)/tokens.cpp
	$(CC) $< -o $@

$(TARGET)/scanner.cpp.o: $(SRC)/scanner.cpp
	$(CC) $< -o $@

$(TARGET)/parser.cpp.o: $(SRC)/parser.cpp
	$(CC) $< -o $@

$(TARGET)/lox.cpp.o: $(SRC)/lox.cpp $(TARGET)
	$(CC) $< -o $@

$(TARGET):
	mkdir -p $(TARGET)

clean:
	rm -rf $(TARGET)/*
