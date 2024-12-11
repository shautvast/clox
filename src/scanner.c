#include "scanner.h"
#include "tokens.h"
#include "utils.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void scan_token(void);
static void error(char *message, char c);
static void report(char *where, char *message, char c);
static bool is_at_end(void);
static bool match(char expected);
static char peek(void);
static char peek_next(void);
static void string(void);
static bool is_digit(char c);
static void number(void);
static bool is_alpha(char c);
static bool is_alphanumeric(char c);
static void identifier(void);

static bool had_error = false;
static int current_pos = -1;
static int start = -1;
static int current_line = -1;
static char *source;
static TokenList token_list;

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
  Token *eof = newToken();
  eof->type = END_OF_FILE;
  eof->lexeme = "";
  eof->literal = "";

  tokenlist_add(&token_list, eof);

  ScanResult scan_result;
  scan_result.token_list = token_list;
  scan_result.had_error = had_error;

  // tokenlist_print(&scan_result.token_list);

  return scan_result;
}

static void add_token(TokenType type) {
  Token *token = newToken();
  token->type = type;
  token->lexeme = substring(source, start + 1, current_pos - start);
  token->literal = NULL;
  token->line = current_line;

  tokenlist_add(&token_list, token);
}

static void add_token_with_literal(TokenType type, char *literal) {
  Token *token = newToken();
  token->type = type;
  token->lexeme = substring(source, start + 1, current_pos - start);
  token->literal = literal;
  token->line = current_line;

  tokenlist_add(&token_list, token);
}

static char advance(void) {
  char c = source[current_pos++];
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
  case ';':
    add_token(SEMICOLON);
    break;
  default:
    if (is_digit(c)) {
      number();
    } else if (is_alpha(c)) {
      identifier();
    } else {
      error("Unexpected character.", c);
    }
    break;
  }
}

static void identifier(void) {
  while (is_alphanumeric(peek())) {
    advance();
  }

  char *text = substring(source, start + 1, current_pos - start);
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
    error("Unterminated string.", '\0');
    return;
  }

  advance();

  char *string = substring(source, start + 2, current_pos - start - 2);
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
  if (current_pos + 1 >= (int)strlen(source)) {
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

static bool is_at_end(void) { return current_pos >= (int)strlen(source); }

static void error(char *message, char c) { report("", message, c); }

static void report(char *where, char *message, char c) {
  printf("*[Line %i] Error %s : %s [%c]\n", current_line, where, message, c);
  had_error = true;
}
