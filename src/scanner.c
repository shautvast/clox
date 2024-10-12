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
  default:
    error("Unexpected character.");
    break;
  }
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

bool is_at_end(void) { return current_pos >= (int)strlen(source); }

void error(char *message) { report("", message); }

void report(char *where, char *message) {
  printf("*[Line %i] Error %s : %s\n", current_line, where, message);
  had_error = true;
}
