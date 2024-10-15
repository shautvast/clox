#pragma once

#include "tokens.hpp"
#include <cstdbool>
#include <list>
#include <string>

typedef struct {
  bool had_error;
  std::list<Token> token_list;
} ScanResult;

class Scanner {
private:
  bool had_error;
  size_t current_pos;
  int start;
  int current_line;
  std::string source;
  std::list<Token> token_list;

public:
  Scanner(std::string s);
  ScanResult scan_tokens();
  void add_token(TokenType type);
  void add_token_with_literal(TokenType type, std::string literal);
  char advance();
  void scan_token();
  void identifier();
  void number();
  bool is_digit(char c);
  void string();
  bool match(char expected);
  char peek_next();
  char peek();
  bool is_alpha(char c);
  bool is_alphanumeric(char c);
  bool is_at_end(void);
  void error(std::string message);
  void report(std::string where, std::string message);
};

typedef struct {
  const std::string key;
  const TokenType value;
} Item;

static const Item keywords[] = {
    {"and", AND},   {"class", CLASS}, {"else", ELSE},     {"false", FALSE},
    {"for", FOR},   {"fun", FUN},     {"if", IF},         {"nil", NIL},
    {"or", OR},     {"print", PRINT}, {"return", RETURN}, {"super", SUPER},
    {"this", THIS}, {"true", TRUE},   {"var", VAR},       {"while", WHILE}};

inline static const TokenType *get_keyword_token(std::string key) {
  int low = 0;
  int high = sizeof(keywords) / sizeof(Item);

  while (low < high) {
    int mid = (low + high) / 2;

    int c = keywords[mid].key.compare(key);
    if (c == 0) {
      return &keywords[mid].value;
    }
    if (c < 0) {
      low = mid + 1;
    } else {
      high = mid;
    }
  }
  return NULL;
}
