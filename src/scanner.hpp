#ifndef SCANNER_H
#define SCANNER_H

#include "tokens.hpp"
#include <cstdbool>
#include <list>
#include <string>

typedef struct {
  bool had_error;
  std::list<Token> token_list;
} ScanResult;

ScanResult scan_tokens(std::string source);

typedef struct {
  const std::string key;
  const TokenType value;
} Item;

static const Item keywords[] = {
    {"and", AND},   {"class", CLASS}, {"else", ELSE},     {"false", FALSE},
    {"for", FOR},   {"fun", FUN},     {"if", IF},         {"nil", NIL},
    {"or", OR},     {"print", PRINT}, {"return", RETURN}, {"super", SUPER},
    {"this", THIS}, {"true", TRUE},   {"var", VAR},       {"while", WHILE}};

inline static const TokenType *get_keyword_token(std::string key) {
  int low = 0;
  int high = sizeof(keywords) / sizeof(Item);

  while (low < high) {
    int mid = (low + high) / 2;

    int c = keywords[mid].key.compare(key);
    if (c == 0) {
      return &keywords[mid].value;
    }
    if (c < 0) {
      low = mid + 1;
    } else {
      high = mid;
    }
  }
  return NULL;
}
#endif
