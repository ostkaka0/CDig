#ifndef SCAN_H
#define SCAN_H

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>

#include "common.h"
#include "token.h"
#include "external/vec.h"

typedef enum {
    TOKEN__NULL = 0,
    TOKEN_IDENTIFIER,
    TOKEN_OPERATOR,
    TOKEN_INT,
    TOKEN_FLOAT,
    TOKEN_CHAR,
    TOKEN_STRING,
    TOKEN__END
} token_type_t;

typedef struct {
    token_type_t type;
    int line_num;
    int column_num;
    char* str;
    int len;
} token_t;

typedef vec_t(token_t) vec_token_t;

static bool token_equals(const token_t token, const char* str) {
    for (int i = 0; i < token.len; i++)
        if (token.str[i] != str[i]) return false;
    return (str[token.len] == '\0');
}

static vec_token_t tokenize(const char* src) {
    vec_token_t tokens;
    vec_init(&tokens);
    int line_num = 0;
    int column_num = 0;
    const char* c = src;
    for (;;) {//while (i + 3 <= buffer_size) {
        token_t token;
        token.type = TOKEN__NULL;
        size_t len = 0;
        
        // Skip whitespace
        while (isspace(c[0])) {
            if (c[0] == '\n') {
                line_num++;
                column_num = 0;
            } else if (c[0] != '\r') {
                column_num++;
            }
            c++;
        }

        // Skip line comment
        if (c[0] == '/' && c[1] == '/') {
            c += 2;
            while (c[0] != '\n' && c[0] != '\0' && c[0] != EOF) c++;
            continue;
        }

        // Skip block comment
        if (c[0] == '/' && c[1] == '*') {
            c += 2;
            while(c[0] != '\n' && c[0] != '\0' && c[0] != EOF) {
                if (c[0] == '*' && c[1] == '/') {
                    c += 2;
                    break;
                }
                c++;
            }
            continue;
        }
        
        // End of file
        if (c[0] == EOF || c[0] == NULL)
            break;
        
        // Identifier
        if (isalpha(c[0]) || c[0] == '_') {
            token.type = TOKEN_IDENTIFIER;
            do len++; while (isalnum(c[len]) || c[len] == '_');
        }
        else if (c[0] == '\'') {
            token.type = TOKEN_CHAR;
            bool backslash = false;
            do {
                backslash = (!backslash && c[len] == '\\');
                len++;
                if (c[len] == NULL || c[len] == EOF || c[len] == '\n') break;
            } while(c[len] != '\'' || backslash);
            len++;
        }
        else if (c[0] == '\"') {
            token.type = TOKEN_STRING;
            bool backslash = false;
            do {
                backslash = (!backslash && c[len] == '\\');
                len++;
                if (c[len] == NULL || c[len] == EOF) break;
            } while(c[len] != '\"' || backslash);
            len++;
        }
        else {
            // Int, Float
            if (isdigit(c[0]) || (c[0] == '.')) {
                len = 0;
                while (isdigit(c[len])) len++;

                if (c[len] == '.') {
                    do len++; while(isdigit(c[len]) || c[len] == '.' || tolower(c[len]) == "f" || tolower(c[len]) == "e");
                    if (len != 1) token.type = TOKEN_FLOAT;
                    else len = 0;
                } else
                   token.type = TOKEN_INT;
            }
            // Char:
            // Operator:
            if (token.type == TOKEN__NULL && (ispunct(c[0]))) {
                token.type = TOKEN_OPERATOR;
                // +=  -=  *=  /=  %=  ^=  !=  <=  >=  ==
                if (c[1] == '=' && (c[0] == '+' || c[0] == '-' || c[0] == '*' || c[0] == '/' || c[0] == '%' || c[0] == '^' || c[0] == '!' || c[0] == '<' || c[0] == '>' || c[0] == '='))
                    len = 2;
                // ++  --  &&  ||
                else if (c[0] == c[1] && (c[0] == '+' || c[0] == '-' || c[0] == '|' || c[0] == '&'))
                    len = 2;
                // ->
                else if (c[0] == '-' && c[1] == '>')
                    len = 2;
                // >>  <<  >>=  <<=
                else if (c[0] == c[1] && (c[0] == '<' || c[0] == '>'))
                    len = (c[2] == '=')? 3 : 2;
                // Single character operators:
                else
                    len = 1;
            }
        }
        if (token.type == TOKEN__NULL) {
            printf("Unexpected character '%c'\n", c[0]);
            len = 1;
        }

        token.line_num = line_num;
        token.column_num = column_num;
        token.str = c;
        token.len = len;
        vec_push(&tokens, token);

        c += len;
        column_num += len;
    }
    return tokens;
}

#endif // SCAN_H
