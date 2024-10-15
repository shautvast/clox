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
