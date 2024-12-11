#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "parser.h"

#define MAX_MAP_SIZE 1000

typedef struct VarMap VarMap;

VarMap *newVarMap(VarMap *enclosing);
void interpret(VarMap *environment, ExpressionList *statements);

typedef struct {
  char key[50]; // Array of strings for keys
  Value *value; // Integer values associated with keys
} MapEntry;

struct VarMap {
  VarMap *enclosing;
  MapEntry entries[MAX_MAP_SIZE]; // Array of key-value pairs
  int size;                       // Current size of the map
};

bool var_isdefined(VarMap *map, const char *key);
void var_add(VarMap *map, const char *key, Value *value);
Value *var_get(VarMap *map, const char *key);
bool var_set(VarMap *map, char *key, Value *value);
#endif
