#ifndef SCAN_H
#define SCAN_H

#include <ctype.h>
#include <stdbool.h>

#include "common.h"
#include "token.h"
#include "external/vec.h"

vec_token_t scan(const char* src) {
    vec_token_t tokens;
    vec_init(&tokens);
    int line_num = 0;
    int column_num = 0;
    int i = 0;
    for (;;) {//while (i + 3 <= buffer_size) {
        token_t token;
        token.token_enum = TOKEN_NULL;
        size_t len = 0;
        
        char c = src[i];
        
        // Skip whitespace
        while (isspace(c)) {
            if (c == '\n') {
                line_num++;
                column_num = 0;
            } else {
                column_num++;
            }
            c = src[++i];
        }
        // TODO: Skip comments
        
        // End of file
        if (c == EOF || c == NULL)
            break;
        
        // Label
        if (isalpha(c) || c == '_') {
            token.token_enum = TOKEN_LABEL;
            do {
                len++;
                c = src[i + len];
            }
            while (isalnum(c) || c == '_');
        }
        else if (c == '\'') {
            token.token_enum = TOKEN_CHAR;
            bool backslash = false;
            do {
                backslash = (!backslash && c == '\\');
                len++;
                c = src[i + len];
                if (c == NULL || c == EOF || c == '\n') break;
            } while(c != '\'' || backslash);
            len++;
        }
        else if (c == '\"') {
            token.token_enum = TOKEN_STRING;
            bool backslash = false;
            do {
                backslash = (!backslash && c == '\\');
                len++;
                c = src[i + len];
                if (c == NULL || c == EOF) break;
            } while(c != '\"' || backslash);
            len++;
        }
        else {
            // Int, Float
            if (isdigit(c) || (c == '.')) {
                len = 0;
                while (isdigit(c)) {
                    len++;
                    c = src[i + len];
                }

                if (c == '.') {
                    do {
                        len++;
                        c = src[i + len];
                    } while(isdigit(c) || c == '.' || tolower(c) == "f" || tolower(c) == "e");
                    if (len != 1) {
                        token.token_enum = TOKEN_FLOAT;
                    } else {
                        len = 0;
                        c = src[i];
                    }
                }
                else
                   token. token_enum = TOKEN_INT;
            }
            // Char:
            // Operator:
            if (token.token_enum == TOKEN_NULL && (ispunct(c))) {
                token.token_enum = TOKEN_OPERATOR;
                char c2 = src[i+1];
                // +=  -=  *=  /=  %=  ^=  !=  <=  >=  ==
                if (c2 == '=' && (c == '+' || c == '-' || c == '*' || c == '/' || c == '%' || c == '^' || c == '!' || c == '<' || c == '>' || c == '='))
                    len = 2;
                // ++  --  &&  ||
                else if (c == c2 && (c == '+' || c == '-' || c == '|' || c == '&'))
                    len = 2;
                // ->
                else if (c == '-' && c2 == '>')
                    len = 2;
                // >>  <<  >>=  <<=
                else if (c == c2 && (c == '<' || c == '>'))
                    len = (src[i + 2] == '=')? 3 : 2;
                else
                    len = 1;
            }
        }
        if (token.token_enum == TOKEN_NULL) {
            printf("Unexpected character '%c'\n", src[i]);
            len = 1;
        }

        token.line_num = line_num;
        token.column_num = column_num;
        token.index = i;
        token.len = len;
        vec_push(&tokens, token);

        i += len;
        (column_num) += len;
    }
    return tokens;
}

#endif // SCAN_H
