#include "ir_emit.h"

#include <stdio.h>
#include <string.h>


static size_t id_cnt = 0;


static IR_Value * emit(AST * ast, IR_Inst_Blk * ir) {
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
                ir_inst_blk_append(ir, ir_func_new(AST_DECL_ALIAS(ast)->alias.length, AST_DECL_ALIAS(ast)->alias.c_str));
                return emit(AST_DECL_ALIAS(ast)->expr, ir);
            } 
            break;
        case AST_Type_Expr_ConstNumber:
            if(ast != NULL) {
                return ir_value_const_new(AST_EXPR_CONST_NUMBER(ast)->length, AST_EXPR_CONST_NUMBER(ast)->c_str);
            } else {
                return NULL;
            }
        case AST_Type_Expr_Lambda:
            ir_inst_blk_append(ir, ir_func_prolog_new());
            emit(AST_EXPR_LAMBDA(ast)->stmt, ir);
            ir_inst_blk_append(ir, ir_func_epilog_new());
            return NULL;
        case AST_Type_Expr_UnaryOp_Not: {
                IR_Value * src = emit(AST_EXPR_UNARY_OP(ast)->expr, ir);
                IR_Value * dest = ir_value_var_id_new(id_cnt++);
                ir_inst_blk_append(ir, ir_inst_not_new(dest, src));
                return dest;
            }
        case AST_Type_Expr_UnaryOp_Neg: {
                IR_Value * src = emit(AST_EXPR_UNARY_OP(ast)->expr, ir);
                IR_Value * dest = ir_value_var_id_new(id_cnt++);
                ir_inst_blk_append(ir, ir_inst_neg_new(dest, src));
                return dest;
            }
        case AST_Type_Stmt_Return: {
                IR_Value * value = emit(AST_STMT_RETURN(ast)->expr, ir);
                ir_inst_blk_append(ir, ir_inst_ret_new(value));
                return NULL;
            }
        case AST_Type_Stmt_Compound:
            for(size_t i = 0; i < AST_STMT_COMPOND(ast)->size; i++) {
                emit(AST_STMT_COMPOND(ast)->stmt[i], ir);
            }
            return NULL;
        default:
            fprintf(stderr, "unknown AST node %d\n", *ast);
    }

    return NULL;
}


IR_Inst_Blk ir_emit(AST * ast) {
    IR_Inst_Blk ir = {0};
    emit(ast, &ir);
    return ir;
}



