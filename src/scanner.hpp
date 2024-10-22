#pragma once

#include "tokens.hpp"
#include <cstdbool>
#include <string>
#include <vector>

using namespace std;

typedef struct {
  bool had_error;
  vector<Token> token_list;
} ScanResult;

class Scanner {
private:
  bool had_error;
  size_t current_pos;
  int start;
  int current_line;
  string source;
  vector<Token> token_list;

public:
  Scanner(string s);
  ScanResult scan_tokens();
  void add_token(Token::Type type);
  void add_token(Token::Type type, string literal);
  char advance();
  void scan_token();
  void identifier();
  void number();
  bool is_digit(char c);
  void scan_string();
  bool match(char expected);
  char peek_next();
  char peek();
  bool is_alpha(char c);
  bool is_alphanumeric(char c);
  bool is_at_end(void);
  void error(string message);
  void report(string where, string message);
};
