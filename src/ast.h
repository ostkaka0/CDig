#ifndef AST_H_INCLUDED
#define AST_H_INCLUDED

#include "external/vec.h"

// Declarations (global scope):

enum expr_type_t {
    EXPR_NULL,
    // Declarations
    EXPR_DECL_STRUCT,
    EXPR_DECL_STRUCT_IMPL,
    EXPR_DECL_FUNCTION,
    EXPR_DECL_FUNCTION_IMPL,
    EXPR_DECL_TYPEDEF,
    EXPR_DECL_VAR,
    EXPR_DECL_VAR_ASSIGN,
};

typedef struct {
    expr_type_t type;
    token_t* token_begin;
    token_t* token_end;
    union {
        struct { // Declaration data(except EXPR_DECL_TYPEDEF):
            token_t* token_name;
            vec_ptr_t children;
        } _decl;
        // Expression data:
        expr_t* _children[2];
    };
} expr_t;

typedef vec_t(expr_t) vec_expr_t;

static vec_expr_t parse(vec_token_t* tokens) {
    vec_expr_t expressions;
    vec_init(&expressions);
    u32 index = 0;

    token_t* token = tokens.data;

    while (token.type != TOKEN_NULL) {
        int line_number = token->line_number;
        expr_t decl = parse_decl(tokens, &token);
        expr.line_number = line_number;
        assert(token_equals(**token, ";"));

        vec_push(&expressions, expr);
    }

    return expressions;
}

static expr_t parse_decl(token_t** token) {
    expr_t expr;

    switch ((*token)->type) {
    case TOKEN_IDENTIFIER:
        if (token_equals(**token, "struct")) {
            expr.type = EXPR_DECL_STRUCT;
            expr.token_begin = *token++;
            expr._decl.token_name = **token;
            expr.token_end = *token++;
            assert((*token)->type == TOKEN_IDENTIFIER);
        } else if (token_equals(**token, "typedef")) {
            expr.type = EXPR_DECL_TYPEDEF;
            expr.token_begin = *token++;
            expr._children[0] = malloc(sizeof(expr_t));
            *expr._children[0] = parse_decl(token);
            expr.token_end = *token - 1;
            return expr;
        } else {
            // EXPR_DECL_FUNCTION or EXPR_DECL_VAR:
            expr_t expr_type = parse_type(token);
            assert((*token)->type == TOKEN_LABEL);
            assert((*token + 1)->type == TOKEN_SYMBOL);
            token_t token_name = *(*token++);
            token_t token_symbol = *(*token);
            if (token_equals(token_symbol, "("))
                expr = parse_decl_function(token, expr_type, token_name);
            else
                expr = parse_decl_var(token, expr_type, token_name);
        }
        break;
    default: assert(0);
    }

    if ((*token)->type = TOKEN_NULL) assert(0);
    if ((*token)->type == TOKEN_OPERATOR && token_equals(**token, ";")) return expr;

    switch(expr.type) {
    case EXPR_DECL_STRUCT: return parse_decl_struct_impl(token, expr);
    case EXPR_DECL_FUNCTION: return parse_decl_function_impl(token, expr);
    case EXPR_DECL_VAR: return parse_decl_var_assign(token, expr);
    case EXPR_DECL_TYPEDEF: assert(0);
    default: assert(0);
    }
}

static expr_t parse_decl_sruct_impl(token_t** token, expr_t expr) {
    assert((*token)->type == TOKEN_OPERATOR && token_equals(**token, "{"));
    *token++;
    assert(0);
}


static expr_t parse_decl_function(token_t** token, expr_t expr_type, token_t token_name) {
    assert(0);
}

static expr_t parse_decl_function_impl(token_t** token, expr_t expr) {
    assert((*token)->type == TOKEN_OPERATOR && token_equals(**token, "{"));
    *token++;
    assert(0);
}

static expr_t parse_decl_var_assign(token_t** token, expr_t expr) {
    assert((*token)->type == TOKEN_OPERATOR && token_equals(**token, "="));
    *token++;:
    assert(0);
}

static expr_t parse_type(token_t** token) {
    assert(0);
}

/*typedef struct {
    s64 array_int;
    double array_float;
    char* array_str;
} ast_data_t;*/

/*enum expr_type_t {
    EXPR_NULL,
    EXPR_STRUCT_EXPR,
    EXPR_FUNCTION_EXPR
};

typedef struct {
    token_t* token_name;
} expr_struct_decl_t;

typedef struct {
    token_t* token_name;
} expr_function_decl_t;

typedef struct {
    expr_type_t expr_type;
    union {
        expr_struct_decl_t struct_decl;
        expr_function_decl_t function_decl;
    };
} expr_t;*/

#endif // AST_H_INCLUDED
