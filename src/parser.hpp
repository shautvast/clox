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

  Token peek();
  bool is_at_end();
  Token *previous();
  Token *advance();
  bool check(Token::Type type);
  bool match(int count, ...);
  Token *consume(Token::Type typ, string message);
  runtime_error error(Token token, string message);
  Expression *primary();
  Expression *unary();
  Expression *expression();
  Expression *factor();
  Expression *term();
  Expression *equality(void);
  Expression *comparison(void);

public:
  Expression *parse(vector<Token> tokenlist);
};
