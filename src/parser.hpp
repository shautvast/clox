#pragma once

#include "tokens.hpp"
#include <cstdarg>
#include <iostream>
#include <vector>

enum class ExprType { Binary, Grouping, Unary, Literal, None };

using namespace std;

/// Base class for expressions
class Expression {
public:
  virtual ExprType get_type() = 0; // abstract, getter for tyoe
  virtual string as_string() = 0;  // abstract, string rep for debugging
  virtual ~Expression();           // destructor
};                                 // namespace stdclass Expression

/// An expression with two operands
class Binary : public Expression {
  Expression *left;
  Token *op;
  Expression *right;

public:
  ExprType get_type() override;
  string as_string() override;
  Binary(Expression *_left, Token *_operator, Expression *_right);
  ~Binary();
};

/// An expression between parentheses
class Grouping : public Expression {
  Expression *expr;

public:
  ExprType get_type() override;
  string as_string() override;
  Grouping(Expression *_expr);
  ~Grouping();
};

/// An expression with one operand (operator is `-`  or `!`)
class Unary : public Expression {
  Token *op;
  Expression *right;

public:
  ExprType get_type() override;
  string as_string() override;
  Unary(Token *_operator, Expression *_right);
  ~Unary();
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
    string str;
    Void dummy;

    Value(double_t _numeric) : numeric(_numeric) {}
    Value(bool _boolean) : boolean(_boolean) {}
    Value(string _str) : str(_str) {}
    Value(Void v) : dummy(v) {}
    ~Value() {}
  } value;

  Literal(Void v) : valuetype(ValueType::Nil), value(v){};
  Literal(double_t _numeric) : valuetype(ValueType::Numeric), value(_numeric){};
  Literal(string _str) : valuetype(ValueType::String), value(_str){};
  Literal(bool _boolean) : valuetype(ValueType::Boolean), value(_boolean){};

  string as_string() override;
};

class Parser {
  vector<Expression> expressions;
  vector<Token> tokens;
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
      // cout << token_name(ttc) << "\n";
      if (check(ttc)) {
        advance();
        return true;
      }
    }
    return false;
  };

  Token *consume(Token::Type typ, string message) {
    if (check(typ)) {
      return advance();
    }
    throw error(peek(), message);
  }

  runtime_error error(Token token, string message) {
    cout << token.as_string() << " " << message;
    return runtime_error(message); // TODO no exceptions
  }

  Expression *primary() {
    if (match(1, Token::Type::FALSE))
      return new Literal(false);
    if (match(1, Token::Type::TRUE))
      return new Literal(true);
    if (match(1, Token::Type::NIL))
      return new Literal(new Void());
    if (match(1, Token::Type::NUMBER)) {
      return new Literal(stod(previous()->literal));
    }
    if (match(1, Token::Type::STRING)) {
      return new Literal(previous()->literal);
    }
    if (match(1, Token::Type::LEFT_PAREN)) {
      Expression *e = expression();
      consume(Token::Type::RIGHT_PAREN, "Expect ')'.");
      return new Grouping(e);
    }
    throw runtime_error("Expected an expression");
  }

public:
  Expression *parse(vector<Token> tokenlist);
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
