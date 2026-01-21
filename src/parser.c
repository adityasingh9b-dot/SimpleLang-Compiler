#include "../include/parser.h"
#include <string.h>
#include <stdlib.h>

Symbol symbol_table[MAX_SYMBOLS];
int symbol_count = 0;
int next_addr = 128; 

void add_symbol(char *name) {
    for (int i = 0; i < symbol_count; i++) {
        if (strcmp(symbol_table[i].name, name) == 0) return;
    }
    strcpy(symbol_table[symbol_count].name, name);
    symbol_table[symbol_count].address = next_addr++;
    symbol_count++;
}

int get_address(char *name) {
    for (int i = 0; i < symbol_count; i++) {
        if (strcmp(symbol_table[i].name, name) == 0) return symbol_table[i].address;
    }
    return -1;
}

void parse_program(FILE *file) {
    Token token;
    printf("; --- Generated Assembly Code ---\n");

    while (1) {
        getNextToken(file, &token);
        if (token.type == TOKEN_EOF) break;

        // 1. Handle Variable Declaration: int x;
        if (token.type == TOKEN_INT) {
            getNextToken(file, &token); 
            add_symbol(token.text);
            getNextToken(file, &token); // skip ';'
        }

        // 2. Handle Assignment/Arithmetic: a = ...
        else if (token.type == TOKEN_IDENTIFIER) {
            char target_var[MAX_TOKEN_LEN];
            strcpy(target_var, token.text);
            int target_addr = get_address(target_var);

            getNextToken(file, &token); // should be '='
            if (token.type == TOKEN_ASSIGN) {
                getNextToken(file, &token); // First value (Number or Identifier)
                
                if (token.type == TOKEN_NUMBER) {
                    printf("ldi A %s\n", token.text);
                } else if (token.type == TOKEN_IDENTIFIER) {
                    printf("lda %d\n", get_address(token.text));
                }

                // Check if there's a '+' or '-'
                getNextToken(file, &token);
                if (token.type == TOKEN_PLUS) {
                    printf("mov B A\n"); // Move first value to B
                    getNextToken(file, &token); // Get second value
                    
                    if (token.type == TOKEN_NUMBER) {
                        printf("ldi A %s\n", token.text);
                    } else if (token.type == TOKEN_IDENTIFIER) {
                        printf("lda %d\n", get_address(token.text));
                    }
                    printf("add\n"); // A = A + B
                    getNextToken(file, &token); // consume ';'
                }
                
                // Final Store
                printf("sta %d\n", target_addr);
            }
        }
        
        
        // 3. Handle IF Statement: if (c == 30) { ... }
        else if (token.type == TOKEN_IF) {
            getNextToken(file, &token); // skip '('
            getNextToken(file, &token); // variable 'c'
            int var_addr = get_address(token.text);
            
            getNextToken(file, &token); // '=='
            getNextToken(file, &token); // '30'
            char constant[MAX_TOKEN_LEN];
            strcpy(constant, token.text);

            // Assembly for Comparison
            printf("lda %d\n", var_addr);   // Load variable into A
            printf("ldi B %s\n", constant); // Load constant into B
            printf("cmp\n");               // Compare A and B
            printf("jne END_IF\n");         // If not equal, jump to label

            getNextToken(file, &token); // skip ')'
            getNextToken(file, &token); // skip '{'
            
            // Now the parser will continue and parse 'c = c + 1' normally
            // But we need to close the IF block
        }
        else if (token.type == TOKEN_RBRACE) {
            printf("END_IF: nop\n"); // Label where 'jne' will jump to
        }
        
        
    }
    printf("hlt\n"); // End of program
}
