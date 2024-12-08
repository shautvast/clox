#pragma once

#include "error.hpp"
#include "tokens.hpp"
#include <memory>
#include <variant>
#include <vector>

enum class ExprType { Binary, Grouping, Unary, Literal, None };

using namespace std;

/// Base class for expressions
class Expression {
public:
  virtual string as_string() = 0; // get string rep for debugging
  virtual ~Expression();
};

/// An expression with two operands
class Binary : public Expression {
  unique_ptr<Expression> left;
  unique_ptr<Token> op;
  unique_ptr<Expression> right;

public:
  string as_string() override;
  Binary(Expression *_left, Token *_operator, Expression *_right);
  ~Binary();
};

/// An expression between parentheses
class Grouping : public Expression {
  unique_ptr<Expression> expr;

public:
  string as_string() override;
  Grouping(Expression *_expr);
  ~Grouping();
};

/// An expression with one operand (operator is `-`  or `!`)
class Unary : public Expression {
  unique_ptr<Token> op;
  unique_ptr<Expression> right;

public:
  string as_string() override;
  Unary(Token *_operator, Expression *_right);
  ~Unary();
};

/// empty class that is the type of the Nil value
class NilType {};
typedef std::variant<double_t, bool, string, NilType> Value;

/// encapsulates a value: numeric, string etc
class Literal : public Expression {
public:
  enum ValueType { String, Numeric, Boolean, Nil } valuetype;

  Value value;

  Literal(NilType v) : value(v){};
  Literal(double_t _numeric) : value(_numeric){};
  Literal(string _str) : value(_str){};
  Literal(bool _boolean) : value(_boolean){};

  string as_string() override;
};

class Parser {
  vector<Expression> expressions;
  vector<Token> tokens;
  int current_token;

  /// returns the current token without moving the pointer;
  /// pointer here meanse index into the tokenlist.
  Token peek();
  /// checks if the current token is EOF
  bool is_at_end();
  /// returns the previous token without moving the pointer
  Token *previous();
  /// increments the token pointer
  Token *advance();
  /// checks if the current token is of specified type
  bool check(Token::Type type);
  /// checks if the current token is one of the specified types
  /// count: the number of tokens to match
  /// ... varargs argument for the tokens to match
  bool match(int count, ...);
  /// checks if the current token is of the specified type and
  /// moves the token forward if so, otherwise throws an exception with
  /// the specified message
  Result<Token *> consume(Token::Type typ, string message);
  /// throws an exception for the specified token with the specified message
  Error error(Token token, string message);
  /// tries to parse the token as a primary value (string, number etc)
  Result<Expression *> primary();
  /// tries to parse the tokens as a unary expression
  Result<Expression *> unary();
  /// tries to parse the tokens
  Result<Expression *> expression();
  /// tries to parse the tokens as a multiplication or division
  Result<Expression *> factor();
  /// tries to parse the tokens as an addition or subtraction
  Result<Expression *> term();
  /// tries to parse the tokens as an equality (`a == b` / `a!= b`)
  Result<Expression *> equality();
  /// tries to parse the tokens as a comparison (`a > b` / `a >= b` / `a < b`
  /// / `a <= b` )
  Result<Expression *> comparison();

public:
  /// public method for parsing expressions
  Result<Expression *> parse(vector<Token> tokenlist);
};
