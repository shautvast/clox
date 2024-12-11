#ifndef TOKENS_H
#define TOKENS_H

typedef enum {
  LEFT_PAREN,
  RIGHT_PAREN,
  LEFT_BRACE,
  RIGHT_BRACE,
  COMMA,
  DOT,
  MINUS,
  PLUS,
  SEMICOLON,
  SLASH,
  STAR,
  BANG,
  BANG_EQUAL,
  EQUAL,
  EQUAL_EQUAL,
  GREATER,
  GREATER_EQUAL,
  LESS,
  LESS_EQUAL,
  IDENTIFIER,
  STRING,
  NUMBER,
  AND,
  CLASS,
  ELSE,
  FALSE,
  FUN,
  FOR,
  IF,
  NIL,
  OR,
  PRINT,
  RETURN,
  SUPER,
  THIS,
  TRUE,
  VAR,
  WHILE,
  END_OF_FILE,
  ERROR
} TokenType;

static inline const char *token_name(TokenType type) {
  static const char *tokens[] = {
      "LEFT_PAREN", "RIGHT_PAREN",   "LEFT_BRACE", "RIGHT_BRACE", "COMMA",
      "DOT",        "MINUS",         "PLUS",       "SEMICOLON",   "SLASH",
      "STAR",       "BANG",          "BANG_EQUAL", "EQUAL",       "EQUAL_EQUAL",
      "GREATER",    "GREATER_EQUAL", "LESS",       "LESS_EQUAL",  "IDENTIFIER",
      "STRING",     "NUMBER",        "AND",        "CLASS",       "ELSE",
      "FALSE",      "FUN",           "FOR",        "IF",          "NIL",
      "OR",         "PRINT",         "RETURN",     "SUPER",       "THIS",
      "TRUE",       "VAR",           "WHILE",      "END_OF_FILE", "ERROR"};

  return tokens[type];
}

typedef struct {
  TokenType type;
  char *lexeme;
  void *literal;
  int line;
} Token;

typedef struct TokenList {
  Token **tokens;
  int size;
  int capacity;
} TokenList;

Token *newToken();

void tokenlist_init(TokenList *list);

void tokenlist_add(TokenList *list, Token *value);

Token *tokenlist_get(TokenList *list, int index);

void tokenlist_print(TokenList *tokenlist);

void tokenlist_free(TokenList *list);

Token *tokenlist_last(TokenList *list);
#endif
