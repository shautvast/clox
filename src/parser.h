#ifndef PARSER_H
#define PARSER_H

#include "tokens.h"

typedef struct Expression Expression;
typedef struct ExpressionList ExpressionList;

typedef union ValueHolder {
  double number;
  char *string;
  bool boolean;
  Expression *expr;
} ValueHolder;

typedef enum { NUMBERTYPE, STRINGTYPE, BOOLEANTYPE, EXPR } Type;

typedef struct Value {
  Type type;
  ValueHolder value;
} Value;

struct Expression {
  char *type;
  Expression *left;
  Expression *right;
  Token *operator;
  char *name;
  Value *value;
  ExpressionList *block;
};

typedef struct ExpressionList {
  Expression **expressions;
  int size;
  int capacity;
} ExpressionList;

const char *value_string(Value *v);
Value *newValue(void);
Value *newString(char *string);
Value *newNumber(double number);
Value *newBoolean(bool boolean);

ExpressionList *parse(TokenList *tokens);
ExpressionList *newExpressionList();
// void exprlist_init(ExpressionList *list);

void exprlist_add(ExpressionList *list, Expression *value);

Expression *exprlist_get(ExpressionList *list, int index);

void exprlist_print(ExpressionList *list);

void exprlist_free(ExpressionList *list);
#endif
