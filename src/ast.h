#ifndef AST_H_INCLUDED
#define AST_H_INCLUDED

#include "common.h"
#include "token.h"
#include "external/vec.h"

#define ast_is_abstract(ast) (ast.type > AST_DECL__BEGIN && ast.type < AST_DECL__END)
#define ast_is_decl(ast)     (ast.type > AST_DECL__BEGIN && ast.type < AST_DECL__END)
#define ast_is_expr(ast)     (ast.type > AST_EXPR__BEGIN && ast.type < AST_EXPR__END)
#define ast_is_valid(ast)    (ast_is_abstract(ast) || ast_is_decl(ast) || ast_is_expr(ast) || ast.type == AST_TOKENS)

enum { _COUNTER_BASE  = __COUNTER__ };
#define AST_T(name, ...) \
    typedef struct ast_##name##_struct { __VA_ARGS__ } ast_##name##_t;\
    enum { ast_##name##__id = __COUNTER__ - _COUNTER_BASE }; \
    typedef vec_t(ast_##name##_t) vec_ast_##name##_t


enum { ast__null__id = __COUNTER__ };
enum { ast__tokens__id = __COUNTER__ };

typedef struct {
    u32 id;
    u16 type;
    u16 lin_num; // max 65535 lines for error messages
} ast_t;

typedef vec_t(ast_t) vec_ast_t;
typedef struct ast_argument_list_struct ast_argument_list_t;
typedef struct ast_decl_var_struct ast_decl_var_t;

AST_T(datatype,
    token_t* token_base_type;
    ast_argument_list_t* ast_argument_list;
);

AST_T(argument_list,
    vec_ast_t* arguments;
);

AST_T(decl_struct,
    token_t identifier;
    vec_ast_t* vars;
);

AST_T(decl_function,
    ast_datatype_t ast_datatype;
    ast_argument_list_t ast_argument_list;
    vec_ast_t vec_ast_expressions;
);

AST_T(decl_typedef,
   ast_t ast_decl;
);

AST_T(decl_var,
   token_t* token_name;
   ast_datatype_t* datatype;
   token_t* rvalue;
);

AST_T(expr_scope,
    vec_ast_t expressions;
);

enum { ast__end__id = __COUNTER__ - _COUNTER_BASE };

vec_ast_t parse(vec_token_t* tokens, vec_void_t* ast_data);
ast_t parse_decl(token_t** token, vec_void_t* ast_data);
ast_t parse_decl_struct_impl(token_t** token, vec_void_t* ast_data, ast_t expr);
ast_t parse_decl_function(token_t** token, vec_void_t* ast_data, ast_t ast_type, token_t token_name);
ast_t parse_decl_function_impl(token_t** token, vec_void_t* ast_data, ast_t expr);
ast_t parse_decl_var_assign(token_t** token, vec_void_t* ast_data, ast_t expr);
ast_t parse_type(token_t** token, vec_void_t* ast_data);

/*enum ast_type_t {
    ast__null__id = 0,
    ast_tokens__id,

    ast_datatype__id,
    ast_argument_list__id,

    ast_decl_struct__id,
    ast_decl_function__id,
    ast_decl_typedef__id,
    ast_decl_var__id,

    ast_expr_scope__id,
    // todo: ast_expr_defer

    ast__end__id,
    
    ast_abstract__begin__id = ast_datatype__id,
    ast_decl__begin__id = ast_decl_struct__id,
    ast_expr__begin__id = ast_expr_scope__id,
    ast_abstract__end__id = ast_decl__begin__id,
    ast_decl__end__id = ast_expr__begin__id,
    ast_expr__end__id = ast__end__id
};*/ /*

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
    token_t* token_base_type;
    ast_abstract_argument_list_t* ast_argument_list;
} ast_abstract_datatype_t;

typedef struct {
    vec_ast_t arguments;
} ast_abstract_argument_list_t;

// Declarations:

typedef struct {
    vec_ast_t children;
} ast_decl_struct_t;

typedef struct {
    ast_datatype_t* datatype;
    ast_argument_list_t* argument_list;
    vec_ast_t expressions;
} ast_decl_function_t;

typedef struct {
    token_t* token_name;
    ast_datatype_t* datatype;
} ast_decl_typedef_t;

typedef struct {
    token_t* token_name;
    ast_datatype_t* datatype;
    token_t* rvalue;
} ast_decl_var_t;

// Expressions:

typedef struct {
    vec_ast_t expressions;
} ast_expr_scope_t;*/

/*typedef struct ast_struct {
    union {
        struct { AST_BASE; };
        ast_datatype_t      ast_datatype;
        ast_argument_list_t ast_argument_list;
        ast_decl_struct_t   decl_struct;
        ast_decl_function_t decl_function;
        ast_decl_typedef_t  decl_typedef;
        ast_decl_var_t      decl_var;
        ast_expr_scope_t    expr_scope;

        / * // Expression data:
        ast_t* _children[2];

        struct { // AST_DECL_STRUCT, AST_ARGUMENT_LIST
            token_t* token_name;
            vec_ptr_t children;
        } _decl;

        struct { // AST_DATATYPE
            token_t* token_base_type;
            ast_t* ast_argument_list; // Only for function pointers
            int ptr_count; // Only for pointers and function pointers
        } _datatype;* /
    };
} ast_t;*/



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
