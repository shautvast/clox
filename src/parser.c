#include "parser.h"
#include "tokens.h"

enum ExprType {
  Bin,
};

struct Binary {
  Binary *left;
  Token operator;
  Binary *right;
};

union Expr {
  Binary binary;
};

struct Expression {
  enum ExprType type;
  union Expr expr;
};
