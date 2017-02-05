#ifndef SCAN_H
#define SCAN_H

#include <ctype.h>

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
        size_t len = 1;
        
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
            len = 0;
            do {
                len++;
                c = src[i + len];
            }
            while (isalnum(c) || c == '_');
            token.token_enum = TOKEN_LABEL;
        }
        // Int, Float
        else if (isdigit(c) || c == '.') {
            len++;
            while (isdigit(c) || tolower(c) == "f" || tolower(c) == "e") {
                len++;
                c = src[i + len];
            }

            if (c == '.') {
                do {
                    len++;
                    c = src[i + len];
                } while(isdigit(c) || c == '.' || tolower(c) == "f" || tolower(c) == "e");
                token.token_enum = TOKEN_FLOAT;
            }
            else
               token. token_enum = TOKEN_INT;
        }
        // Operator:
        else if (ispunct(c)) {
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
