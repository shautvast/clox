TARGET := ./target
SRC := ./src
CC := clang -c -std=c17 -Wall -Wextra -pedantic -Werror 

SRCS := $(shell find $(SRC) -name '*.c')
OBJS := $(SRCS:%=$(TARGET)/%.o)

#$(TARGET)/lox: $(OBJS)
#	clang $(TARGET)/lox.o -L$(TARGET) -ltokens.o -o $(TARGET)/lox


$(TARGET)/%.c.o: $(SRC)/%.c $(TARGET)
	$(CC) $< -o $@

$(TARGET):
	mkdir -p $(TARGET)
	
clean:
	rm -r $(TARGET)/*

