#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"

#define MAX_SYMBOLS 100

typedef struct {
    char name[MAX_TOKEN_LEN];
    int address;
} Symbol;

// Parser functions
void parse_program(FILE *file);
void add_symbol(char *name);
int get_address(char *name);

#endif
