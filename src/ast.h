#ifndef AST_H_INCLUDED
#define AST_H_INCLUDED

/*typedef struct {
    s64 array_int;
    double array_float;
    char* array_str;
} ast_data_t;*/

enum expr_type_t {
    EXPR_NULL,
    EXPR_STRUCT_DECL,
    EXPR_FUNCTION_DECL
};

typedef struct {
    char* name;

} expr_struct_decl_t;

typedef struct {
    int name_token;
} expr_function_decl_t;

typedef struct {
    expr_type_t expr_type;
    union {
        expr_struct_decl_t struct_decl;
        expr_function_decl_t function_decl;
    };
} expr_t;

#endif // AST_H_INCLUDED
