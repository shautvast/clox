#include "tokens.h"

typedef struct Expression Expression;

struct Expression {
  char *type;
  Expression *left;
  Expression *right;
  Token *operator;
  char *name;
  void *value;
};

typedef struct ExpressionList {
  Expression **expressions;
  int size;
  int capacity;
} ExpressionList;

ExpressionList parse(TokenList *tokens);

void exprlist_init(ExpressionList *list);

void exprlist_add(ExpressionList *list, Expression *value);

Expression *exprlist_get(ExpressionList *list, int index);

void exprlist_print(ExpressionList *list);

void exprlist_free(ExpressionList *list);
