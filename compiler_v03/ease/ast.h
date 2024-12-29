#ifndef _AST_H_
#define _AST_H_

#include <stdlib.h>
#include <stddef.h>
#include <alloc/alloc.h>


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
    , AST_Type_Expr_BinaryOp_Add
    , AST_Type_Expr_BinaryOp_Subtract
    , AST_Type_Expr_BinaryOp_Multiply
    , AST_Type_Expr_BinaryOp_Divide
    , AST_Type_Expr_BinaryOp_Reminder

    , AST_Type_Stmt_Return
    , AST_Type_Stmt_Compound
} AST;


typedef struct {
    AST ast;
} AST_DType_Primitive;

#define AST_DTYPE_PRIMITIVE(T) ((AST_DType_Primitive*) (T))


AST * ast_dtype_i8_new(Alloc * alloc);


AST * ast_dtype_i16_new(Alloc * alloc);


AST * ast_dtype_i32_new(Alloc * alloc);


AST * ast_dtype_i64_new(Alloc * alloc);


AST * ast_dtype_u8_new(Alloc * alloc);


AST * ast_dtype_u16_new(Alloc * alloc);


AST * ast_dtype_u32_new(Alloc * alloc);


AST * ast_dtype_u64_new(Alloc * alloc);


AST * ast_dtype_char_new(Alloc * alloc);


typedef struct {
    AST ast;
    size_t length;
    char * c_str;
} AST_Expr_ConstNumber;


#define AST_EXPR_CONST_NUMBER(T) ((AST_Expr_ConstNumber*) (T))


AST * ast_expr_const_number_new(Alloc * alloc, size_t length, char * c_str);


typedef struct {
    AST ast;
    AST * return_dtype;
    AST * stmt;
} AST_Expr_Lambda;


#define AST_EXPR_LAMBDA(T) ((AST_Expr_Lambda*) (T))


AST * ast_expr_lambda_new(Alloc * alloc, AST * return_dtype, AST * stmt);


typedef struct {
    AST ast;
    AST * expr;
} AST_Expr_UnaryOP;


#define AST_EXPR_UNARY_OP(T) ((AST_Expr_UnaryOP*) (T))


AST * ast_expr_unary_op_neg_new(Alloc * alloc, AST * expr);


AST * ast_expr_unary_op_not_new(Alloc * alloc, AST * expr);
    

typedef struct {
    AST ast;
    AST * left_expr;
    AST * right_expr;
} AST_Expr_BinaryOp;


#define AST_EXPR_BINARY_OP(T) ((AST_Expr_BinaryOp*) (T))


AST * ast_expr_binary_op_add_new(Alloc * alloc, AST * left_expr, AST * right_expr);


AST * ast_expr_binary_op_subtract_new(Alloc * alloc, AST * left_expr, AST * right_expr);


AST * ast_expr_binary_op_multiply_new(Alloc * alloc, AST * left_expr, AST * right_expr);


AST * ast_expr_binary_op_divide_new(Alloc * alloc, AST * left_expr, AST * right_expr);


AST * ast_expr_binary_op_reminder_new(Alloc * alloc, AST * left_expr, AST * right_expr);


typedef struct {
    AST ast;
    AST * expr;
} AST_Stmt_Return;


#define AST_STMT_RETURN(T) ((AST_Stmt_Return*) (T))


AST * ast_stmt_return_new(Alloc * alloc, AST * expr);


typedef struct {
    AST ast;
    size_t capacity;
    size_t size;
    AST ** stmt;
} AST_Stmt_Compound;


#define AST_STMT_COMPOND(T) ((AST_Stmt_Compound *) (T))


AST * ast_stmt_compound_new(Alloc * alloc);


void ast_stmt_compound_append(Alloc * alloc, AST_Stmt_Compound * self, AST * stmt);


typedef struct {
    AST ast;
 
    struct {   
        size_t length;
        char * c_str;
    } alias;

    AST * expr;
} AST_Decl_Alias;


#define AST_DECL_ALIAS(T) ((AST_Decl_Alias*) (T))


AST * ast_decl_alias_new(Alloc * alloc, size_t length, char * alias, AST * expr);


#include <stdio.h>

void ast_show(AST * self, FILE * stream);



#endif


