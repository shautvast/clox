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

void Scanner::add_token(Token::Type type) {
  Token token =
      Token(type, source.substr(start, current_pos), "", current_line);
  token_list.push_front(token);
}

void Scanner::add_token_with_literal(Token::Type type, std::string literal) {
  Token token =
      Token(type, source.substr(start, current_pos), literal, current_line);
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
    add_token(Token::Type::LEFT_PAREN);
    break;
  case ')':
    add_token(Token::Type::RIGHT_PAREN);
    break;
  case '{':
    add_token(Token::Type::LEFT_BRACE);
    break;
  case '}':
    add_token(Token::Type::RIGHT_BRACE);
    break;
  case ',':
    add_token(Token::Type::COMMA);
    break;
  case '.':
    add_token(Token::Type::DOT);
    break;
  case '+':
    add_token(Token::Type::PLUS);
    break;
  case '-':
    add_token(Token::Type::MINUS);
    break;
  case '!':
    add_token(match('=') ? Token::Type::BANG_EQUAL : Token::Type::BANG);
    break;
  case '=':
    add_token(match('=') ? Token::Type::EQUAL_EQUAL : Token::Type::EQUAL);
    break;
  case '>':
    add_token(match('=') ? Token::Type::GREATER_EQUAL : Token::Type::GREATER);
    break;
  case '<':
    add_token(match('=') ? Token::Type::LESS_EQUAL : Token::Type::LESS);
    break;
  case '/':
    if (match('/')) {
      while (peek() != '\n' && !is_at_end()) {
        advance();
      }
    } else {
      add_token(Token::Type::SLASH);
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

  const Token::Type *tokentype = get_keyword_token(text);
  if (tokentype == NULL) {
    add_token(Token::Type::IDENTIFIER);
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
  add_token_with_literal(Token::Type::NUMBER,
                         source.substr(start + 1, current_pos - start));
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

  std::string string = source.substr(start + 1, current_pos - start - 2);
  add_token_with_literal(Token::Type::STRING, string);
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
