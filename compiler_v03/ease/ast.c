#include "ast.h"

#include <stdlib.h>
#include <stdio.h>

static AST * ast_dtype_primitive_new(Alloc * alloc, AST dtype) {
    AST_DType_Primitive * self = new(alloc, sizeof(AST_DType_Primitive));
    self->ast = dtype;
    return &self->ast;
}


AST * ast_dtype_i8_new(Alloc * alloc) {
    return ast_dtype_primitive_new(alloc, AST_Type_DType_I8);
}


AST * ast_dtype_i16_new(Alloc * alloc) {
    return ast_dtype_primitive_new(alloc, AST_Type_DType_I16);
}


AST * ast_dtype_i32_new(Alloc * alloc) {
    return ast_dtype_primitive_new(alloc, AST_Type_DType_I32);
}


AST * ast_dtype_i64_new(Alloc * alloc) {
    return ast_dtype_primitive_new(alloc, AST_Type_DType_I64);
}


AST * ast_dtype_u8_new(Alloc * alloc) {
    return ast_dtype_primitive_new(alloc, AST_Type_DType_U8);
}


AST * ast_dtype_u16_new(Alloc * alloc) {
    return ast_dtype_primitive_new(alloc, AST_Type_DType_U16);
}


AST * ast_dtype_u32_new(Alloc * alloc) {
    return ast_dtype_primitive_new(alloc, AST_Type_DType_U32);
}


AST * ast_dtype_u64_new(Alloc * alloc) {
    return ast_dtype_primitive_new(alloc, AST_Type_DType_U64);
}


AST * ast_dtype_char_new(Alloc * alloc) {
    return ast_dtype_primitive_new(alloc, AST_Type_DType_Char);
}


AST * ast_expr_const_number_new(Alloc * alloc, size_t length, char * c_str) {
    AST_Expr_ConstNumber * self = new(alloc, sizeof(AST_Expr_ConstNumber));

    *self = (AST_Expr_ConstNumber) {
        .ast = AST_Type_Expr_ConstNumber
        , .length = length
        , .c_str = c_str
    };

    return &self->ast;
}


AST * ast_expr_lambda_new(Alloc * alloc, AST * return_dtype, AST * stmt) {
    AST_Expr_Lambda * self = new(alloc, sizeof(AST_Expr_Lambda));

    *self = (AST_Expr_Lambda) {
        .ast = AST_Type_Expr_Lambda
        , .return_dtype = return_dtype
        , .stmt = stmt
    };

    return &self->ast;
}


static AST * ast_expr_unary_op_new(Alloc * alloc, AST op, AST * expr) {
    AST_Expr_UnaryOP * self = new(alloc, sizeof(AST_Expr_UnaryOP));

    *self = (AST_Expr_UnaryOP) {
        .ast = op
        , .expr = expr
    };

    return &self->ast;
}


AST * ast_expr_unary_op_neg_new(Alloc * alloc, AST * expr) {
    return ast_expr_unary_op_new(alloc, AST_Type_Expr_UnaryOp_Neg, expr);
}


AST * ast_expr_unary_op_not_new(Alloc * alloc, AST * expr) {
    return ast_expr_unary_op_new(alloc, AST_Type_Expr_UnaryOp_Not, expr);
}


static AST * ast_expr_binary_op_new(Alloc * alloc, AST op, AST * left_expr, AST * right_expr) {
    AST_Expr_BinaryOp * self = new(alloc, sizeof(AST_Expr_BinaryOp));

    *self = (AST_Expr_BinaryOp) {
        .ast = op
        , .left_expr = left_expr
        , .right_expr = right_expr
    };

    return &self->ast;
}


AST * ast_expr_binary_op_add_new(Alloc * alloc, AST * left_expr, AST * right_expr) {
    return ast_expr_binary_op_new(alloc, AST_Type_Expr_BinaryOp_Add, left_expr, right_expr);
}


AST * ast_expr_binary_op_subtract_new(Alloc * alloc, AST * left_expr, AST * right_expr) {
    return ast_expr_binary_op_new(alloc, AST_Type_Expr_BinaryOp_Subtract, left_expr, right_expr);
}


AST * ast_expr_binary_op_multiply_new(Alloc * alloc, AST * left_expr, AST * right_expr) {
    return ast_expr_binary_op_new(alloc, AST_Type_Expr_BinaryOp_Multiply, left_expr, right_expr);
}


AST * ast_expr_binary_op_divide_new(Alloc * alloc, AST * left_expr, AST * right_expr) {
    return ast_expr_binary_op_new(alloc, AST_Type_Expr_BinaryOp_Divide, left_expr, right_expr);
}


AST * ast_expr_binary_op_reminder_new(Alloc * alloc, AST * left_expr, AST * right_expr) {
    return ast_expr_binary_op_new(alloc, AST_Type_Expr_BinaryOp_Reminder, left_expr, right_expr);
}


AST * ast_stmt_return_new(Alloc * alloc, AST * expr) {
    AST_Stmt_Return * self = new(alloc, sizeof(AST_Stmt_Return));

    *self = (AST_Stmt_Return) {
        .ast = AST_Type_Stmt_Return
        , .expr = expr
    };

    return &self->ast;
}


AST * ast_stmt_compound_new(Alloc * alloc) {
    AST_Stmt_Compound * self = new(alloc, sizeof(AST_Stmt_Compound));

    *self = (AST_Stmt_Compound) {
        .ast = AST_Type_Stmt_Compound
    };

    return &self->ast;
}


#include <string.h>


void ast_stmt_compound_append(Alloc * alloc, AST_Stmt_Compound * self, AST * stmt) {
    if(self->size >= self->capacity) {
        self->capacity = (self->capacity + 1) * 2;
        AST ** arr = new(alloc, sizeof(AST*) * self->capacity);
        memcpy(arr, self->stmt, sizeof(AST*) * self->size);
        delete(alloc, self->stmt);
        self->stmt = arr;
    }

    self->stmt[self->size++] = stmt;
}


AST * ast_decl_alias_new(Alloc * alloc, size_t length, char * alias, AST * expr) {
    AST_Decl_Alias * self = new(alloc, sizeof(AST_Decl_Alias));

    *self = (AST_Decl_Alias) {
        .ast = AST_Type_Decl_Alias
        , .alias = {.length = length, .c_str = alias}
        , .expr = expr
    };

    return &self->ast;
}



static void _ast_show(AST * self, int indentation, FILE * stream) {
    if(self != NULL) {
        switch(*self) {
            case AST_Type_DType_I8:
                fprintf(stream, "DType_I8");
                break;
            case AST_Type_DType_I16:
                fprintf(stream, "DType_I16");
                break;
            case AST_Type_DType_I32:
                fprintf(stream, "DType_I32");
                break;
            case AST_Type_DType_I64:
                fprintf(stream, "DType_I64");
                break;
            case AST_Type_DType_U8:
                fprintf(stream, "DType_U8");
                break;
            case AST_Type_DType_U16:
                fprintf(stream, "DType_U16");
                break;
            case AST_Type_DType_U32:
                fprintf(stream, "DType_U32");
                break;
            case AST_Type_DType_U64:
                fprintf(stream, "DType_U64");
                break;
            case AST_Type_DType_Char:
                fprintf(stream, "DType_Char");
                break;
            case AST_Type_Decl_Alias:
                fprintf(stream, "%*sDecl_Alias (\n", indentation, "");
                fprintf(stream, "%*sname: \"%.*s\"\n", indentation + 2, "", (int) AST_DECL_ALIAS(self)->alias.length, AST_DECL_ALIAS(self)->alias.c_str);
                fprintf(stream, "%*s, expr: ", indentation + 2, "");
                _ast_show(((AST_Decl_Alias*) self)->expr, indentation + 3, stream);
                fprintf(stream, "\n%*s)", indentation+1, "");
                break;
            case AST_Type_Expr_ConstNumber:
                fprintf(stream, "ConstNumber(%.*s)"
                        , (int) ((AST_Expr_ConstNumber*) self)->length, ((AST_Expr_ConstNumber*) self)->c_str);
                break;
            case AST_Type_Expr_Lambda:
                fprintf(stream, "Lambda(\n");
                fprintf(stream, "%*sreturn_dtype: ", indentation+1, "");
                _ast_show(((AST_Expr_Lambda*) self)->return_dtype, indentation + 2, stream);
                fprintf(stream, "\n%*s, body: ", indentation+1, "");
                _ast_show(((AST_Expr_Lambda*) self)->stmt, indentation + 2, stream);
                fprintf(stream, "\n%*s)", indentation, "");
                break;
            case AST_Type_Expr_UnaryOp_Neg:
                fprintf(stream, "Expr_UnaryOP_Neg(\n");
                fprintf(stream, "%*sexpr: ", indentation+1, "");
                _ast_show(((AST_Expr_UnaryOP*)self)->expr, indentation + 2, stream);
                fprintf(stream, "\n%*s)", indentation, "");
                break;
            case AST_Type_Expr_UnaryOp_Not:
                fprintf(stream, "Expr_UnaryOP_Not(\n");
                fprintf(stream, "%*sexpr: ", indentation+1, "");
                _ast_show(((AST_Expr_UnaryOP*)self)->expr, indentation + 2, stream);
                fprintf(stream, "\n%*s)", indentation, "");
                break;
            case AST_Type_Expr_BinaryOp_Add:
                fprintf(stream, "Expr_BinaryOp_Add(\n");
                fprintf(stream, "%*sleft_expr: ", indentation+1, "");
                _ast_show(AST_EXPR_BINARY_OP(self)->left_expr, indentation + 2, stream);
                fprintf(stream, "\n%*s, right_expr: ", indentation+1, "");
                _ast_show(AST_EXPR_BINARY_OP(self)->right_expr, indentation + 2, stream);
                fprintf(stream, "\n%*s)", indentation, "");
                break;
            case AST_Type_Expr_BinaryOp_Subtract:
                fprintf(stream, "Expr_BinaryOp_Sumbtract(\n");
                fprintf(stream, "%*sleft_expr: ", indentation+1, "");
                _ast_show(AST_EXPR_BINARY_OP(self)->left_expr, indentation + 2, stream);
                fprintf(stream, "\n%*s, right_expr: ", indentation + 1, "");
                _ast_show(AST_EXPR_BINARY_OP(self)->right_expr, indentation + 2, stream);
                fprintf(stream, "\n%*s)", indentation, "");
                break;
            case AST_Type_Expr_BinaryOp_Multiply:
                fprintf(stream, "Expr_BinaryOp_Multiply(\n");
                fprintf(stream, "%*sleft_expr: ", indentation+1, "");
                _ast_show(AST_EXPR_BINARY_OP(self)->left_expr, indentation + 2, stream);
                fprintf(stream, "\n%*s, right_expr: ", indentation, "");
                _ast_show(AST_EXPR_BINARY_OP(self)->right_expr, indentation + 2, stream);
                fprintf(stream, "\n%*s)", indentation, "");
                break;
            case AST_Type_Expr_BinaryOp_Divide:
                fprintf(stream, "Expr_BinaryOp_Divide(\n");
                fprintf(stream, "%*sleft_expr: ", indentation+1, "");
                _ast_show(AST_EXPR_BINARY_OP(self)->left_expr, indentation + 2, stream);
                fprintf(stream, "\n%*s, right_expr: ", indentation + 1, "");
                _ast_show(AST_EXPR_BINARY_OP(self)->right_expr, indentation + 2, stream);
                fprintf(stream, "\n%*s)", indentation, "");
                break;
            case AST_Type_Expr_BinaryOp_Reminder:
                fprintf(stream, "Expr_BinaryOp_Reminder(\n");
                fprintf(stream, "%*sleft_expr: ", indentation+1, "");
                _ast_show(AST_EXPR_BINARY_OP(self)->left_expr, indentation + 2, stream);
                fprintf(stream, "\n%*s, right_expr: ", indentation+1, "");
                _ast_show(AST_EXPR_BINARY_OP(self)->right_expr, indentation + 2, stream);
                fprintf(stream, "\n%*s)", indentation, "");
                break;
            case AST_Type_Stmt_Return:
                fprintf(stream, "Stmt_Return(\n");
                fprintf(stream, "%*sexpr: ", indentation + 1, "");
                _ast_show(((AST_Stmt_Return*) self)->expr, indentation + 2, stream);
                fprintf(stream, "\n%*s)", indentation, "");
                break;
            case AST_Type_Stmt_Compound:
                fprintf(stream, "Stmt_Compound(\n");
                for(size_t i = 0; i < ((AST_Stmt_Compound*) self)->size; i++) {
                    fprintf(stream, i == 0 ? "%*s" : ", %*s\n", indentation+1, "");
                    _ast_show(((AST_Stmt_Compound*) self)->stmt[i], indentation + 2, stream);
                }
                fprintf(stream, "\n%*s)", indentation, "");
                break;
        }
    } else {
        fprintf(stream, "(null)");
    }
}


void ast_show(AST * self, FILE * stream) {
    fprintf(stream, "Program (\n");
    _ast_show(self, 1, stream);
    fprintf(stream, "\n)\n");
}



