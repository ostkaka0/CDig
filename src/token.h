#ifndef TOKEN_H
#define TOKEN_H

#include "common.h"
#include "external/vec.h"

typedef enum {
    TOKEN_LABEL,
    TOKEN_OPERATOR,
    TOKEN_INT,
    TOKEN_FLOAT,
    TOKEN_CHAR,
    TOKEN_STRING
} token_enum_t;

typedef struct {
    token_enum_t token_enum;
    int line_num;
    int column_num;
    int index;
    int len;
} token_t;
typedef vec_t(token_t) vec_token_t;

#endif // TOKEN_H
