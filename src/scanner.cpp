#include "scanner.hpp"
#include "tokens.hpp"
#include <cstdbool>
#include <iostream>
#include <list>
#include <string>

static void scan_token(void);
static void error(std::string message);
static void report(std::string where, std::string message);
static bool is_at_end(void);
static bool match(char expected);
static char peek(void);
static char peek_next(void);
static void string(void);
static bool is_digit(char c);
static void number(void);
static bool is_alpha(char c);
static bool is_alphanumeric(char c);
static void identifier();

static bool had_error = false;
static size_t current_pos = -1;
static int start = -1;
static int current_line = -1;
static std::string source;
static std::list<Token> token_list;

ScanResult scan_tokens(std::string src) {
  current_pos = 0;
  start = 0;
  current_line = 1;
  source = src;

  while (current_pos < source.length()) {
    start = current_pos;
    scan_token();
  }

  ScanResult scan_result;
  scan_result.token_list = token_list;
  scan_result.had_error = had_error;

  // tokenlist_print(&scan_result.token_list);

  return scan_result;
}

static void add_token(TokenType type) {
  Token token;
  token.type = type;
  token.lexeme = source.substr(start, current_pos);
  token.literal = NULL;
  token.line = current_line;

  token_list.push_front(token);
}

static void add_token_with_literal(TokenType type, std::string literal) {
  Token token;
  token.type = type;
  token.lexeme = source.substr(start, current_pos);
  token.literal = static_cast<void *>(&literal);
  token.line = current_line;

  token_list.push_front(token);
}

static char advance(void) {
  char c = source.at(current_pos++);
  return c;
}

static void scan_token(void) {
  char c = advance();

  switch (c) {
  case '(':
    add_token(LEFT_PAREN);
    break;
  case ')':
    add_token(RIGHT_PAREN);
    break;
  case '{':
    add_token(LEFT_BRACE);
    break;
  case '}':
    add_token(RIGHT_BRACE);
    break;
  case ',':
    add_token(COMMA);
    break;
  case '.':
    add_token(DOT);
    break;
  case '+':
    add_token(PLUS);
    break;
  case '-':
    add_token(MINUS);
    break;
  case '!':
    add_token(match('=') ? BANG_EQUAL : BANG);
    break;
  case '=':
    add_token(match('=') ? EQUAL_EQUAL : EQUAL);
    break;
  case '>':
    add_token(match('=') ? GREATER_EQUAL : GREATER);
    break;
  case '<':
    add_token(match('=') ? LESS_EQUAL : LESS);
    break;
  case '/':
    if (match('/')) {
      while (peek() != '\n' && !is_at_end()) {
        advance();
      }
    } else {
      add_token(SLASH);
    }
    break;
  case ' ':
  case '\t':
  case '\r':
    break;
  case '\n':
    current_line += 1;
    break;
  case '"':
    string();
    break;
  default:
    if (is_digit(c)) {
      number();
    } else if (is_alpha(c)) {
      identifier();
    } else {
      error("Unexpected character.");
    }
    break;
  }
}

static void identifier(void) {
  while (is_alphanumeric(peek())) {
    advance();
  }

  std::string text = source.substr(start + 1, current_pos - start);

  const TokenType *tokentype = get_keyword_token(text);
  if (tokentype == NULL) {
    add_token(IDENTIFIER);
  } else {
    add_token(*tokentype);
  }
}

static void number(void) {
  while (is_digit(peek()))
    advance();
  if (peek() == '.' && is_digit((peek_next())))
    advance();
  while (is_digit(peek()))
    advance();
  add_token_with_literal(NUMBER, source.substr(start + 1, current_pos - start));
}

bool is_digit(char c) { return c >= '0' && c <= '9'; }

void string(void) {
  while (peek() != '"' && !is_at_end()) {
    if (peek() == '\n')
      current_line += 1;
    advance();
  }

  if (is_at_end()) {
    error("Unterminated string.");
    return;
  }

  advance();

  std::string string = source.substr(start + 2, current_pos - start - 2);
  add_token_with_literal(STRING, string);
}

static bool match(char expected) {
  if (is_at_end()) {
    return false;
  }
  if (expected != source[current_pos]) {
    return false;
  }
  current_pos += 1;
  return true;
}

static char peek_next(void) {
  if (current_pos + 1 >= source.length()) {
    return '\0';
  }
  return source[current_pos + 1];
}

static char peek(void) {
  if (is_at_end()) {
    return '\0';
  }
  return source[current_pos];
}
static bool is_alpha(char c) {
  return ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_');
}

static bool is_alphanumeric(char c) { return is_alpha(c) || is_digit(c); }

static bool is_at_end(void) { return current_pos >= source.length(); }

static void error(std::string message) { report("", message); }

static void report(std::string where, std::string message) {
  std::cout << "*[Line " << current_line << "] Error " << where << " : "
            << message << "\n";
  had_error = true;
}
