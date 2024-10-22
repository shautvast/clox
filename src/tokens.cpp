#include "tokens.hpp"

using namespace std;

Token::Token(Token::Type _tokentype, string _lexeme, string _literal, int _line)
    : lexeme(_lexeme), literal(_literal), line(_line), tokentype(_tokentype) {}

string token_name(Token::Type tokentype) {
  static const std::string tokens[] = {
      "END_OF_FILE", "LEFT_PAREN", "RIGHT_PAREN",   "LEFT_BRACE", "RIGHT_BRACE",
      "COMMA",       "DOT",        "MINUS",         "PLUS",       "SEMICOLON",
      "SLASH",       "STAR",       "BANG",          "BANG_EQUAL", "EQUAL",
      "EQUAL_EQUAL", "GREATER",    "GREATER_EQUAL", "LESS",       "LESS_EQUAL",
      "IDENTIFIER",  "STRING",     "NUMBER",        "AND",        "CLASS",
      "ELSE",        "FALSE",      "FUN",           "FOR",        "IF",
      "NIL",         "OR",         "PRINT",         "RETURN",     "SUPER",
      "THIS",        "TRUE",       "VAR",           "WHILE"};
  return tokens[(int)tokentype];
}

std::string Token::as_string() { return token_name(tokentype); }
