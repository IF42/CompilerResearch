#ifndef _AST_H_
#define _AST_H_

#include <stddef.h>


typedef enum {
    AST_Type_DType_I8
    , AST_Type_DType_I16
    , AST_Type_DType_I32
    , AST_Type_DType_I64
    , AST_Type_DType_U8
    , AST_Type_DType_U16
    , AST_Type_DType_U32
    , AST_Type_DType_U64
    , AST_Type_DType_Char

    , AST_Type_Decl_Alias

    , AST_Type_Expr_ConstNumber
    , AST_Type_Expr_Lambda
    , AST_Type_Expr_UnaryOp_Not
    , AST_Type_Expr_UnaryOp_Neg

    , AST_Type_Stmt_Return
    , AST_Type_Stmt_Compound
} AST;


typedef struct {
    AST ast;
} AST_DType_Primitive;


AST * ast_dtype_i8_new(void);


AST * ast_dtype_i16_new(void);


AST * ast_dtype_i32_new(void);


AST * ast_dtype_i64_new(void);


AST * ast_dtype_u8_new(void);


AST * ast_dtype_u16_new(void);


AST * ast_dtype_u32_new(void);


AST * ast_dtype_u64_new(void);


AST * ast_dtype_char_new(void);


typedef struct {
    AST ast;
    size_t length;
    char * c_str;
} AST_Expr_ConstNumber;


AST * ast_expr_const_number_new(size_t length, char * c_str);


typedef struct {
    AST ast;
    AST * return_dtype;
    AST * stmt;
} AST_Expr_Lambda;


AST * ast_expr_lambda_new(AST * return_dtype, AST * stmt);


typedef struct {
    AST ast;
    AST * expr;
} AST_Expr_UnaryOP;


AST * ast_expr_unary_op_neg_new(AST * expr);


AST * ast_expr_unary_op_not_new(AST * expr);


typedef struct {
    AST ast;
    AST * expr;
} AST_Stmt_Return;


AST * ast_stmt_return_new(AST * expr);


typedef struct {
    AST ast;
    size_t capacity;
    size_t size;
    AST ** stmt;
} AST_Stmt_Compound;


AST * ast_stmt_compound_new(void);


void ast_stmt_compound_append(AST_Stmt_Compound * self, AST * stmt);


typedef struct {
    AST ast;
 
    struct {   
        size_t length;
        char * c_str;
    } alias;

    AST * expr;
} AST_Decl_Alias;


AST * ast_decl_alias_new(size_t length, char * alias, AST * expr);


void ast_show(AST * self);


void ast_finalize(AST * self);


#endif


