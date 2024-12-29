#include "asm_x86_64_emit.h"


static void asm_win_x86_64_emit(ASM * self, FILE * stream) {
    fprintf(stream, "extern ExitProcess\n");
    fprintf(stream, "extern _start\n");
    fprintf(stream, "section .text\n");
    fprintf(stream, "_start:\n");
    
    if(self->inst_blk.identifier.length > 0) {
        fprintf(stream, "\tcall %.*s\n", (int) self->inst_blk.identifier.length, self->inst_blk.identifier.c_str);
        fprintf(stream, "\tmov rcx, rax\n");
    } else {
        fprintf(stream, "\tmov rcx, 0\n");
    }

    fprintf(stream, "\tcall ExitProcess\n");

    asm_x86_64_inst_emit(self, stream);
}


static void emit_to_asm_win_x86_64(ASM * self, IR_Inst_Blk * ir_blk) {
    for(size_t i = 0; i < ir_blk->size; i++) {
        switch(*ir_blk->inst[i]) {
            case IR_Type_Inst_Neg: {
                    ASM_Operand * src = ir_value_to_asm_operand(self->alloc, IR_INST_NEG(ir_blk->inst[i])->src);
                    ASM_Operand * dest1 = ir_value_to_asm_operand(self->alloc, IR_INST_NEG(ir_blk->inst[i])->dest);
                    ASM_Operand * dest2 = ir_value_to_asm_operand(self->alloc, IR_INST_NEG(ir_blk->inst[i])->dest);

                    asm_inst_blk_append(self->alloc, &self->inst_blk, asm_inst_mov_new(self->alloc, src, dest1));
                    asm_inst_blk_append(self->alloc, &self->inst_blk, asm_inst_neg_new(self->alloc, dest2));
                }
                break;
            case IR_Type_Inst_Not: {
                    ASM_Operand * src = ir_value_to_asm_operand(self->alloc, IR_INST_NOT(ir_blk->inst[i])->src);
                    ASM_Operand * dest1 = ir_value_to_asm_operand(self->alloc, IR_INST_NOT(ir_blk->inst[i])->dest);
                    ASM_Operand * dest2 = ir_value_to_asm_operand(self->alloc, IR_INST_NOT(ir_blk->inst[i])->dest);

                    asm_inst_blk_append(self->alloc, &self->inst_blk, asm_inst_mov_new(self->alloc, src, dest1));
                    asm_inst_blk_append(self->alloc, &self->inst_blk, asm_inst_not_new(self->alloc, dest2));
                }
                break;
            case IR_Type_Inst_Ret: {
                    ASM_Operand * operand = ir_value_to_asm_operand(self->alloc, IR_INST_RET(ir_blk->inst[i])->value);
                    if(operand != NULL) {
                        asm_inst_blk_append(
                            self->alloc, &self->inst_blk
                            , asm_inst_mov_new(self->alloc, operand, asm_operand_reg_new(self->alloc, ASM_Reg_AX)));
                    }
                    asm_inst_blk_append(self->alloc, &self->inst_blk, asm_inst_ret_new(self->alloc));
                }
                break;
            case IR_Type_Inst_Add: {
                    ASM_Operand * src1 = ir_value_to_asm_operand(self->alloc, IR_INST_ADD(ir_blk->inst[i])->src1);
                    ASM_Operand * src2 = ir_value_to_asm_operand(self->alloc, IR_INST_ADD(ir_blk->inst[i])->src2);
                    ASM_Operand * dest1 = ir_value_to_asm_operand(self->alloc, IR_INST_ADD(ir_blk->inst[i])->dest);
                    ASM_Operand * dest2 = ir_value_to_asm_operand(self->alloc, IR_INST_ADD(ir_blk->inst[i])->dest);

                    asm_inst_blk_append(self->alloc, &self->inst_blk, asm_inst_mov_new(self->alloc, src1, dest1));
                    asm_inst_blk_append(self->alloc, &self->inst_blk, asm_inst_add_new(self->alloc, src2, dest2));
                }
                break;
            case IR_Type_Inst_Subtract: {
                    ASM_Operand * src1 = ir_value_to_asm_operand(self->alloc, IR_INST_SUBTRACT(ir_blk->inst[i])->src1);
                    ASM_Operand * src2 = ir_value_to_asm_operand(self->alloc, IR_INST_SUBTRACT(ir_blk->inst[i])->src2);
                    ASM_Operand * dest1 = ir_value_to_asm_operand(self->alloc, IR_INST_SUBTRACT(ir_blk->inst[i])->dest);
                    ASM_Operand * dest2 = ir_value_to_asm_operand(self->alloc, IR_INST_SUBTRACT(ir_blk->inst[i])->dest);

                    asm_inst_blk_append(self->alloc, &self->inst_blk, asm_inst_mov_new(self->alloc, src1, dest1));
                    asm_inst_blk_append(self->alloc, &self->inst_blk, asm_inst_subtract_new(self->alloc, src2, dest2));
                }
                break;
            case IR_Type_Inst_Multiply: {
                    ASM_Operand * src1 = ir_value_to_asm_operand(self->alloc, IR_INST_MULTIPLY(ir_blk->inst[i])->src1);
                    ASM_Operand * src2 = ir_value_to_asm_operand(self->alloc, IR_INST_MULTIPLY(ir_blk->inst[i])->src2);
                    ASM_Operand * dest1 = ir_value_to_asm_operand(self->alloc, IR_INST_MULTIPLY(ir_blk->inst[i])->dest);
                    ASM_Operand * dest2 = ir_value_to_asm_operand(self->alloc, IR_INST_MULTIPLY(ir_blk->inst[i])->dest);

                    asm_inst_blk_append(self->alloc, &self->inst_blk, asm_inst_mov_new(self->alloc, src1, dest1));
                    asm_inst_blk_append(self->alloc, &self->inst_blk, asm_inst_multiply_new(self->alloc, src2, dest2));
                }
                break;
            case IR_Type_Inst_Divide: {
                    ASM_Operand * src1 = ir_value_to_asm_operand(self->alloc, IR_INST_MULTIPLY(ir_blk->inst[i])->src1);
                    ASM_Operand * src2 = ir_value_to_asm_operand(self->alloc, IR_INST_MULTIPLY(ir_blk->inst[i])->src2);
                    ASM_Operand * dest = ir_value_to_asm_operand(self->alloc, IR_INST_MULTIPLY(ir_blk->inst[i])->dest);
                    
                    asm_inst_blk_append(
                        self->alloc, &self->inst_blk
                        , asm_inst_mov_new(self->alloc, src1, asm_operand_reg_new(self->alloc, ASM_Reg_AX)));

                    asm_inst_blk_append(self->alloc, &self->inst_blk, asm_inst_cdq_new(self->alloc));
                    asm_inst_blk_append(self->alloc, &self->inst_blk, asm_inst_idiv_new(self->alloc, src2));
                    asm_inst_blk_append(self->alloc, &self->inst_blk, asm_inst_mov_new(self->alloc, asm_operand_reg_new(self->alloc, ASM_Reg_AX), dest));
                }
                break;
            case IR_Type_Inst_Reminder: {
                    ASM_Operand * src1 = ir_value_to_asm_operand(self->alloc, IR_INST_MULTIPLY(ir_blk->inst[i])->src1);
                    ASM_Operand * src2 = ir_value_to_asm_operand(self->alloc, IR_INST_MULTIPLY(ir_blk->inst[i])->src2);
                    ASM_Operand * dest = ir_value_to_asm_operand(self->alloc, IR_INST_MULTIPLY(ir_blk->inst[i])->dest);
                    
                    asm_inst_blk_append(
                        self->alloc, &self->inst_blk
                        , asm_inst_mov_new(self->alloc, src1, asm_operand_reg_new(self->alloc, ASM_Reg_AX)));

                    asm_inst_blk_append(self->alloc, &self->inst_blk, asm_inst_cdq_new(self->alloc));
                    asm_inst_blk_append(self->alloc, &self->inst_blk, asm_inst_idiv_new(self->alloc, src2));
                    asm_inst_blk_append(self->alloc, &self->inst_blk, asm_inst_mov_new(self->alloc, asm_operand_reg_new(self->alloc, ASM_Reg_DX), dest));
                }
                break;
            case IR_Type_FuncProlog:
                break;
            case IR_Type_FuncEpilog:
                break;
            case IR_Type_Func:
                asm_inst_blk_set_func(&self->inst_blk, IR_FUNC(ir_blk->inst[i])->identifier.length, IR_FUNC(ir_blk->inst[i])->identifier.c_str);
                break;
        }
    }
}


ASM asm_win_x86_64(Alloc * alloc, IR_Inst_Blk * ir_blk) {
    ASM assembly = {
        .alloc = alloc 
        , .emit = asm_win_x86_64_emit
    };

    emit_to_asm_win_x86_64(&assembly, ir_blk);

    size_t stack_size = asm_pseudo_pass(&assembly);
    asm_fix_assembly(&assembly, stack_size);

    return assembly;
}






