#include "interpreter.h"
#include "parser.h"
#include "scanner.h"
#include "utils.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int run_file(char *file);
void run_prompt(void);
void run(char *source);
static VarMap *environment;

int main(int argc, char *argv[]) {
  environment = newVarMap(NULL);

  setvbuf(stdout, NULL, _IONBF, 0);
  if (argc > 2) {
    puts("Usage: lox [script]");
    return EXIT_FAILURE;
  } else if (argc == 2) {
    return run_file(argv[1]);
  } else {
    run_prompt();
  }
  return EXIT_SUCCESS;
}

int run_file(char *filename) {

  FILE *file = fopen(filename, "r");
  if (file == NULL) {
    printf("unable to open file '%s'\n", filename);
    return EXIT_FAILURE;
  }
  char line[255];

  char *content = malloc(1);
  if (content == NULL) {
    puts("Out of memory");
    return EXIT_FAILURE;
  }
  content[0] = '\0';

  while (fgets(line, sizeof(line), file)) {
    content = realloc(content, strlen(content) + strlen(line) + 1);
    if (content == NULL) {
      return EXIT_FAILURE;
    }
    strcat(content, line);
  }

  fclose(file);

  run(content);

  // FREE UP
  free(content);

  // if (scan_result.had_error) {
  //   return 65;
  // }

  return EXIT_SUCCESS;
}

void run_prompt(void) {
  char line[255];

  for (;;) {
    printf(">");
    char *r = fgets(line, 255, stdin);

    if (r == NULL) {
      break;
    }

    int len = (int)strlen(line);
    run(substring(line, 1, len - 1));
  }
}

void run(char *source) {
  ScanResult scan_result = scan_tokens(source);
  // tokenlist_print(&scan_result.token_list);
  ExpressionList *list = parse(&scan_result.token_list);
  // exprlist_print(list);
  interpret(environment, list);
}
