#include "parser.hpp"
#include "scanner.hpp"
#include "tokens.hpp"
#include <cstdbool>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

void print_tokens(std::vector<Token> *list);
int run_file(std::string file);
void run_prompt(void);
ScanResult run(std::string source);

int main(int argc, char *argv[]) {
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

int run_file(std::string filename) {
  std::string content;
  std::ifstream file;
  file.open(filename);
  if (file.is_open()) {
    file >> content;
  } else {
    exit(1);
  }

  ScanResult scan_result = run(content);

  return EXIT_SUCCESS;
}

void run_prompt(void) {
  std::string line;

  for (;;) {
    std::cout << ">";

    std::getline(std::cin, line);

    ScanResult scan_result = run(line.substr(0, line.length()));
    // print_tokens(&scan_result.token_list);
    if (!scan_result.had_error) {
      Expression *e = (new Parser())->parse(scan_result.token_list);
      std::cout << e->to_string();
      std::cout << "\n";
    }
  }
}

ScanResult run(std::string source) {
  Scanner *scanner = new Scanner(source);
  return scanner->scan_tokens();
}

void print_tokens(std::vector<Token> *list) {
  for (std::vector<Token>::iterator token = list->begin(); token != list->end();
       ++token) {
    std::cout << token->to_string() << "(" << token->literal << "), ";
  }

  std::cout << "\n";
}
