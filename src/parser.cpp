#include "parser.hpp"

Expression::~Expression() {}

// class Binary
ExprType Binary::get_type() { return ExprType::Binary; }

std::string Binary::to_string() {
  return "(" + token_name(op->tokentype) + " " + left->to_string() + " " +
         right->to_string() + ")";
}

Binary::Binary(Expression *_left, Token *_operator, Expression *_right)
    : left(_left), op(_operator), right(_right){};

Binary::~Binary() {
  delete left;
  delete right;
  delete op;
}

// class Grouping
ExprType Grouping::get_type() { return ExprType::Grouping; }

std::string Grouping::to_string() { return "(" + expr->to_string() + ")"; }

Grouping::Grouping(Expression *_expr) : expr(_expr){};

Grouping::~Grouping() { delete expr; }

// class Unary
ExprType Unary::get_type() { return ExprType::Unary; }

std::string Unary::to_string() {
  return token_name(op->tokentype) + right->to_string();
}

Unary::Unary(Token *_operator, Expression *_right)
    : op(_operator), right(_right){};

Unary::~Unary() {
  delete right;
  delete op;
}

// class Literal
std::string Literal::to_string() {
  std::string text;
  switch (valuetype) {
  case String:
    text = "\"" + value.str + "\"";
    break;
  case Numeric:
    text = std::to_string(value.numeric);
    break;
  case Boolean:
    text = value.boolean ? "True" : "False";
    break;
  case Nil:
    text = "NULL";
    break;
  }
  return text;
}

Expression *Parser::parse(std::vector<Token> tokenlist) {
  tokens = tokenlist;
  current_token = 0;
  return expression();
}
