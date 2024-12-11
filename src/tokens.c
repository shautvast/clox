#include "tokens.h"
#include <stdio.h>
#include <stdlib.h>

Token *newToken() {
  Token *token = malloc(sizeof(Token));
  if (token == NULL) {
    printf("can't allocate memory");
    exit(1);
  }
  return token;
}

void tokenlist_init(TokenList *list) {
  list->tokens = malloc(sizeof(Token) * 32);
  list->size = 0;
  list->capacity = 32;
}

void tokenlist_add(TokenList *list, Token *value) {
  if (list->size >= list->capacity) {
    list->capacity *= 2;
    list->tokens = realloc(list->tokens, sizeof(Token) * list->capacity);
  }
  list->tokens[list->size] = value;
  list->size += 1;
}

Token *tokenlist_get(TokenList *list, int index) {
  if (index >= list->size || index < 0) {
    printf("Index %d out of bounds for list of size %d\n", index, list->size);
    exit(1);
  }
  return list->tokens[index];
}

Token *tokenlist_last(TokenList *list) { return list->tokens[list->size - 1]; }

void tokenlist_print(TokenList *tokenlist) {
  for (int i = 0; i < tokenlist->size; i++) {
    Token *token = tokenlist_get(tokenlist, i);
    if (token->literal != NULL) {
      printf("%s(x:%s,l:%s), ", token_name(token->type), token->lexeme,
             (char *)token->literal);
    } else {
      printf("%s(l:%s)", token_name(token->type), token->lexeme);
    }
  }
  printf("\n");
}

void tokenlist_free(TokenList *list) { free(list->tokens); }
