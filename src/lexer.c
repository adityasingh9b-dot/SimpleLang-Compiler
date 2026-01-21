#include "../include/lexer.h"
#include <ctype.h>
#include <string.h>

void getNextToken(FILE *file, Token *token) {
    int c;
    while ((c = fgetc(file)) != EOF) {
        
       
        if (isspace(c)) continue;

        if (c == '/')
        {
            if ((c = fgetc(file)) == '/')
            {
              while ((c = fgetc(file)) != '\n' && c != EOF);
              continue;
            }
        
        else
        {
          ungetc(c, file);
          c = '/';
        }
        
        }

        if (isalpha(c)) {
            int len = 0;
            token->text[len++] = c;
            while (isalnum(c = fgetc(file))) {
                if (len < MAX_TOKEN_LEN - 1) token->text[len++] = c;
            }
            ungetc(c, file);
            token->text[len] = '\0';

            if (strcmp(token->text, "int") == 0) token->type = TOKEN_INT;
            else if (strcmp(token->text, "if") == 0) token->type = TOKEN_IF;
            else token->type = TOKEN_IDENTIFIER;
            return;
        }

        if (isdigit(c)) {
            int len = 0;
            token->text[len++] = c;
            while (isdigit(c = fgetc(file))) {
                if (len < MAX_TOKEN_LEN - 1) token->text[len++] = c;
            }
            ungetc(c, file);
            token->text[len] = '\0';
            token->type = TOKEN_NUMBER;
            return;
        }

        switch (c) {
            case '+': token->type = TOKEN_PLUS; strcpy(token->text, "+"); return;
            case '-': token->type = TOKEN_MINUS; strcpy(token->text, "-"); return;
            case ';': token->type = TOKEN_SEMICOLON; strcpy(token->text, ";"); return;
            case '{': token->type = TOKEN_LBRACE; strcpy(token->text, "{"); return;
            case '}': token->type = TOKEN_RBRACE; strcpy(token->text, "}"); return;
            case '=':
                c = fgetc(file);
                if (c == '=') {
                    token->type = TOKEN_EQUAL;
                    strcpy(token->text, "==");
                } else {
                    ungetc(c, file);
                    token->type = TOKEN_ASSIGN;
                    strcpy(token->text, "=");
                }
                return;
        }
    }
    token->type = TOKEN_EOF;
    token->text[0] = '\0';
}
