#include "parser.hpp"

Expression *Parser::parse(std::vector<Token> tokenlist) {
  tokens = tokenlist;
  current_token = 0;
  return expression();
}
