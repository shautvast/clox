#include "interpreter.h"
#include <string.h>

Value *accept(Expression *expr);
void checkNumeric(Value *left, Value *right);
Value *isEqual(Value *left, Value *right);
bool streq(char *left, char *right);

Value *visitBinary(Expression *expr);
Value *visitUnary(Expression *unary);
Value *visitLiteral(Expression *literal);
Value *visitGroup(Expression *literal);
Value *visitVariable(Expression *var);
Value *visitVariableStmt(Expression *varStmt);
Value *visitAssignStmt(Expression *varStmt);

Value *visitPrintStmt(Expression *printStatement);
Value *visitBlock(Expression *block);

static VarMap *current;

Value *accept(Expression *expr) {
  // printf("accept %s\n", expr->type);
  char *type = expr->type;
  if (streq(type, "BinaryExpr")) {
    return visitBinary(expr);
  }
  if (streq(type, "UnaryExpr")) {
    return visitUnary(expr);
  }
  if (streq(type, "Literal")) {
    return visitLiteral(expr);
  }
  if (streq(type, "Group")) {
    return visitGroup(expr);
  }
  if (streq(type, "PrintStmt")) {
    return visitPrintStmt(expr);
  }
  if (streq(type, "VariableStmt")) {
    return visitVariableStmt(expr);
  }
  if (streq(type, "AssignStmt")) {
    return visitAssignStmt(expr);
  }
  if (streq(type, "Variable")) {
    return visitVariable(expr);
  }
  if (streq(type, "ExprStmt")) {
    return accept(expr->left);
  }
  if (streq(type, "Block")) {
    return visitBlock(expr);
  }

  return NULL;
}

void execute(Expression *statement) { accept(statement); }

void interpret(VarMap *environment, ExpressionList *statements) {
  current = environment;

  for (int i = 0; i < statements->size; i++) {
    execute(exprlist_get(statements, i));
  }
}

Value *visitVariable(Expression *var) { return var_get(current, var->name); }

Value *visitVariableStmt(Expression *var) {
  Value *value = accept(var->left);
  if (var_isdefined(current, var->name)) {
    printf("%s is already defined\n", var->name);
    return NULL;
  }
  var_add(current, var->name, value);
  return NULL;
}

Value *visitBlock(Expression *blockStmt) {
  VarMap *previous = current;
  current = newVarMap(previous);
  for (int i = 0; i < blockStmt->block->size; i++) {
    Expression *e = exprlist_get(blockStmt->block, i);
    execute(e);
  }
  current = previous;
  return NULL;
}

Value *visitGroup(Expression *group) { return accept(group->left); }

Value *visitPrintStmt(Expression *printStatement) {
  Value *value = accept(printStatement->left);
  if (value == NULL) {
    return NULL;
  }
  printf("%s\n", value_string(value));
  return NULL;
}

Value *visitAssignStmt(Expression *var) {
  Value *value = accept(var->left);
  bool result = var_set(current, var->name, value);
  if (!result) {
    printf("%s is not defined", var->name);
  }
  return NULL;
}

Value *visitUnary(Expression *unary) {
  Value *right = accept(unary->right);

  switch (unary->operator->type) {
  case MINUS:
    return newNumber(-right->value.number);
  case BANG:
    return newBoolean(!right->value.boolean);
  default:
    return NULL;
  };
}

Value *visitLiteral(Expression *literal) { return literal->value; }

Value *visitBinary(Expression *expr) {
  Value *left = accept(expr->left);
  Value *right = accept(expr->right);

  switch (expr->operator->type) {
  case MINUS:
    checkNumeric(left, right);
    return newNumber(left->value.number - right->value.number);
  case PLUS:
    checkNumeric(left, right);
    return newNumber(left->value.number + right->value.number);
  case SLASH:
    checkNumeric(left, right);
    return newNumber(left->value.number / right->value.number);
  case STAR:
    checkNumeric(left, right);
    return newNumber(left->value.number * right->value.number);
  case GREATER:
    checkNumeric(left, right);
    return newBoolean(left->value.number > right->value.number);
  case GREATER_EQUAL:
    checkNumeric(left, right);
    return newBoolean(left->value.number >= right->value.number);
  case LESS:
    checkNumeric(left, right);
    return newBoolean(left->value.number < right->value.number);
  case LESS_EQUAL:
    checkNumeric(left, right);
    return newBoolean(left->value.number <= right->value.number);
  case BANG_EQUAL:
    return isEqual(left, right);
  case EQUAL_EQUAL:
    return isEqual(left, right);
  default:
    return NULL;
  }
}

void checkNumeric(Value *left, Value *right) {
  if (left->type != NUMBERTYPE || right->type != NUMBERTYPE) {
    printf("operands should be numeric");
    exit(-1);
  }
}

VarMap *newVarMap(VarMap *enclosing) {
  VarMap *map = malloc(sizeof(VarMap));
  if (map == NULL) {
    printf("Can not allocate memory for VarMap");
    exit(1);
  }
  map->size = 0;
  map->enclosing = enclosing;
  return map;
}

bool var_isdefined(VarMap *map, const char *key) {
  for (int i = 0; i < map->size; i++) {
    if (strcmp(map->entries[i].key, key) == 0) {
      return true;
    }
  }
  if (map->enclosing != NULL) {
    return var_isdefined(map->enclosing, key);
  }

  return false;
}

void var_add(VarMap *map, const char *key, Value *value) {
  if (map->size == MAX_MAP_SIZE) {
    printf("Map is full!\n");
    return;
  }
  strcpy(map->entries[map->size].key, key);
  map->entries[map->size].value = value;

  map->size += 1;
}

bool var_set(VarMap *map, char *key, Value *value) {
  for (int i = 0; i < map->size; i++) {
    if (strcmp(map->entries[i].key, key) == 0) {
      map->entries[i].value = value; // Return the value
      return true;
    }
  }
  return false;
}

Value *var_get(VarMap *map, const char *key) {
  for (int i = 0; i < map->size; i++) {

    if (strcmp(map->entries[i].key, key) == 0) {
      return map->entries[i].value; // Return the value
    }
  }
  if (map->enclosing != NULL) {
    return var_get(map->enclosing, key);
  }

  printf("%s is not defined\n", key);
  return NULL; // Key not found
}

Value *isEqual(Value *left, Value *right) {
  if (left->type != right->type) {
    return newBoolean(false);
  }
  switch (left->type) {
  case STRINGTYPE:
    return newBoolean(strcmp(left->value.string, right->value.string) == 0);
  case NUMBERTYPE:
    return newBoolean(left->value.number == right->value.number);
  case BOOLEANTYPE:
    return newBoolean(left->value.boolean == right->value.boolean);
  case EXPR: // MUST NOT HAPPEN :(=)
    return NULL;
  }
}

bool streq(char *left, char *right) { return strcmp(left, right) == 0; }
