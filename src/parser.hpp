#pragma once

#include "tokens.hpp"
#include <cstdarg>
#include <iostream>
#include <vector>

enum class ExprType { Binary, Grouping, Unary, Literal, None };

/// Base class for expressions
class Expression {
public:
  virtual ExprType get_type() = 0;     // abstract, getter for tyoe
  virtual std::string to_string() = 0; // abstract, string rep for debugging
  virtual ~Expression() = 0;           // destructor
};

/// An expression with two operands
class Binary : public Expression {
public:
  ExprType get_type() override { return ExprType::Binary; }
  std::string to_string() override {
    return "(" + token_name(op->tokentype) + " " + left->to_string() + " " +
           right->to_string() + ")";
  }
  Expression *left;
  Token *op;
  Expression *right;
  Binary(Expression *_left, Token *_operator, Expression *_right)
      : left(_left), op(_operator), right(_right){};
  ~Binary() override {
    delete left;
    delete right;
    delete op;
  }
};

/// An expression between parentheses
class Grouping : public Expression {
public:
  ExprType get_type() override { return ExprType::Grouping; }
  std::string to_string() override { return "(" + expr->to_string() + ")"; }
  Expression *expr;
  Grouping(Expression *_expr) : expr(_expr){};
  ~Grouping() override { delete expr; }
};

/// An expression with one operand (operator is `-`  or `!`)
class Unary : public Expression {
public:
  ExprType get_type() override { return ExprType::Unary; }
  std::string to_string() override {
    return token_name(op->tokentype) + right->to_string();
  }
  Token *op;
  Expression *right;

  Unary(Token *_operator, Expression *_right) : op(_operator), right(_right){};
  ~Unary() override {
    delete right;
    delete op;
  }
};

/// empty class that is the type of the Nil value
class Void {};

/// encapsulates a value: numeric, string etc
class Literal : public Expression {
public:
  enum ValueType { String, Numeric, Boolean, Nil } valuetype;

  ExprType get_type() override { return ExprType::Literal; }

  union Value {
    double_t numeric;
    bool boolean;
    std::string str;
    Void dummy;

    Value(double_t _numeric) : numeric(_numeric) {}
    Value(bool _boolean) : boolean(_boolean) {}
    Value(std::string _str) : str(_str) {}
    Value(Void v) : dummy(v) {}
    ~Value() {}
  } value;

  Literal(Void v) : valuetype(ValueType::Nil), value(v) {}
  Literal(double_t _numeric) : valuetype(ValueType::Numeric), value(_numeric) {}
  Literal(std::string _str) : valuetype(ValueType::String), value(_str) {}
  Literal(bool _boolean) : valuetype(ValueType::Boolean), value(_boolean) {}

  std::string to_string() override {
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
};

class Parser {
private:
  std::vector<Expression> expressions;
  std::vector<Token> tokens;
  int current_token;

  Token peek() { return tokens[current_token]; };

  bool is_at_end() { return peek().tokentype == Token::END_OF_FILE; };

  Token *previous() { return &tokens[current_token - 1]; };

  Token *advance() {
    if (!is_at_end())
      current_token += 1;
    return previous();
  }

  bool check(Token::Type type) {
    if (is_at_end()) {
      return false;
    }
    return peek().tokentype == type;
  }

  bool match(int count, ...) {
    va_list list;
    va_start(list, count);

    for (int i = 0; i < count; i++) {
      Token::Type ttc = va_arg(list, Token::Type);
      // std::cout << token_name(ttc) << "\n";
      if (check(ttc)) {
        advance();
        return true;
      }
    }
    return false;
  };

  Token *consume(Token::Type typ, std::string message) {
    if (check(typ)) {
      return advance();
    }
    throw error(peek(), message);
  }

  std::runtime_error error(Token token, std::string message) {
    std::cout << token.to_string() << " " << message;
    return std::runtime_error(message);
  }

  Expression *primary() {
    if (match(1, Token::Type::FALSE))
      return new Literal(false);
    if (match(1, Token::Type::TRUE))
      return new Literal(true);
    if (match(1, Token::Type::NIL))
      return new Literal(new Void());
    if (match(1, Token::Type::NUMBER)) {
      return new Literal(std::stod(previous()->literal));
    }
    if (match(1, Token::Type::STRING)) {
      return new Literal(previous()->literal);
    }
    if (match(1, Token::Type::LEFT_PAREN)) {
      Expression *e = expression();
      consume(Token::Type::RIGHT_PAREN, "Expect ')'.");
      return new Grouping(e);
    }
    throw std::runtime_error("Expected an expression");
  }

public:
  Expression *parse(std::vector<Token> tokenlist);
  Expression *unary() {
    if (match(2, Token::BANG, Token::Type::MINUS)) {
      Token *op = previous();
      Expression *right = unary();
      return new Unary(op, right);
    }
    return primary();
  }

  Expression *expression() { return equality(); }

  Expression *factor() {
    Expression *expr = unary();
    while (match(2, Token::Type::SLASH, Token::Type::STAR)) {
      Token *op = previous();
      Expression *right = unary();
      expr = new Binary(expr, op, right);
    }
    return expr;
  }

  Expression *term() {
    Expression *expr = factor();
    while (match(2, Token::Type::MINUS, Token::Type::PLUS)) {
      Token *op = previous();
      Expression *right = unary();
      expr = new Binary(expr, op, right);
    }
    return expr;
  }

  Expression *equality(void) {
    Expression *expr = comparison();

    while (match(2, Token::Type::BANG_EQUAL, Token::Type::BANG_EQUAL)) {
      Token *op = previous();
      Expression *right = comparison();
      return new Binary(expr, op, right);
    }
    return expr;
  }

  Expression *comparison(void) {
    Expression *expr = term();
    while (match(4, Token::Type::GREATER, Token::Type::GREATER_EQUAL,
                 Token::Type::LESS, Token::Type::LESS_EQUAL)) {
      Token *op = previous();
      Expression *right = term();
      expr = new Binary(expr, op, right);
    }
    return expr;
  }
};
