#include "ast.h"

#include <stdlib.h>
#include <stdio.h>

static AST * ast_dtype_primitive_new(AST dtype) {
    AST_DType_Primitive * self = malloc(sizeof(AST_DType_Primitive));

    self->ast = dtype;

    return &self->ast;
}


AST * ast_dtype_i8_new(void) {
    return ast_dtype_primitive_new(AST_Type_DType_I8);
}


AST * ast_dtype_i16_new(void) {
    return ast_dtype_primitive_new(AST_Type_DType_I16);
}


AST * ast_dtype_i32_new(void) {
    return ast_dtype_primitive_new(AST_Type_DType_I32);
}


AST * ast_dtype_i64_new(void) {
    return ast_dtype_primitive_new(AST_Type_DType_I64);
}


AST * ast_dtype_u8_new(void) {
    return ast_dtype_primitive_new(AST_Type_DType_U8);
}


AST * ast_dtype_u16_new(void) {
    return ast_dtype_primitive_new(AST_Type_DType_U16);
}


AST * ast_dtype_u32_new(void) {
    return ast_dtype_primitive_new(AST_Type_DType_U32);
}


AST * ast_dtype_u64_new(void) {
    return ast_dtype_primitive_new(AST_Type_DType_U64);
}


AST * ast_dtype_char_new(void) {
    return ast_dtype_primitive_new(AST_Type_DType_Char);
}


AST * ast_expr_const_number_new(size_t length, char * c_str) {
    AST_Expr_ConstNumber * self = malloc(sizeof(AST_Expr_ConstNumber));

    *self = (AST_Expr_ConstNumber) {
        .ast = AST_Type_Expr_ConstNumber
        , .length = length
        , .c_str = c_str
    };

    return &self->ast;
}


AST * ast_expr_lambda_new(AST * return_dtype, AST * stmt) {
    AST_Expr_Lambda * self = malloc(sizeof(AST_Expr_Lambda));

    *self = (AST_Expr_Lambda) {
        .ast = AST_Type_Expr_Lambda
        , .return_dtype = return_dtype
        , .stmt = stmt
    };

    return &self->ast;
}


static AST * ast_expr_unary_op_new(AST op, AST * expr) {
    AST_Expr_UnaryOP * self = malloc(sizeof(AST_Expr_UnaryOP));

    *self = (AST_Expr_UnaryOP) {
        .ast = op
        , .expr = expr
    };

    return &self->ast;
}


AST * ast_expr_unary_op_neg_new(AST * expr) {
    return ast_expr_unary_op_new(AST_Type_Expr_UnaryOp_Neg, expr);
}


AST * ast_expr_unary_op_not_new(AST * expr) {
    return ast_expr_unary_op_new(AST_Type_Expr_UnaryOp_Not, expr);
}



AST * ast_stmt_return_new(AST * expr) {
    AST_Stmt_Return * self = malloc(sizeof(AST_Stmt_Return));

    *self = (AST_Stmt_Return) {
        .ast = AST_Type_Stmt_Return
        , .expr = expr
    };

    return &self->ast;
}


AST * ast_stmt_compound_new(void) {
    AST_Stmt_Compound * self = malloc(sizeof(AST_Stmt_Compound));

    *self = (AST_Stmt_Compound) {
        .ast = AST_Type_Stmt_Compound
    };

    return &self->ast;
}


void ast_stmt_compound_append(AST_Stmt_Compound * self, AST * stmt) {
    if(self->size >= self->capacity) {
        self->capacity = (self->capacity + 1) * 2;
        self->stmt = realloc(self->stmt, sizeof(AST*) * self->capacity);
    }

    self->stmt[self->size++] = stmt;
}


AST * ast_decl_alias_new(size_t length, char * alias, AST * expr) {
    AST_Decl_Alias * self = malloc(sizeof(AST_Decl_Alias));

    *self = (AST_Decl_Alias) {
        .ast = AST_Type_Decl_Alias
        , .alias = {.length = length, .c_str = alias}
        , .expr = expr
    };

    return &self->ast;
}


static void _ast_show(AST * self) {
    if(self != NULL) {
        switch(*self) {
            case AST_Type_DType_I8:
                printf("AST {type: AST_DType, dtype: DType_I8}");
                break;
            case AST_Type_DType_I16:
                printf("AST {type: AST_DType, dtype: DType_I16}");
                break;
            case AST_Type_DType_I32:
                printf("AST {type: AST_DType, dtype: DType_I32}");
                break;
            case AST_Type_DType_I64:
                printf("AST {type: AST_DType, dtype: DType_I64}");
                break;
            case AST_Type_DType_U8:
                printf("AST {type: AST_DType, dtype: DType_U8}");
                break;
            case AST_Type_DType_U16:
                printf("AST {type: AST_DType, dtype: DType_U16}");
                break;
            case AST_Type_DType_U32:
                printf("AST {type: AST_DType, dtype: DType_U32}");
                break;
            case AST_Type_DType_U64:
                printf("AST {type: AST_DType, dtype: DType_U64}");
                break;
            case AST_Type_DType_Char:
                printf("AST {type: AST_DType, dtype: DType_Char}");
                break;
            case AST_Type_Decl_Alias:
                printf("AST {type: AST_Decl_Alias, expr: ");
                _ast_show(((AST_Decl_Alias*) self)->expr);
                printf("}");
                break;
            case AST_Type_Expr_ConstNumber:
                printf("AST {type: AST_Expr_ConstNumber, value: %.*s}", (int) ((AST_Expr_ConstNumber*) self)->length, ((AST_Expr_ConstNumber*) self)->c_str);
                break;
            case AST_Type_Expr_Lambda:
                printf("AST {type: AST_Expr_Lambda, return_dtype: ");
                _ast_show(((AST_Expr_Lambda*) self)->return_dtype);
                printf(", stmt: ");
                _ast_show(((AST_Expr_Lambda*) self)->stmt);
                printf("}");
                break;
            case AST_Type_Expr_UnaryOp_Neg:
                printf("AST {type: AST_Expr_UnaryOP_Neg, expr: ");
                _ast_show(((AST_Expr_UnaryOP*)self)->expr);
                printf("}");
                break;
            case AST_Type_Expr_UnaryOp_Not:
                printf("AST {type: AST_Expr_UnaryOP_Not, expr: ");
                _ast_show(((AST_Expr_UnaryOP*)self)->expr);
                printf("}");
                break;
            case AST_Type_Stmt_Return:
                printf("AST {type: AST_Stmt_Return, expr: ");
                _ast_show(((AST_Stmt_Return*) self)->expr);
                printf("}");
                break;
            case AST_Type_Stmt_Compound:
                printf("AST {type: AST_Stmt_Compound, stmt: [");
                for(size_t i = 0; i < ((AST_Stmt_Compound*) self)->size; i++) {
                    printf(i == 0 ? "" : ", ");
                    _ast_show(((AST_Stmt_Compound*) self)->stmt[i]);
                }
                printf("]}");
                break;
        }
    } else {
        printf("(null)");
    }
}


void ast_show(AST * self) {
    _ast_show(self);
    printf("\n");
}


void ast_finalize(AST * self) {
    if(self != NULL) {
        switch(*self) {
            case AST_Type_DType_I8:
            case AST_Type_DType_I16:
            case AST_Type_DType_I32:
            case AST_Type_DType_I64:
            case AST_Type_DType_U8:
            case AST_Type_DType_U16:
            case AST_Type_DType_U32:
            case AST_Type_DType_U64:
            case AST_Type_DType_Char:
                free(self);
                break;
            case AST_Type_Decl_Alias:
                ast_finalize(((AST_Decl_Alias*) self)->expr);
                free(self);
                break;
            case AST_Type_Expr_ConstNumber:
                free(self);
                break;
            case AST_Type_Expr_Lambda:
                ast_finalize(((AST_Expr_Lambda*) self)->return_dtype);
                ast_finalize(((AST_Expr_Lambda*) self)->stmt);
                free(self);
                break;
            case AST_Type_Expr_UnaryOp_Neg:
            case AST_Type_Expr_UnaryOp_Not:
                ast_finalize(((AST_Expr_UnaryOP*) self)->expr);
                free(self);
                break;
            case AST_Type_Stmt_Return:
                ast_finalize(((AST_Stmt_Return*) self)->expr);
                free(self);
                break;
            case AST_Type_Stmt_Compound:
                for(size_t i = 0; i < ((AST_Stmt_Compound*) self)->size; i++) {
                    ast_finalize(((AST_Stmt_Compound*) self)->stmt[i]);
                }
                free(self);
                break;
        }
    }
}


