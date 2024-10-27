#include "scanner.hpp"
#include "error.hpp"
#include "tokens.hpp"
#include <cstdbool>
#include <iostream>
#include <map>
#include <string>
#include <vector>

using std::string, std::map, std::vector, std::cout;

static const map<string, Token::Type> keywords = {
    {"and", Token::Type::AND},       {"class", Token::Type::CLASS},
    {"else", Token::Type::ELSE},     {"false", Token::Type::FALSE},
    {"for", Token::Type::FOR},       {"fun", Token::Type::FUN},
    {"if", Token::Type::IF},         {"nil", Token::Type::NIL},
    {"or", Token::Type::OR},         {"print", Token::Type::PRINT},
    {"return", Token::Type::RETURN}, {"super", Token::Type::SUPER},
    {"this", Token::Type::THIS},     {"true", Token::Type::TRUE},
    {"var", Token::Type::VAR},       {"while", Token::Type::WHILE},
};

Scanner::Scanner(string _source)
    : had_error(false), current_pos(0), start(0), current_line(1),
      source(_source), token_list(vector<Token>()) {}

Result<vector<Token>> Scanner::scan_tokens() {
  while (current_pos < source.length()) {
    start = current_pos;
    Result<Void> r = scan_token();
    if (is_err(r)) {
      return Err(r);
    }
  }
  return token_list;
}

void Scanner::add_token(Token::Type type) {
  Token token = Token(type, "", "", current_line);
  token_list.push_back(token);
}

void Scanner::add_token(Token::Type type, string literal) {
  Token token = Token(type, literal, literal, current_line);
  token_list.push_back(token);
}

char Scanner::advance() {
  char c = source.at(current_pos++);
  return c;
}

Result<Void> Scanner::scan_token() {
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
  case '*':
    add_token(Token::Type::STAR);
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
    scan_string();
    break;
  default:
    if (is_digit(c)) {
      number();
    } else if (is_alpha(c)) {
      identifier();
    } else {
      return Error{"Unexpected character."};
    }
    break;
  }
  return Void{};
}

void Scanner::identifier() {
  while (is_alphanumeric(peek())) {
    advance();
  }

  string text = source.substr(start, current_pos - start);
  auto it = keywords.find(text);
  if (it != keywords.end()) {
    add_token(it->second, text);
  } else {
    add_token(Token::Type::IDENTIFIER, text);
  }
}

void Scanner::number() {
  while (is_digit(peek()))
    advance();
  if (peek() == '.' && is_digit((peek_next())))
    advance();
  while (is_digit(peek()))
    advance();
  add_token(Token::Type::NUMBER, source.substr(start, current_pos - start));
}

bool Scanner::is_digit(char c) { return c >= '0' && c <= '9'; }

void Scanner::scan_string() {
  while (peek() != '"' && !is_at_end()) {
    if (peek() == '\n')
      current_line += 1;
    advance();
  }

  if (is_at_end()) {
    report("Unterminated string.");
    return;
  }

  advance();

  string s = source.substr(start + 1, current_pos - start - 2);
  add_token(Token::Type::STRING, s);
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

void Scanner::report(string message) { report("", message); }

void Scanner::report(string where, std::string message) {
  cout << "*[Line " << current_line << "] Error " << where << " : " << message
       << "\n";
  had_error = true;
}
