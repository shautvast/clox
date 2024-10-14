// #include "tokens.hpp"
// #include <stdbool.h>

// static const TokenType eq[] = {BANG_EQUAL, EQUAL_EQUAL};
// static const TokenType comp[] = {GREATER, GREATER_EQUAL, LESS, LESS_EQUAL};
// static const TokenType unarytokens[] = {BANG, MINUS};

// // static int current = -1;
// // TokenList *tokens;

// class Expression;

// class Binary {
// public:
//   Expression *left;
//   Token *operator;
//   Expression *right;
// };

// typedef struct Grouping {
//   Expression *expr;
// } Grouping;

// typedef struct Unary {
//   Token *operator;
//   Expression *expr;
// } Unary;

// class Expression {
// public:
//   enum ExprType { Bin, Grp, Lit, Una } type;
//   union Expr {
//     Binary *binary;
//     Grouping *grouping;
//     Token *literal;
//     Unary *unary;
//   } expr;
// };

// void parse(TokenList *tokenlist) {
//   tokens = tokenlist;
//   current = 0;
// }

// static Token *peek(void) { return tokenlist_get(tokens, current); }

// static bool is_at_end(void) { return peek()->type == END_OF_FILE; }

// static bool check(TokenType type) {
//   if (is_at_end()) {
//     return false;
//   }
//   return peek()->type == type;
// }

// static bool match(const TokenType tokens[], int n) {
//   for (int i = 0; i < n; i++) {
//     if (check(tokens[i])) {
//     }
//   }
//   return false;
// }

// static Token previous(void) {
//   Token t = {NUMBER, "", 0, 0};
//   return t;
// }

// static Expression unary(void) {
//   if (match(unarytokens, 2)) {
//     Token operator= previous();
//     Expression right = unary();
//     Expression un;
//     un.type = Una;
//     Unary new_una = {&operator, & right };
//     un.expr.unary = &new_una;
//     return un;
//   }
//   return primary();
// }

// static Expression factor(void) {}

// static Expression term(void) { Expression expr = factor(); }

// static Expression comparison(void) {
//   Expression expr = term();
//   while (match(comp, 4)) {
//     Token operator= previous();
//     Expression right = term();

//     Binary new_bin = {&expr, &operator, & right };
//     expr.expr.binary = &new_bin;
//     expr.type = Bin;
//   }
//   return expr;
// }
// static Expression equality(void) {
//   Expression expr = comparison();

//   while (match(eq, 2)) {
//     Token operator= previous();
//     Expression right = comparison();
//     Binary new_bin = {&expr, &operator, & right };
//     expr.expr.binary = &new_bin;
//     expr.type = Bin;
//   }
//   return expr;
// }

// // static Expression expression(void) { return equality(); }
