#include "tokens.hpp"

Token::Token(Token::Type _tokentype, std::string _lexeme, std::string _literal,
             int _line)
    : lexeme(_lexeme), literal(_literal), line(_line), tokentype(_tokentype) {}

std::string Token::to_string() {
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
