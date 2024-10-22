#include "parser.hpp"

using namespace std;

Expression::~Expression() {}

// class Binary
ExprType Binary::get_type() { return ExprType::Binary; }

string Binary::as_string() {
  return "(" + token_name(op->tokentype) + " " + left->as_string() + " " +
         right->as_string() + ")";
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

string Grouping::as_string() { return "(" + expr->as_string() + ")"; }

Grouping::Grouping(Expression *_expr) : expr(_expr){};

Grouping::~Grouping() { delete expr; }

// class Unary
ExprType Unary::get_type() { return ExprType::Unary; }

string Unary::as_string() {
  return token_name(op->tokentype) + right->as_string();
}

Unary::Unary(Token *_operator, Expression *_right)
    : op(_operator), right(_right){};

Unary::~Unary() {
  delete right;
  delete op;
}

// class Literal
string Literal::as_string() {
  string text;
  switch (valuetype) {
  case String:
    text = "\"" + value.str + "\"";
    break;
  case Numeric:
    text = to_string(value.numeric);
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

Expression *Parser::parse(vector<Token> tokenlist) {
  tokens = tokenlist;
  current_token = 0;
  return expression();
}
