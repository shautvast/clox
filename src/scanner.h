#ifndef SCANNER_H
#define SCANNER_H

#include "tokens.h"
#include <stdbool.h>
#include <string.h>

typedef struct {
  bool had_error;
  TokenList token_list;
} ScanResult;

ScanResult scan_tokens(char *source);

typedef struct {
  const char *key;
  const enum TokenType value;
} Item;

static const Item keywords[] = {
    {"and", AND},   {"class", CLASS}, {"else", ELSE},     {"false", FALSE},
    {"for", FOR},   {"fun", FUN},     {"if", IF},         {"nil", NIL},
    {"or", OR},     {"print", PRINT}, {"return", RETURN}, {"super", SUPER},
    {"this", THIS}, {"true", TRUE},   {"var", VAR},       {"while", WHILE}};

#endif
