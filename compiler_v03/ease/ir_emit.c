#include "ir_emit.h"

#include <stdio.h>
#include <string.h>


typedef struct {
    Alloc * alloc;
    size_t id_cnt;
    IR_Inst_Blk ir;
} IR_Trans;


static IR_Value * emit(IR_Trans * self, AST * ast) {
    switch(*ast) {
        case AST_Type_DType_I8:
            break;
        case AST_Type_DType_I16:
            break;
        case AST_Type_DType_I32:
            break;
        case AST_Type_DType_I64:
            break;
        case AST_Type_DType_U8:
            break;
        case AST_Type_DType_U16:
            break;
        case AST_Type_DType_U32:
            break;
        case AST_Type_DType_U64:
            break;
        case AST_Type_DType_Char:
            break;
        case AST_Type_Decl_Alias:
            if(*AST_DECL_ALIAS(ast)->expr == AST_Type_Expr_Lambda) {
                ir_inst_blk_append(self->alloc, &self->ir, ir_func_new(self->alloc, AST_DECL_ALIAS(ast)->alias.length, AST_DECL_ALIAS(ast)->alias.c_str));
                return emit(self, AST_DECL_ALIAS(ast)->expr);
            } 
            break;
        case AST_Type_Expr_ConstNumber:
            if(ast != NULL) {
                return ir_value_const_new(self->alloc, AST_EXPR_CONST_NUMBER(ast)->length, AST_EXPR_CONST_NUMBER(ast)->c_str);
            } else {
                return NULL;
            }
        case AST_Type_Expr_Lambda:
            ir_inst_blk_append(self->alloc, &self->ir, ir_func_prolog_new(self->alloc));
            emit(self, AST_EXPR_LAMBDA(ast)->stmt);
            ir_inst_blk_append(self->alloc, &self->ir, ir_func_epilog_new(self->alloc));
            return NULL;
        case AST_Type_Expr_UnaryOp_Not: {
                IR_Value * src = emit(self, AST_EXPR_UNARY_OP(ast)->expr);
                IR_Value * dest = ir_value_var_id_new(self->alloc, self->id_cnt++);
                ir_inst_blk_append(self->alloc, &self->ir, ir_inst_not_new(self->alloc, dest, src));
                return dest;
            }
        case AST_Type_Expr_UnaryOp_Neg: {
                IR_Value * src = emit(self, AST_EXPR_UNARY_OP(ast)->expr);
                IR_Value * dest = ir_value_var_id_new(self->alloc, self->id_cnt++);
                ir_inst_blk_append(self->alloc, &self->ir, ir_inst_neg_new(self->alloc, dest, src));
                return dest;
            }
        case AST_Type_Expr_BinaryOp_Add: {
                IR_Value * src1 = emit(self, AST_EXPR_BINARY_OP(ast)->left_expr);
                IR_Value * src2 = emit(self, AST_EXPR_BINARY_OP(ast)->right_expr);
                IR_Value * dest = ir_value_var_id_new(self->alloc, self->id_cnt++);
                ir_inst_blk_append(self->alloc, &self->ir, ir_inst_add_new(self->alloc, dest, src1, src2));
                return dest;
            }
        case AST_Type_Expr_BinaryOp_Subtract: {
                IR_Value * src1 = emit(self, AST_EXPR_BINARY_OP(ast)->left_expr);
                IR_Value * src2 = emit(self, AST_EXPR_BINARY_OP(ast)->right_expr);
                IR_Value * dest = ir_value_var_id_new(self->alloc, self->id_cnt++);
                ir_inst_blk_append(self->alloc, &self->ir, ir_inst_subtract_new(self->alloc, dest, src1, src2));
                return dest;
            }
        case AST_Type_Expr_BinaryOp_Multiply: {
                IR_Value * src1 = emit(self, AST_EXPR_BINARY_OP(ast)->left_expr);
                IR_Value * src2 = emit(self, AST_EXPR_BINARY_OP(ast)->right_expr);
                IR_Value * dest = ir_value_var_id_new(self->alloc, self->id_cnt++);
                ir_inst_blk_append(self->alloc, &self->ir, ir_inst_multiply_new(self->alloc, dest, src1, src2));
                return dest;
            }
        case AST_Type_Expr_BinaryOp_Divide: {
                IR_Value * src1 = emit(self, AST_EXPR_BINARY_OP(ast)->left_expr);
                IR_Value * src2 = emit(self, AST_EXPR_BINARY_OP(ast)->right_expr);
                IR_Value * dest = ir_value_var_id_new(self->alloc, self->id_cnt++);
                ir_inst_blk_append(self->alloc, &self->ir, ir_inst_divide_new(self->alloc, dest, src1, src2));
                return dest;
            }
        case AST_Type_Expr_BinaryOp_Reminder: {
                IR_Value * src1 = emit(self, AST_EXPR_BINARY_OP(ast)->left_expr);
                IR_Value * src2 = emit(self, AST_EXPR_BINARY_OP(ast)->right_expr);
                IR_Value * dest = ir_value_var_id_new(self->alloc, self->id_cnt++);
                ir_inst_blk_append(self->alloc, &self->ir, ir_inst_reminder_new(self->alloc, dest, src1, src2));
                return dest;
            }
        case AST_Type_Stmt_Return: {
                IR_Value * value = emit(self, AST_STMT_RETURN(ast)->expr);
                ir_inst_blk_append(self->alloc, &self->ir, ir_inst_ret_new(self->alloc, value));
                return NULL;
            }
        case AST_Type_Stmt_Compound:
            for(size_t i = 0; i < AST_STMT_COMPOND(ast)->size; i++) {
                emit(self, AST_STMT_COMPOND(ast)->stmt[i]);
            }
            return NULL;
        default:
            fprintf(stderr, "unknown AST node %d\n", *ast);
    }

    return NULL;
}


IR_Inst_Blk ir_emit(Alloc * alloc, AST * ast) {
    IR_Trans ir_trans = {.alloc = alloc, .ir = {0}, .id_cnt = 0};
    emit(&ir_trans, ast);
    return ir_trans.ir;
}



