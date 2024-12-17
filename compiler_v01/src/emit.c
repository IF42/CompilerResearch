#include "emit.h"


void emit_return(ASM_Inst_Seq * seq, AST * ret) {
    AST_Expr_ConstNumber * expr = (AST_Expr_ConstNumber*) ((AST_Stmt_Return*) ret)->expr;

    asm_inst_seq_append_instruction(seq, asm_instruction_mov(asm_operand_reg("rcx"), asm_operand_imm(expr->length, expr->c_str)));
    asm_inst_seq_append_instruction(seq, asm_instruction_ret());
}


void emit_compond(ASM_Inst_Seq * seq, AST * compond) {
    for(size_t i = 0; i < ((AST_Stmt_Compound*) compond)->size; i++) {
        switch(*((AST_Stmt_Compound*) compond)->stmt[i]) {
            case AST_Type_Stmt_Compound:
                emit_compond(seq, ((AST_Stmt_Compound*) compond)->stmt[i]);
                break;
            case AST_Type_Stmt_Return:
                emit_return(seq, ((AST_Stmt_Compound*) compond)->stmt[i]);
                break;
            default:
                fprintf(stderr, "unsupported AST node\n");
        }
    }
}


ASM_Inst_Seq emit_lambda(AST * lambda) {
    ASM_Inst_Seq seq = {0};

    switch(*((AST_Expr_Lambda*) lambda)->stmt) {
        case AST_Type_Stmt_Compound:
            emit_compond(&seq, ((AST_Expr_Lambda*) lambda)->stmt);
            break;
        case AST_Type_Stmt_Return:
            emit_return(&seq, ((AST_Expr_Lambda*) lambda)->stmt);
            break;
        default:
            fprintf(stderr, "unsupported AST node\n");
    }

    return seq;
}


ASM_Function emit_function(AST * lambda, size_t length, char * c_str) {
    return asm_function(length, c_str, emit_lambda(lambda));
}


ASM_Prog emit_to_win_x86_64_asm(AST * ast) {
    ASM_Prog asm_prog = {0};
    
    switch(*ast) {
        case AST_Type_Decl_Alias:
            switch(*((AST_Decl_Alias*) ast)->expr) {
                case AST_Type_Expr_Lambda: 
                    asm_prog.function = 
                        emit_function(((AST_Decl_Alias*) ast)->expr, ((AST_Decl_Alias*) ast)->alias.length, ((AST_Decl_Alias*) ast)->alias.c_str);
                    break;
                default:
                    fprintf(stderr, "unsupported AST node\n");
            }
            break;
        default:
            fprintf(stderr, "unsupported AST node\n");
    }

    return asm_prog;
}


