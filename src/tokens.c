#include <stdlib.h>
#include <stdio.h>
#include "tokens.h"

char* token_name(Token* token){
    char* type;
    switch (token->type){
        case LEFT_PAREN: type = "LEFT_PAREN"; break;
        case RIGHT_PAREN: type = "RIGHT_PAREN"; break;
        case LEFT_BRACE: type = "LEFT_BRACE"; break;
        case RIGHT_BRACE: type = "RIGHT_BRACE"; break;
        case COMMA: type = "COMMA"; break;
        case DOT: type = "DOT"; break;
        case MINUS: type = "MINUS"; break;
        case PLUS: type = "PLUS"; break;
        case SEMICOLON: type = "SEMICOLON"; break;
        case SLASH: type = "SLASH"; break;
        case STAR: type = "STAR"; break;
        case BANG: type = "BANG"; break;
        case BANG_EQUAL: type = "BANG_EQUAL"; break;
        case EQUAL: type = "EQUAL"; break;
        case EQUAL_EQUAL: type = "EQUAL_EQUAL"; break;
        case GREATER: type = "GREATER"; break;
        case GREATER_EQUAL: type = "GREATER_EQUAL"; break;
        case LESS: type = "LESS"; break;
        case LESS_EQUAL: type = "LESS_EQUAL"; break;
        case IDENTIFIER: type = "IDENTIFIER"; break;
        case STRING: type = "STRING"; break;
        case NUMBER: type = "NUMBER"; break;
        case AND: type = "AND"; break;
        case CLASS: type = "CLASS"; break;
        case ELSE: type = "ELSE"; break;
        case FALSE: type = "FALSE"; break;
        case FUN: type = "FUN"; break;
        case FOR: type = "FOR"; break;
        case IF: type = "IF"; break;
        case NIL: type = "NIL"; break;
        case OR: type = "OR"; break;
        case PRINT: type = "PRINT"; break;
        case RETURN: type = "RETURN"; break;
        case SUPER: type = "SUPER"; break;
        case THIS: type = "THIS"; break;
        case TRUE: type = "TRUE"; break;
        case VAR: type = "VAR"; break;
        case WHILE: type = "WHILE"; break;
        case END_OF_FILE: type = "EOF"; break;
    }
    return type;
}

void tokenlist_init(TokenList* list){
    list->tokens = malloc(sizeof(Token) * 32);
    list->size = 0;
    list->capacity = 32;
}

void tokenlist_add(TokenList* list, Token value){
    if (list->size >= list->capacity){
        list->capacity *= 2;
        list->tokens = realloc(list->tokens, sizeof(Token)* list->capacity);
    }
    list->tokens[list->size] = value;
    list->size +=1;
}

Token* tokenlist_get(TokenList* list, int index){
    if (index >= list->size || index < 0){
        printf("Index %d out of bounds for list of size %d\n", index, list->size);
        exit(1);
    }
    return &list->tokens[index];
}

void tokenlist_print(TokenList* tokenlist){
    for (int i=0; i< tokenlist->size; i++){
        printf("%s, ", token_name(tokenlist_get(tokenlist, i)));
        printf("\n");
    }
}

void tokenlist_free(TokenList* list){
    free(list->tokens);
}
