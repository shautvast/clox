#include "parser.hpp"

#include <iostream>
#include <memory>

using std::string, std::to_string, std::vector;

Expression::~Expression() = default;

// class Binary

string Binary::as_string() const {
  return "(" + token_name(op->tokentype) + " " + left->as_string() + " " +
         right->as_string() + ")";
}

Binary::Binary(Expression *_left, Token *_operator, Expression *_right)
    : left(_left), op(_operator), right(_right){};

Binary::~Binary() = default;

// class Grouping

string Grouping::as_string() const { return "(" + expr->as_string() + ")"; }

Grouping::Grouping(Expression *_expr) : expr(_expr){};

Grouping::~Grouping() = default;

// class Unary

string Unary::as_string() const {
  return token_name(op->tokentype) + right->as_string();
}

Unary::Unary(Token *_operator, Expression *_right)
    : op(_operator), right(_right){};

Unary::~Unary() = default;

// class Literal
string Literal::as_string() const {
  string text;
  if (holds_alternative<string>(value)) {
    return "\"" + get<string>(value) + "\"";
  }
  if (holds_alternative<double_t>(value)) {
    return to_string(get<double_t>(value));
  }
  if (holds_alternative<bool>(value)) {
    return get<bool>(value) ? "True" : "False";
  }
  if (holds_alternative<NilType>(value)) {
    return "NULL";
  }
  return "unexpected";
}

// class Parser
Result<Expression *> Parser::parse(vector<Token> tokenlist) {
  tokens = tokenlist;
  current_token = 0;
  return expression();
}

Token Parser::peek() { return tokens[current_token]; };

bool Parser::is_at_end() { return peek().tokentype == Token::END_OF_FILE; };

Token *Parser::previous() { return &tokens[current_token - 1]; };

Token *Parser::advance() {
  if (!is_at_end())
    current_token += 1;
  return previous();
}

bool Parser::check(Token::Type type) {
  if (is_at_end()) {
    return false;
  }
  return peek().tokentype == type;
}

bool Parser::match(int count, ...) {
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

Result<Token *> Parser::consume(Token::Type typ, string message) {
  if (check(typ)) {
    return advance();
  }
  return error(peek(), message);
}

Error Parser::error(Token token, string message) {
  std::cout << token.as_string() << " " << message;
  return Error(message); // TODO no exceptions
}

Result<Expression *> Parser::primary() {
  if (match(1, Token::Type::FALSE))
    return new Literal(false);
  if (match(1, Token::Type::TRUE))
    return new Literal(true);
  if (match(1, Token::Type::NIL))
    return new Literal(new NilType());
  if (match(1, Token::Type::NUMBER)) {
    return new Literal(stod(previous()->literal));
  }
  if (match(1, Token::Type::STRING)) {
    return new Literal(previous()->literal);
  }
  if (match(1, Token::Type::LEFT_PAREN)) {
    auto expr = expression();
    Result<Token *> r = consume(Token::Type::RIGHT_PAREN, "Expect ')'.");
    if (is_err(r)) {
      return Err(r);
    }
    return new Grouping(Ok(expr));
  }
  return Error("Expected an expression");
}

Result<Expression *> Parser::unary() {
  if (match(2, Token::BANG, Token::Type::MINUS)) {
    Token *op = previous();
    Result<Expression *> right = unary();
    if (is_ok(right)) {
      return new Unary(op, Ok(right));
    }
  }
  return primary();
}

Result<Expression *> Parser::expression() { return equality(); }

Result<Expression *> Parser::factor() {
  Result<Expression *> expr = unary();
  if (is_err(expr)) {
    return expr;
  }
  while (match(2, Token::Type::SLASH, Token::Type::STAR)) {
    Token *op = previous();
    auto right = unary();
    if (is_err(right)) {
      return right;
    }
    expr = new Binary(Ok(expr), op, Ok(right));
  }
  return expr;
}

Result<Expression *> Parser::term() {
  auto expr = factor();
  if (is_err(expr)) {
    return expr;
  }
  while (match(2, Token::Type::MINUS, Token::Type::PLUS)) {
    Token *op = previous();
    auto right = factor();
    if (is_err(right)) {
      return right;
    }
    expr = new Binary(Ok(expr), op, Ok(right));
  }
  return expr;
}

Result<Expression *> Parser::equality(void) {
  auto expr = comparison();
  if (is_err(expr)) {
    return expr;
  }
  while (match(2, Token::Type::BANG_EQUAL, Token::Type::BANG_EQUAL)) {
    Token *op = previous();
    auto right = comparison();
    if (is_err(right)) {
      return right;
    }
    return new Binary(Ok(expr), op, Ok(right));
  }
  return expr;
}

Result<Expression *> Parser::comparison(void) {
  auto expr = term();
  if (is_err(expr)) {
    return expr;
  }
  while (match(4, Token::Type::GREATER, Token::Type::GREATER_EQUAL,
               Token::Type::LESS, Token::Type::LESS_EQUAL)) {
    Token *op = previous();
    auto right = term();
    if (is_err(right)) {
      return right;
    }
    expr = new Binary(Ok(expr), op, Ok(right));
  }
  return expr;
}
