#include "tokens.hpp"

using std::string;

Token::Token(Token::Type _tokentype, string _lexeme, string _literal, int _line)
    : lexeme(_lexeme), literal(_literal), line(_line), tokentype(_tokentype) {}

string token_name(Token::Type tokentype) {
  static const std::string tokens[] = {
      "EOF",    "(",      ")",   "{",     "}",     ",",      "*",
      "-",      "+",      ";",   "/",     "*",     "!",      "!=",
      "=",      "==",     ">",   ">=",    "<",     "<=",     "IDENTIFIER",
      "string", "number", "and", "class", "else",  "false",  "fun",
      "for",    "if",     "Nil", "or",    "print", "return", "super",
      "this",   "true",   "var", "while"};
  return tokens[(int)tokentype];
}

std::string Token::as_string() { return token_name(tokentype); }
