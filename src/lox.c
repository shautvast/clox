#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "tokens.h"

int run_file(char* file);
void run_prompt(void);
void run(char* source);
void scan_tokens(char* source);
void scan_token(char* source, int line, int start, int* current_pos, TokenList* token_list);
void error(int line, char* nessage);
void report(int line, char* where, char* message);
char* substring(char* string, int position, int length);

static bool had_error = false;

int main(int argc, char* argv[]){
    if (argc>2){
        puts("Usage: lox [script]");
        return EXIT_FAILURE;
    } else if (argc == 2){
        return run_file(argv[1]);
    } else {
        run_prompt();
    }
    return EXIT_SUCCESS;
}


int run_file(char* filename){
    FILE* file = fopen(filename, "r");
    if (file == NULL){
        printf("unable to open file '%s'\n", filename);
        return EXIT_FAILURE;
    }
    char line[255];
    
    char* content = malloc(1);
    if (content == NULL){
        puts("Out of memory");
        return EXIT_FAILURE;
    }
    content[0] = '\0';
    
        
    while (fgets(line, sizeof(line), file)){
        content = realloc(content, strlen(content) + strlen(line) + 1 );
        if (content == NULL){
            return EXIT_FAILURE;
        }
        strcat(content, line);
    }        
    
    fclose(file);
   
    run(content);

    // FREE UP
    free(content);
    
    if (had_error){
        return 65;
    }

    return EXIT_SUCCESS;
}

void run_prompt(void){
    char line[255];

    for (;;){
        printf(">");
        char* r = fgets(line, 255, stdin);
        
        if (r == NULL){
            break;
        } 

        run(line);
        had_error = false;
    }
}

void run(char* source){
    printf("%s", source);
    scan_tokens(source);
}

void scan_tokens(char* source){
    int current = 0;
    int start = 0;
    int line = 1;

    TokenList token_list;
    tokenlist_init(&token_list);

    while (current < (int)strlen(source)) {
        start = current;
        scan_token(source, line, start, &current, &token_list);
    }
    tokenlist_print(&token_list);
}

void add_token(char* source, int line, TokenList* token_list, enum TokenType type, int start, int current_pos){
    Token token;
    token.type = type;
    token.lexeme = substring(source, start, current_pos);
    token.literal = NULL;
    token.line = line;
       
    tokenlist_add(token_list, token);
}

void scan_token(char* source, int line, int start, int* current_pos, TokenList* token_list){
    char c = source[*current_pos];
    (*current_pos) += 1;
    switch (c){
        case '(': add_token(source, line, token_list, LEFT_PAREN, start, *current_pos); break;
        case ')': add_token(source, line, token_list, RIGHT_PAREN, start, *current_pos); break;
        case '{': add_token(source, line, token_list, LEFT_BRACE, start, *current_pos); break;
        case '}': add_token(source, line, token_list, RIGHT_BRACE, start, *current_pos); break;
        case ',': add_token(source, line, token_list, COMMA, start, *current_pos); break;
        case '.': add_token(source, line, token_list, DOT, start, *current_pos); break;
        case '+': add_token(source, line, token_list, PLUS, start, *current_pos); break;
        case ';': add_token(source, line, token_list, SEMICOLON, start, *current_pos); break;
        case '/': add_token(source, line, token_list, SLASH, start, *current_pos); break;
    }
}

void error(int line, char* message){
    report(line, "", message);
}

void report(int line, char* where, char* message){
    printf("[Line %i] Error %s : %s\n", line, where, message);
    had_error = true;
}

char* substring(char* string, int position, int length){
    char* ptr = malloc(length+1);
    if (ptr == NULL) {
        printf("out of memory");
        exit(EXIT_FAILURE);
    }

    int c;
    for (c=0; c< length; c++){
        *(ptr+c) = *(string+position-1);
        string += sizeof(char);
    }
    *(ptr+c) = '\0';

    return ptr;
}
    
