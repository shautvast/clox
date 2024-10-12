#include "scanner.h"
#include "tokens.h"
#include "utils.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void scan_token(void);
void error(char *message);
void report(char *where, char *message);
bool is_at_end(void);
bool match(char expected);
char peek(void);
char peek_next(void);
void string(void);
bool is_digit(char c);
void number(void);

bool had_error = false;
int current_pos = -1;
int start = -1;
int current_line = -1;
char *source;
TokenList token_list;

ScanResult scan_tokens(char *src) {
  current_pos = 0;
  start = 0;
  current_line = 1;
  source = src;

  tokenlist_init(&token_list);
  int len = (int)strlen(source);

  while (current_pos < len) {
    start = current_pos;
    scan_token();
  }

  ScanResult scan_result;
  scan_result.token_list = token_list;
  scan_result.had_error = had_error;

  // tokenlist_print(&scan_result.token_list);

  return scan_result;
}

void add_token(enum TokenType type) {
  Token token;
  token.type = type;
  token.lexeme = substring(source, start, current_pos);
  token.literal = NULL;
  token.line = current_line;

  tokenlist_add(&token_list, token);
}

void add_token_with_literal(enum TokenType type, char *literal) {
  Token token;
  token.type = type;
  token.lexeme = substring(source, start, current_pos);
  token.literal = literal;
  token.line = current_line;

  tokenlist_add(&token_list, token);
}

char advance(void) {
  char c = source[current_pos++];
  return c;
}

void scan_token(void) {
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
    } else {
      error("Unexpected character.");
    }
    break;
  }
}

void number(void) {
  while (is_digit(peek()))
    advance();
  if (peek() == '.' && is_digit((peek_next())))
    advance();
  while (is_digit(peek()))
    advance();
  add_token_with_literal(NUMBER,
                         substring(source, start + 1, current_pos - start));
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

  char *string = substring(source, start + 2, current_pos - start - 2);
  add_token_with_literal(STRING, string);
}

bool match(char expected) {
  if (is_at_end()) {
    return false;
  }
  if (expected != source[current_pos]) {
    return false;
  }
  current_pos += 1;
  return true;
}

char peek_next(void) {
  if (current_pos + 1 >= (int)strlen(source)) {
    return '\0';
  }
  return source[current_pos + 1];
}

char peek(void) {
  if (is_at_end()) {
    return '\0';
  }
  return source[current_pos];
}

bool is_at_end(void) { return current_pos >= (int)strlen(source); }

void error(char *message) { report("", message); }

void report(char *where, char *message) {
  printf("*[Line %i] Error %s : %s\n", current_line, where, message);
  had_error = true;
}
