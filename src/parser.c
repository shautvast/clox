#include "parser.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

bool is_at_end(void);
Token *peek(void);
void expr_print(const Expression *expr);
Expression *declaration(void);
bool match1(TokenType t);
bool match2(TokenType type1, TokenType type2);
bool match3(TokenType type1, TokenType type2, TokenType type3);
bool match4(TokenType type1, TokenType type2, TokenType type3, TokenType type4);
bool check(TokenType t);
Token *advance(void);
Token *previous(void);
Expression *var_declaration(void);
Expression *expression(void);
Expression *statement(void);
Expression *printStatement(void);
ExpressionList *parse_block(void);
Expression *expressionStatement(void);
Expression *assignment(void);
Expression *equality(void);
Expression *comparison(void);
Expression *term(void);
Expression *factor(void);
Expression *unary(void);
Expression *primary(void);
Expression *newExpression(char *type);
Token *consume(TokenType type, char *message);

static TokenList *tokens;
static int current;

ExpressionList *parse(TokenList *tokens_to_parse) {
  ExpressionList *statements = newExpressionList();

  tokens = tokens_to_parse;
  current = 0;

  while (!is_at_end()) {
    exprlist_add(statements, declaration());
  }
  return statements;
}

Expression *declaration(void) {
  if (match1(VAR)) {
    return var_declaration();
  } else {
    return statement();
  }
}

Expression *var_declaration(void) {
  Token *name = consume(IDENTIFIER, "Expected a variable name");
  Expression *initializer = NULL;
  if (match1(EQUAL)) {
    initializer = expression();
  }
  consume(SEMICOLON, "Expected semicolon");
  Expression *variableStatement = newExpression("VariableStmt");
  variableStatement->name = name->lexeme;
  variableStatement->left = initializer;
  return variableStatement;
}

Expression *statement(void) {
  if (match1(PRINT)) {
    return printStatement();
  }
  if (match1(LEFT_BRACE)) {
    Expression *block = newExpression("Block");

    ExpressionList *block_statements = newExpressionList();

    while (!check(RIGHT_BRACE) && !is_at_end()) {
      exprlist_add(block_statements, declaration());
    }
    advance();

    block->block = block_statements;
    return block;
  }

  return expressionStatement();
}

Expression *printStatement(void) {
  Expression *value = expression();
  consume(SEMICOLON, "Expected semicolon");
  Expression *print = newExpression("PrintStmt");
  print->left = value;
  return print;
}

Expression *expressionStatement(void) {
  Expression *value = expression();
  consume(SEMICOLON, "Expected semicolon");
  Expression *statement = newExpression("ExprStmt");
  statement->left = value;
  return statement;
}

Expression *expression(void) { return assignment(); }

Expression *assignment(void) {
  Expression *expr = equality();
  if (match1(EQUAL)) {
    Token *equals = previous();
    Expression *value = assignment();
    if (strcmp(expr->type, "Variable") == 0) {
      Expression *assign = newExpression("AssignStmt");
      assign->name = expr->name;
      assign->left = value;
      return assign;
    }
    Expression *error = newExpression("Error");
    error->operator= equals;
    return error;
  }
  return expr;
}

Expression *equality(void) {
  Expression *expr = comparison();
  while (match2(BANG_EQUAL, EQUAL_EQUAL)) {
    Token *operator= previous();
    Expression *right = comparison();
    Expression *binary = newExpression("BinaryExpr");
    binary->operator= operator;
    binary->left = expr;
    binary->right = right;
    expr = binary;
  }
  return expr;
}

Expression *comparison(void) {
  Expression *expr = term();
  while (match4(GREATER, GREATER_EQUAL, LESS, LESS_EQUAL)) {
    Token *operator= previous();
    Expression *right = term();
    Expression *binary = newExpression("BinaryExpr");
    binary->operator= operator;
    binary->left = expr;
    binary->right = right;
    expr = binary;
  }
  return expr;
}

Expression *term(void) {
  Expression *expr = factor();
  while (match2(MINUS, PLUS)) {
    Token *operator= previous();
    Expression *right = factor();
    Expression *binary = newExpression("BinaryExpr");
    binary->operator= operator;
    binary->left = expr;
    binary->right = right;
    expr = binary;
  }
  return expr;
}

Expression *factor(void) {
  Expression *expr = unary();
  while (match2(SLASH, STAR)) {
    Token *operator= previous();
    Expression *right = unary();
    Expression *binary = newExpression("BinaryExpr");
    binary->operator= operator;
    binary->left = expr;
    binary->right = right;
    expr = binary;
  }
  return expr;
}

Expression *unary(void) {
  if (match2(BANG, MINUS)) {
    Token *operator= previous();
    Expression *right = unary();
    Expression *unary = newExpression("Unary");
    unary->operator= operator;
    unary->right = right;
    return unary;
  }
  return primary();
}

Expression *primary(void) {
  Expression *r = newExpression("Literal");
  if (match1(FALSE)) {
    r->value = newBoolean(false);
    return r;
  }
  if (match1(TRUE)) {
    r->value = newBoolean(true);
    return r;
  }

  if (match1(NIL)) {
    r->name = "nil";
    r->value = NULL;
    return r;
  }

  if (check(NUMBER)) {
    advance();
    r->value = newNumber(strtod(previous()->literal, NULL));
    return r;
  }
  if (check(STRING)) {
    advance();
    r->value = newString(previous()->literal);
    return r;
  }
  if (match1(IDENTIFIER)) {
    Expression *var = newExpression("Variable");
    var->name = previous()->lexeme;
    return var;
  }
  if (match1(LEFT_PAREN)) {
    Expression *expr = expression();
    Expression *group = newExpression("Group");
    consume(RIGHT_PAREN, "Expect ')' after expression.");
    group->left = expr;
    return group;
  }

  Expression *error = newExpression("Error");
  return error;
}

Token *consume(TokenType type, char *message) {
  // printf("%s==%s\n", token_name(type), token_name(peek()->type));
  if (check(type)) {
    return advance();
  }

  Token *t = newToken();
  t->type = ERROR;
  t->lexeme = message;

  tokenlist_add(tokens, t);
  return tokenlist_last(tokens);
}

bool match1(TokenType type) {
  if (check(type)) {
    advance();
    return true;
  } else {
    return false;
  }
}

bool match2(TokenType type1, TokenType type2) {
  if (check(type1) || check(type2)) {
    advance();
    return true;
  } else {
    return false;
  }
}

bool match3(TokenType type1, TokenType type2, TokenType type3) {
  if (check(type1) || check(type2) || check(type3)) {
    advance();
    return true;
  } else {
    return false;
  }
}

bool match4(TokenType type1, TokenType type2, TokenType type3,
            TokenType type4) {
  if (check(type1) || check(type2) || check(type3) || check(type4)) {
    advance();
    return true;
  } else {
    return false;
  }
}

Token *advance(void) {
  if (!is_at_end()) {
    current += 1;
  }
  return previous();
}

Token *previous(void) { return tokenlist_get(tokens, current - 1); }

bool check(TokenType type) { return peek()->type == type; }

bool is_at_end(void) { return peek()->type == END_OF_FILE; }

Token *peek(void) { return tokenlist_get(tokens, current); }

ExpressionList *newExpressionList() {
  ExpressionList *list = malloc(sizeof(ExpressionList));
  if (list == NULL) {
    printf("Cannot allocate memory for ExpressionList");
    exit(1);
  }

  list->expressions = malloc(sizeof(Expression) * 32);
  if (list->expressions == NULL) {
    printf("Cannot allocate memory for ExpressionList");
    exit(1);
  }
  list->size = 0;
  list->capacity = 32;
  return list;
}

void exprlist_add(ExpressionList *list, Expression *value) {
  if (list->size >= list->capacity) {
    list->capacity *= 2;
    list->expressions =
        realloc(list->expressions, sizeof(Expression) * list->capacity);
  }
  list->expressions[list->size++] = value;
}

Expression *exprlist_get(ExpressionList *list, int index) {
  if (index >= list->size || index < 0) {
    printf("Index %d out of bounds for list of size %d\n", index, list->size);
    exit(1);
  }
  return list->expressions[index];
}

void exprlist_print(ExpressionList *list) {
  for (int i = 0; i < list->size; i++) {
    Expression *expr = exprlist_get(list, i);
    expr_print(expr);
  }
  printf("\n");
}

void exprlist_free(ExpressionList *list) { free(list->expressions); }

void expr_print(const Expression *expr) {
  printf("Expr[type: %s", expr->type);
  if (expr->left != NULL) {
    printf(", left: ");
    expr_print(expr->left);
  }
  if (expr->right != NULL) {
    printf(", right: ");
    expr_print(expr->right);
  }
  if (expr->operator!= NULL && expr->operator->lexeme != NULL) {
    printf(", operator: %s", expr->operator->lexeme);
  }
  if (expr->name != NULL) {
    printf(", name: %s", expr->name);
    if (strcmp(expr->name, "nil") == 0 && expr->value == NULL) {
      printf(", value: NULL");
    }
  }
  if (expr->value != NULL) {
    printf(", value: %s", value_string(expr->value));
  }
  printf("]");
}

Expression *newExpression(char *type) {
  Expression *e = malloc(sizeof(Expression));
  e->type = type;
  e->left = NULL;
  e->right = NULL;
  e->name = NULL;
  e->operator= NULL;
  e->value = NULL;
  e->block = NULL;
  return e;
}

Value *newString(char *string) {
  Value *value = newValue();
  value->type = STRINGTYPE;
  value->value.string = string;
  return value;
}

Value *newBoolean(bool boolean) {
  Value *value = newValue();
  value->type = BOOLEANTYPE;
  value->value.boolean = boolean;
  return value;
}

Value *newNumber(double number) {
  Value *value = newValue();
  value->type = NUMBERTYPE;
  value->value.number = number;
  return value;
}

Value *newValue(void) {
  Value *value = malloc(sizeof(Value));
  if (value == NULL) {
    printf("can't allocate memory for Value");
    exit(1);
  }
  return value;
}

char *d_to_s(double d) {
  char *str = (char *)malloc(50);
  if (str == NULL) {
    puts("cannot allocate memory for string");
    exit(1);
  }
  snprintf(str, sizeof(str), "%lf", d); //
  return str;
}

const char *value_string(Value *v) {
  switch (v->type) {
  case STRINGTYPE:
    return v->value.string;
  case BOOLEANTYPE:
    return v->value.boolean ? "true" : "false";
  case NUMBERTYPE:
    return d_to_s(v->value.number);
  case EXPR:
    return v->value.expr->type;
  }
}
