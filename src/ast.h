#ifndef AST_H_INCLUDED
#define AST_H_INCLUDED

#include "external/vec.h"

#define ast_is_abstract(ast) (ast.type > AST_DECL__BEGIN && ast.type < AST_DECL__END)
#define ast_is_decl(ast)     (ast.type > AST_DECL__BEGIN && ast.type < AST_DECL__END)
#define ast_is_expr(ast)     (ast.type > AST_EXPR__BEGIN && ast.type < AST_EXPR__END)
#define ast_is_valid(ast)    (ast_is_abstract(ast) || ast_is_decl(ast) || ast_is_expr(ast) || ast.type == AST_TOKENS)

enum ast_type_t {
    AST__NULL = 0,
    AST_TOKENS,

    AST_ABSTRACT__BEGIN,
    AST_DATATYPE,
    AST_ARGUMENT_LIST,
    AST_ABSTRACT__END,

    AST_DECL__BEGIN,
    AST_DECL_STRUCT,
    AST_DECL_FUNCTION,
    AST_DECL_TYPEDEF,
    AST_DECL_VAR,
    AST_DECL__END,

    AST_EXPR__BEGIN,
    AST_EXPR_TOKENS, // unparsed expression
    AST_EXPR_SCOPE,
    // TODO: AST_EXPR_DEFER
    AST_EXPR__END,

    AST__END
};

#define AST_BASE\
    ast_type_t type;\
    token_t* token_begin;\
    token_t* token_end

typedef struct ast_t ast_t;
typedef vec_t(ast_t) vec_ast_t;
typedef struct ast_abstract_argument_list_t ast_abstract_argument_list_t;
typedef struct ast_decl_var_t ast_decl_var_t;

// Abstract nodes:

typedef struct {
    AST_BASE;
    token_t* token_base_type;
    ast_abstract_argument_list_t* ast_argument_list;
} ast_abstract_datatype_t;

typedef struct {
    AST_BASE;
    vec_ast_t arguments;
} ast_abstract_argument_list_t;

// Declarations:

typedef struct {
    AST_BASE;
    token_t* token_name;
    vec_ast_t children;
} ast_decl_struct_t;

typedef struct {
    AST_BASE;
    ast_datatype_t* datatype;
    ast_argument_list_t* argument_list;
    vec_ast_t expressions;
} ast_decl_function_t;

typedef struct {
    AST_BASE;
    token_t* token_name;
    ast_datatype_t* datatype;
} ast_decl_typedef_t;

typedef struct {
    AST_BASE;
    token_t* token_name;
    ast_datatype_t* datatype;
    token_t* rvalue;
} ast_decl_var_t;

// Expressions:

typedef struct {
    AST_BASE;
    vec_ast_t expressions;
} ast_expr_scope_t;

typedef struct {
    union {
        struct { AST_BASE; };
        ast_datatype_t      ast_datatype;
        ast_argument_list_t ast_argument_list;
        ast_decl_struct_t   decl_struct;
        ast_decl_function_t decl_function;
        ast_decl_typedef_t  decl_typedef;
        ast_decl_var_t      decl_var;
        ast_expr_scope_t    expr_scope;

        /*// Expression data:
        ast_t* _children[2];

        struct { // AST_DECL_STRUCT, AST_ARGUMENT_LIST
            token_t* token_name;
            vec_ptr_t children;
        } _decl;

        struct { // AST_DATATYPE
            token_t* token_base_type;
            ast_t* ast_argument_list; // Only for function pointers
            int ptr_count; // Only for pointers and function pointers
        } _datatype;*/
    };
} ast_t;

static vec_ast_t parse(vec_token_t* tokens) {
    vec_ast_t expressions;
    vec_init(&expressions);
    u32 index = 0;

    token_t* token = tokens.data;

    while (token.type != TOKEN_NULL) {
        int line_number = token->line_number;
        ast_t decl = parse_decl(tokens, &token);
        expr.line_number = line_number;
        assert(token_equals(**token, ";"));

        vec_push(&expressions, expr);
    }

    return expressions;
}

static ast_t parse_decl(token_t** token) {
    ast_t expr;

    switch ((*token)->type) {
    case TOKEN_IDENTIFIER:
        if (token_equals(**token, "struct")) {
            expr.type = AST_DECL_STRUCT;
            expr.token_begin = *token++;
            expr._decl.token_name = **token;
            expr.token_end = *token++;
            assert((*token)->type == TOKEN_IDENTIFIER);
        } else if (token_equals(**token, "typedef")) {
            expr.type = AST_DECL_TYPEDEF;
            expr.token_begin = *token++;
            expr._children[0] = malloc(sizeof(ast_t));
            *expr._children[0] = parse_decl(token);
            expr.token_end = *token - 1;
            return expr;
        } else {
            // AST_DECL_FUNCTION or AST_DECL_VAR:
            ast_t ast_type = parse_type(token);
            assert((*token)->type == TOKEN_LABEL);
            assert((*token + 1)->type == TOKEN_SYMBOL);
            token_t token_name = *(*token++);
            token_t token_symbol = *(*token);
            if (token_equals(token_symbol, "("))
                expr = parse_decl_function(token, ast_type, token_name);
            else
                expr = parse_decl_var(token, ast_type, token_name);
        }
        break;
    default: assert(0);
    }

    if ((*token)->type = TOKEN_NULL) assert(0);
    if ((*token)->type == TOKEN_OPERATOR && token_equals(**token, ";")) return expr;

    switch(expr.type) {
    case AST_DECL_STRUCT: return parse_decl_struct_impl(token, expr);
    case AST_DECL_FUNCTION: return parse_decl_function_impl(token, expr);
    case AST_DECL_VAR: return parse_decl_var_assign(token, expr);
    case AST_DECL_TYPEDEF: assert(0);
    default: assert(0);
    }
}

static ast_t parse_decl_sruct_impl(token_t** token, ast_t expr) {
    assert((*token)->type == TOKEN_OPERATOR && token_equals(**token, "{"));
    *token++;
    assert(0);
}


static ast_t parse_decl_function(token_t** token, ast_t ast_type, token_t token_name) {
    assert(0);
}

static ast_t parse_decl_function_impl(token_t** token, ast_t expr) {
    assert((*token)->type == TOKEN_OPERATOR && token_equals(**token, "{"));
    *token++;
    assert(0);
}

static ast_t parse_decl_var_assign(token_t** token, ast_t expr) {
    assert((*token)->type == TOKEN_OPERATOR && token_equals(**token, "="));
    *token++;
    while((**token).type != TOKEN_NULL && !token_equals(**token, ";")) *token++;
}

static ast_t parse_type(token_t** token) {
    assert((**token).type == TOKEN_IDENTIFIER);
    ast_t expr;
    expr.type = AST_DATATYPE;
    expr.token_begin = *token++;

    for (;;) {
        if ((**token).type == TOKEN_NULL) assert(0);
        if (token_equals(**token, ";")) assert(0);
        if (token_equals(**token, ",")) assert(0);
        // TODO: Function pointers
        if ((**token).type == TOKEN_IDENTIFIER) {
            expr.token_end = *token++;
            return expr;
        }
        *token++;
    }
}

/*typedef struct {
    s64 array_int;
    double array_float;
    char* array_str;
} ast_data_t;*/

/*enum ast_type_t {
    AST_NULL,
    AST_STRUCT_AST,
    AST_FUNCTION_AST
};

typedef struct {
    token_t* token_name;
} ast_struct_decl_t;

typedef struct {
    token_t* token_name;
} ast_function_decl_t;

typedef struct {
    ast_type_t ast_type;
    union {
        ast_struct_decl_t struct_decl;
        ast_function_decl_t function_decl;
    };
} ast_t;*/

#endif // AST_H_INCLUDED
