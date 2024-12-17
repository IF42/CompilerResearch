#include "asm_x86_64_emit.h"


ASM_Operand * ir_value_to_asm_operand(IR_Value * value) {
    if(value != NULL) {
        switch(*value) {
            case IR_Value_Type_Const:
                return asm_operand_imm_new(IR_VALUE_CONST(value)->value.length, IR_VALUE_CONST(value)->value.c_str);
            case IR_Value_Type_Var:
                return asm_operand_pseudo_var_new(IR_VALUE_VAR(value)->identifier.length, IR_VALUE_VAR(value)->identifier.c_str);
            case IR_Value_Type_ID:
                return asm_operand_pseudo_id_new(IR_VALUE_ID(value)->id);
        }
    }

    return NULL;
}


static void emit_to_asm_linux_x86_64(IR_Inst_Blk * ir_blk, ASM_Inst_Blk * assembly) {
    for(size_t i = 0; i < ir_blk->size; i++) {
        switch(*ir_blk->inst[i]) {
            case IR_Type_Inst_Neg:
            case IR_Type_Inst_Not:
            case IR_Type_Inst_Ret:
            case IR_Type_FuncProlog:
            case IR_Type_FuncEpilog:
            case IR_Type_Func:
                break;
        }
    }
}


ASM_Inst_Blk ir_emit_to_asm_linux_x86_64(IR_Inst_Blk * ir_blk) {
    ASM_Inst_Blk assembly = {0};
    emit_to_asm_linux_x86_64(ir_blk, &assembly);
    return assembly;
}


static void emit_to_asm_win_x86_64(IR_Inst_Blk * ir_blk, ASM_Inst_Blk * assembly) {
    for(size_t i = 0; i < ir_blk->size; i++) {
        switch(*ir_blk->inst[i]) {
            case IR_Type_Inst_Neg: {
                    ASM_Operand * src = ir_value_to_asm_operand(IR_INST_NEG(ir_blk->inst[i])->src);
                    ASM_Operand * dest1 = ir_value_to_asm_operand(IR_INST_NEG(ir_blk->inst[i])->dest);
                    ASM_Operand * dest2 = ir_value_to_asm_operand(IR_INST_NEG(ir_blk->inst[i])->dest);

                    asm_inst_blk_append(assembly, asm_inst_mov_new(src, dest1));
                    asm_inst_blk_append(assembly, asm_inst_neg_new(dest2));
                }
                break;
            case IR_Type_Inst_Not: {
                    ASM_Operand * src = ir_value_to_asm_operand(IR_INST_NOT(ir_blk->inst[i])->src);
                    ASM_Operand * dest1 = ir_value_to_asm_operand(IR_INST_NOT(ir_blk->inst[i])->dest);
                    ASM_Operand * dest2 = ir_value_to_asm_operand(IR_INST_NOT(ir_blk->inst[i])->dest);

                    asm_inst_blk_append(assembly, asm_inst_mov_new(src, dest1));
                    asm_inst_blk_append(assembly, asm_inst_not_new(dest2));
                }
                break;
            case IR_Type_Inst_Ret: {
                    ASM_Operand * operand = ir_value_to_asm_operand(IR_INST_RET(ir_blk->inst[i])->value);
                    if(operand != NULL) {
                        asm_inst_blk_append(assembly, asm_inst_mov_new(operand, asm_operand_reg_new(ASM_Reg_AX)));
                    }
                    asm_inst_blk_append(assembly, asm_inst_ret_new());
                }
                break;
            case IR_Type_FuncProlog:
                break;
            case IR_Type_FuncEpilog:
                break;
            case IR_Type_Func:
                asm_inst_blk_set_func(assembly, IR_FUNC(ir_blk->inst[i])->identifier.length, IR_FUNC(ir_blk->inst[i])->identifier.c_str);
                break;
        }
    }
}


ASM_Inst_Blk ir_emit_to_asm_win_x86_64(IR_Inst_Blk * ir_blk) {
    ASM_Inst_Blk assembly = {0};
    emit_to_asm_win_x86_64(ir_blk, &assembly);
    return assembly;
}


#include <stdlib.h>


size_t asm_pseudo_pass(ASM_Inst_Blk * assembly) {
    size_t stack_size = 0;

    /* 
     * TODO: improve pseudo-register map for handling string names
     */
    size_t pseudo_reg_map = 0;

    for(size_t i = 0; i < assembly->size; i++) {
        if(*assembly->inst[i] == ASM_Type_Inst_Mov) {
            if(*ASM_INST_MOV(assembly->inst[i])->src == ASM_Operand_Type_PseudoID) {
                ASM_Operand * src = asm_operand_stack_new((ASM_OPERAND_PSEUDO_ID(ASM_INST_MOV(assembly->inst[i])->src)->id + 1) * 8);
                free(ASM_INST_MOV(assembly->inst[i])->src);
                ASM_INST_MOV(assembly->inst[i])->src = src;
            }

            if(*ASM_INST_MOV(assembly->inst[i])->dest == ASM_Operand_Type_PseudoID) {
                ASM_Operand * dest = asm_operand_stack_new((ASM_OPERAND_PSEUDO_ID(ASM_INST_MOV(assembly->inst[i])->dest)->id + 1) * 8);
                free(ASM_INST_MOV(assembly->inst[i])->dest);
                ASM_INST_MOV(assembly->inst[i])->dest = dest;
            }

        } else if(*assembly->inst[i] == ASM_Type_Inst_Neg) {
            if(*ASM_INST_NEG(assembly->inst[i])->operand == ASM_Operand_Type_PseudoID) {
                ASM_Operand * operand = asm_operand_stack_new((ASM_OPERAND_PSEUDO_ID(ASM_INST_NEG(assembly->inst[i])->operand)->id + 1) * 8);
                free(ASM_INST_NEG(assembly->inst[i])->operand);
                ASM_INST_NEG(assembly->inst[i])->operand = operand;
                stack_size += 8;
            }
        } else if(*assembly->inst[i] == ASM_Type_Inst_Not) {
            if(*ASM_INST_NOT(assembly->inst[i])->operand == ASM_Operand_Type_PseudoID) {
                ASM_Operand * operand = asm_operand_stack_new((ASM_OPERAND_PSEUDO_ID(ASM_INST_NOT(assembly->inst[i])->operand)->id + 1) * 8);
                free(ASM_INST_NEG(assembly->inst[i])->operand);
                ASM_INST_NOT(assembly->inst[i])->operand = operand;
                stack_size += 8;
            }
        }
    }

    return stack_size;
}


void asm_fix_assembly(ASM_Inst_Blk * assembly, size_t stack_size) {
    asm_inst_blk_insert(assembly, 0, asm_inst_alloc_stack_new(stack_size));

    for(size_t i = 0; i < assembly->size; i++) {
        if(*assembly->inst[i] == ASM_Type_Inst_Mov) {
            if((*ASM_INST_MOV(assembly->inst[i])->src == ASM_Operand_Type_Stack) && (*ASM_INST_MOV(assembly->inst[i])->dest == ASM_Operand_Type_Stack)) {
                ASM * mov = asm_inst_mov_new(ASM_INST_MOV(assembly->inst[i])->src, asm_operand_reg_new(ASM_Reg_R10));
                ASM_INST_MOV(assembly->inst[i])->src = asm_operand_reg_new(ASM_Reg_R10);
                asm_inst_blk_insert(assembly, i, mov);
            } else if((*ASM_INST_MOV(assembly->inst[i])->src == ASM_Operand_Type_Imm) && (*ASM_INST_MOV(assembly->inst[i])->dest == ASM_Operand_Type_Stack)) {
                ASM * mov = asm_inst_mov_new(ASM_INST_MOV(assembly->inst[i])->src, asm_operand_reg_new(ASM_Reg_R10));
                ASM_INST_MOV(assembly->inst[i])->src = asm_operand_reg_new(ASM_Reg_R10);
                asm_inst_blk_insert(assembly, i, mov);
            }
        }
    }
}




