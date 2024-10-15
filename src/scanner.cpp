#include "scanner.hpp"
#include "tokens.hpp"
#include <cstdbool>
#include <iostream>
#include <list>
#include <string>

Scanner::Scanner(std::string s)
    : had_error(false), current_pos(0), start(0), current_line(1), source(s),
      token_list(std::list<Token>()) {}

ScanResult Scanner::scan_tokens() {
  while (current_pos < source.length()) {
    start = current_pos;
    scan_token();
  }

  ScanResult scan_result;
  scan_result.token_list = token_list;
  scan_result.had_error = had_error;

  return scan_result;
}

void Scanner::add_token(TokenType type) {
  Token token;
  token.type = type;
  token.lexeme = source.substr(start, current_pos);
  token.literal = NULL;
  token.line = current_line;

  token_list.push_front(token);
}

void Scanner::add_token_with_literal(TokenType type, std::string literal) {
  Token token;
  token.type = type;
  token.lexeme = source.substr(start, current_pos);
  token.literal = static_cast<void *>(&literal);
  token.line = current_line;

  token_list.push_front(token);
}

char Scanner::advance() {
  char c = source.at(current_pos++);
  return c;
}

void Scanner::scan_token() {
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

void Scanner::identifier() {
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

void Scanner::number() {
  while (is_digit(peek()))
    advance();
  if (peek() == '.' && is_digit((peek_next())))
    advance();
  while (is_digit(peek()))
    advance();
  add_token_with_literal(NUMBER, source.substr(start + 1, current_pos - start));
}

bool Scanner::is_digit(char c) { return c >= '0' && c <= '9'; }

void Scanner::string() {
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

bool Scanner::match(char expected) {
  if (is_at_end()) {
    return false;
  }
  if (expected != source[current_pos]) {
    return false;
  }
  current_pos += 1;
  return true;
}

char Scanner::peek_next() {
  if (current_pos + 1 >= source.length()) {
    return '\0';
  }
  return source[current_pos + 1];
}

char Scanner::peek() {
  if (is_at_end()) {
    return '\0';
  }
  return source[current_pos];
}

bool Scanner::is_alpha(char c) {
  return ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_');
}

bool Scanner::is_alphanumeric(char c) { return is_alpha(c) || is_digit(c); }

bool Scanner::is_at_end(void) { return current_pos >= source.length(); }

void Scanner::error(std::string message) { report("", message); }

void Scanner::report(std::string where, std::string message) {
  std::cout << "*[Line " << current_line << "] Error " << where << " : "
            << message << "\n";
  had_error = true;
}
