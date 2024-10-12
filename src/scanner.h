#ifndef SCANNER_H
#define SCANNER_H

#include <stdbool.h>
#include "tokens.h"

typedef struct {
    bool had_error;
    TokenList token_list;
} ScanResult;

ScanResult scan_tokens(char* source);

#endif
