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
  void add_token(Token::Type type);
  void add_token_with_literal(Token::Type type, std::string literal);
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
  const Token::Type value;
} Item;

static const Item keywords[] = {
    {"and", Token::Type::AND},       {"class", Token::Type::CLASS},
    {"else", Token::Type::ELSE},     {"false", Token::Type::FALSE},
    {"for", Token::Type::FOR},       {"fun", Token::Type::FUN},
    {"if", Token::Type::IF},         {"nil", Token::Type::NIL},
    {"or", Token::Type::OR},         {"print", Token::Type::PRINT},
    {"return", Token::Type::RETURN}, {"super", Token::Type::SUPER},
    {"this", Token::Type::THIS},     {"true", Token::Type::TRUE},
    {"var", Token::Type::VAR},       {"while", Token::Type::WHILE}};

inline static const Token::Type *get_keyword_token(std::string key) {
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
