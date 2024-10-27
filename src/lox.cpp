#include "parser.hpp"
#include "scanner.hpp"
#include "tokens.hpp"
#include <cstdbool>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using std::string, std::vector, std::ifstream, std::cin, std::cout;

void print_tokens(vector<Token> *list);
int run_file(string file);
void run_prompt(void);
Result<vector<Token>> run(string source);

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

int run_file(string filename) {
  string content;
  ifstream file;
  file.open(filename);
  if (file.is_open()) {
    file >> content;
  } else {
    exit(1);
  }

  Result<vector<Token>> scan_result = run(content);

  return EXIT_SUCCESS;
}

void run_prompt(void) {
  string line;

  for (;;) {
    cout << ">";

    getline(cin, line);

    auto scan_result = run(line.substr(0, line.length()));
    // print_tokens(&scan_result.token_list);
    if (is_ok(scan_result)) {
      auto expression = (new Parser())->parse(get<vector<Token>>(scan_result));
      if (is_ok(expression)) {
        cout << Ok(expression)->as_string() << "\n";
      } else {
        cout << err_msg(expression) << "\n";
      }
    } else {
      cout << err_msg(scan_result) << "\n";
    }
  }
}

Result<vector<Token>> run(string source) {
  Scanner *scanner = new Scanner(source);
  return scanner->scan_tokens();
}

void print_tokens(vector<Token> *list) {
  for (vector<Token>::iterator token = list->begin(); token != list->end();
       ++token) {
    cout << token->as_string() << "(" << token->literal << "), ";
  }

  cout << "\n";
}
