#ifndef TOKENS_H
#define TOKENS_H

enum TokenType{
    LEFT_PAREN, RIGHT_PAREN, LEFT_BRACE, RIGHT_BRACE, 
    COMMA, DOT, MINUS, PLUS, SEMICOLON, SLASH, STAR,
    BANG, BANG_EQUAL, EQUAL, EQUAL_EQUAL,
    GREATER, GREATER_EQUAL, LESS, LESS_EQUAL,
    IDENTIFIER, STRING, NUMBER,
    AND, CLASS, ELSE, FALSE, FUN, FOR, IF, NIL, OR,
    PRINT, RETURN, SUPER, THIS, TRUE, VAR, WHILE,
    END_OF_FILE
};

typedef struct {
    enum TokenType type;
    char* lexeme;
    void* literal;
    int line;
} Token;

char* token_name(Token* token);

typedef struct {
    Token* tokens;
    int size;
    int capacity;
} TokenList;

void tokenlist_init(TokenList* list);

void tokenlist_add(TokenList* list, Token value);

Token* tokenlist_get(TokenList* list, int index);

void tokenlist_print(TokenList* tokenlist);

void tokenlist_free(TokenList* list);

#endif
