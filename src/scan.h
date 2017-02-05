#ifndef SCAN_H
#define SCAN_H

#include <ctype.h>

#include "common.h"
#include "token.h"
#include "external/vec.h"

// return value - buffer-index
static int scan(vec_token_t* rokens, int* line_num, int* column_num, const char* buffer, const int buffer_size) {
    int i = 0;
    while (i < buffer_size) {
        token_t token;
        token_enum_t token_enum;
        size_t len = 1;
        
        char c = buffer[i];
        
        // Skip whitespace
        while (isspace(c)) {
            if (c == '\n') {
                (*line_number)++;
                (*column_num) = 0;
            } else {
                (*column_num)++;
            }
            c = buffer[++i];
            if (i >= buffer_size) return i;
        }
        
        // End of file
        if (c == EOF || c == NULL)
            break;

        printf("%c ", c);
        
        // Label
        if (isalpha(c) || c == '_') {
            len = 0;
            do {
                len++;
                if (i + len >= buffer_size) return i;
                c = buffer[i + len];
            }
            while (isalnum(c) || c == '_');
            token_enum = TOKEN_LABEL;
        }
        // Int, Float
        else if (isdigit(c) || c == '.') {
            len = 0;
            
            while (isdigit(c)) {
                len++;
                c = buffer[i+len];
                if (i + len >= 2048) // TODO: Compile error
                    runtime_error(line_number, "Digit is longer than max len.");
            }
            
            if (c == '.') {
                do {
                    len++;
                    c = buffer[i+len];
                    if (i + len >= 2048) // TODO: Compile error
                        runtime_error(line_number, "Digit is longer than max len.");
                } while(isdigit(c) || c == '.' || tolower(c) == "f" || tolower(c) == "e");
                token_enum = TOKEN_FLOAT;
            }
            else
                token_enum = TOKEN_INT;
        }
        // Symbol:
        else if (ispunct(c)) {
            switch(c) {
                case '+':
                    if (buffer[i+1] == '+') {
                        token = create_token_symbol(SYMBOL_INCREASE);
                        len = 2;
                        break;
                    }
                    token = create_token_symbol(SYMBOL_ADD);
                    break;
                case '-':
                    if (buffer[i+1] == '+') {
                        token = create_token_symbol(SYMBOL_DECREASE);
                        len = 2;
                        break;
                    }
                    token = create_token_symbol(SYMBOL_SUBTRACT);
                    break;
                case '(':
                    token = create_token_symbol(SYMBOL_PARANTHESIS_BEGIN);
                    break;
                case ')':
                    token = create_token_symbol(SYMBOL_PARANTHESIS_END);
                    break;
                case '*':
                    token = create_token_symbol(SYMBOL_MULTIPLY);
                    break;
                case '/': // TODO: Comments
                    token = create_token_symbol(SYMBOL_DIVIDE);
                    break;
                case '%': // TODO: Comments
                    token = create_token_symbol(SYMBOL_MODULO);
                    break;
                case ';':
                    token = create_token_symbol(SYMBOL_SEMICOLON);
                    break;
                case '=':
                    token = create_token_symbol(SYMBOL_ASSIGN);
                    break;
                
                default:
                    break;
            }
        }
        
        /*if (strcmp(buffer[i], "++") == 0)
            token = { SYMBOL_INCREASE, TOKEN_OPERATOR }; len = 2;
        else if (strcmp(data[i], "--") == 0)
            token = { SYMBOL_DECREASE, TOKEN_OPERATOR }; len = 2;
        else if (strcmp(data[i], "(") == 0)
            token = { SYMBOL_PARANTHESIS_BEGIN, TOKEN_OPERATOR };
        else if (strcmp(data[i], ")") == 0)
            token = { SYMBOL_PARANTHESIS_END, TOKEN_OPERATOR };
        else if (strcmp(data[i], "+") == 0)
            token = { SYMBOL_ADD, TOKEN_OPERATOR };
        else if (strcmp(data[i], "-") == 0)
            token = { SYMBOL_SUBTRACT, TOKEN_OPERATOR };
        else if (strcmp(data[i], "*") == 0)
            token = { SYMBOL_MULTIPLY, TOKEN_OPERATOR };
        else if (strcmp(data[i], "/") == 0)
            token = { SYMBOL_DIVIDE, TOKEN_OPERATOR };
        else if (data[i], '"' {
            size_t i2 = i;
            bool backslash = false;
            while(data[i2+1] != '"' || backslash) {
                i2++;
                if (backslash)
                    backslash = false;
                else if(data[i2] == '\\')
                    backslash = true;
            }
            len = i2 - i;
        }*/

        token.line_num = (*line_num);
        token.column_num = (*column_num);
        token.len = len;
        vec_push(tokens, token);

        i += len;
        (*column_num) += len;
    }
    
    printf("Lines: %i\n", line_number);
    

    return i;
}

#endif // SCAN_H
