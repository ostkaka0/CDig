#include "ast.h"

#include <assert.h>

vec_ast_t parse(vec_token_t* tokens, vec_void_t* ast_data) {
    vec_ast_t decls;
    vec_init(&decls);
    u32 index = 0;

    memset(ast_data, 0, ast__end__id * sizeof(vec_ast_t));

    token_t* token = tokens->data;

    while (token->type != TOKEN__NULL) {
        int lin_num = token->line_num;
        ast_t decl = parse_decl(&token, ast_data);
        decl.lin_num = lin_num;
        assert(token_equals(*token, ";"));

        vec_push(&decls, decl);
    }

    return decls;
}

#define CREATE_AST_DATA(name, ...)\
    expr.type = ast_##name##__id;\
    expr.id = ast_data[ast_##name##__id].length;\
    ast_##name##_t ast_##name = { __VA_ARGS__ };\
    vec_push((vec_ast_##name##_t*)&ast_data[ast_##name##__id], ast_##name)

ast_t parse_decl(token_t** token, vec_void_t* ast_data) {
    ast_t expr;

    switch ((*token)->type) {
    case TOKEN_IDENTIFIER:
        if (token_equals(**token, "struct")) {
            (*token)++;
            if ((**token).type == TOKEN_IDENTIFIER) {
                CREATE_AST_DATA(decl_struct, *((*token)++), NULL);
            } else if (token_equals(**token, "{")) {
                CREATE_AST_DATA(decl_struct, (u32)-1, NULL);
            } else assert(0);
        } else if (token_equals(**token, "typedef")) {
            (*token)++;
            CREATE_AST_DATA(decl_typedef, parse_decl(token, ast_data));
            return expr;
        } else {
            // AST_DECL_FUNCTION or AST_DECL_VAR:
            ast_t ast_type = parse_type(token, ast_data);
            assert((*token)->type == TOKEN_IDENTIFIER);
            assert(((*token) + 1)->type == TOKEN_OPERATOR);
            token_t token_name = *((*token)++);
            token_t token_symbol = *(*token);
            if (token_equals(token_symbol, "("))
                expr = parse_decl_function(token, ast_data, ast_type, token_name);
            else
                assert(0);//expr = parse_decl_var(token, ast_data, ast_type, token_name);
        }
        break;
    default: assert(0);
    }

    if ((*token)->type = TOKEN__NULL) assert(0);
    if ((*token)->type == TOKEN_OPERATOR && token_equals(**token, ";")) return expr;

    switch(expr.type) {
    case ast_decl_struct__id: return parse_decl_struct_impl(token, ast_data, expr);
    case ast_decl_function__id: return parse_decl_function_impl(token, ast_data, expr);
    case ast_decl_var__id: return parse_decl_var_assign(token, ast_data, expr);
    case ast_decl_typedef__id: assert(0);
    default: assert(0);
    }
    assert(0);
}

ast_t parse_decl_struct_impl(token_t** token, vec_void_t* ast_data, ast_t expr) {
    assert((*token)->type == TOKEN_OPERATOR && token_equals(**token, "{"));
    (*token)++;
    assert(0);
}

ast_t parse_decl_function(token_t** token, vec_void_t* ast_data, ast_t ast_type, token_t token_name) {
    assert(0);
}

ast_t parse_decl_function_impl(token_t** token, vec_void_t* ast_data, ast_t expr) {
    assert((*token)->type == TOKEN_OPERATOR && token_equals(**token, "{"));
    (*token)++;
    assert(0);
}

ast_t parse_decl_var_assign(token_t** token, vec_void_t* ast_data, ast_t expr) {
    assert((*token)->type == TOKEN_OPERATOR && token_equals(**token, "="));
    (*token)++;
    while((**token).type != TOKEN__NULL && !token_equals(**token, ";")) (*token)++;
}

ast_t parse_type(token_t** token, vec_void_t* ast_data) {
    assert((**token).type == TOKEN_IDENTIFIER);
    ast_t expr;
    expr.type = ast_datatype__id;
    (*token)++;//expr.token_begin = (*token)++;

    for (;;) {
        if ((**token).type == TOKEN__NULL) assert(0);
        if (token_equals(**token, ";")) assert(0);
        if (token_equals(**token, ",")) assert(0);
        // TODO: Function pointers
        if ((**token).type == TOKEN_IDENTIFIER) {
            //(*token)++;//expr.token_end = (*token)++;
            return expr;
        }
        (*token)++;
    }
}
