#include "asm_x86_64_emit.h"


void asm_x86_64_emit(ASM * self, FILE * stream) {
    self->emit(self, stream);
}


static void asm_emit_operand(ASM_Operand * op, FILE * stream) {
    switch(*op) {
        case ASM_Operand_Type_Imm:
            fprintf(stream, "%.*s", (int) ASM_OPERAND_IMM(op)->value.length, ASM_OPERAND_IMM(op)->value.c_str);
            break;
        case ASM_Operand_Type_Reg:
            switch(ASM_OPERAND_REG(op)->reg_id) {
                case ASM_Reg_AX:
                    fprintf(stream, "rax");
                    break;
                case ASM_Reg_CX:
                    fprintf(stream, "rcx");
                    break;
                case ASM_Reg_DX:
                    fprintf(stream, "rdx");
                    break;
                case ASM_Reg_R10:
                    fprintf(stream, "r10");
                    break;
                case ASM_Reg_R11:
                    fprintf(stream, "r11");
                    break;
            }
            break;
        case ASM_Operand_Type_Stack:
            fprintf(stream, "qword [rbp - %zu]", ASM_OPERAND_STACK(op)->size);
            break;
        case ASM_Operand_Type_PseudoVar:
            fprintf(stream, "%%%.*s", (int)ASM_OPERAND_PSEUDO_VAR(op)->identifier.length, ASM_OPERAND_PSEUDO_VAR(op)->identifier.c_str);
            break;
        case ASM_Operand_Type_PseudoID:
            fprintf(stream, "%%%zu", ASM_OPERAND_PSEUDO_ID(op)->id);
            break;
        default:
            break;
    }
}


ASM_Operand * ir_value_to_asm_operand(Alloc * alloc, IR_Value * value) {
    if(value != NULL) {
        switch(*value) {
            case IR_Value_Type_Const:
                return asm_operand_imm_new(alloc, IR_VALUE_CONST(value)->value.length, IR_VALUE_CONST(value)->value.c_str);
            case IR_Value_Type_Var:
                return asm_operand_pseudo_var_new(alloc, IR_VALUE_VAR(value)->identifier.length, IR_VALUE_VAR(value)->identifier.c_str);
            case IR_Value_Type_ID:
                return asm_operand_pseudo_id_new(alloc, IR_VALUE_ID(value)->id);
        }
    }

    return NULL;
}


void asm_x86_64_inst_emit(ASM * self, FILE * stream) {
    if(self->inst_blk.identifier.length > 0) {
        fprintf(stream, "\n%.*s:\n", (int) self->inst_blk.identifier.length, self->inst_blk.identifier.c_str);
    }

    for(size_t i = 0; i < self->inst_blk.size; i++) {
        switch(*self->inst_blk.inst[i]) {
            case ASM_Inst_Type_Ret:
                fprintf(stream, "\tmov rsp, rbp\n");
                fprintf(stream, "\tpop rbp\n");
                fprintf(stream, "\tret\n");
                break;
            case ASM_Inst_Type_Mov:
                fprintf(stream, "\tmov ");
                asm_emit_operand(ASM_INST_MOV(self->inst_blk.inst[i])->dest, stream);
                fprintf(stream, ", ");
                asm_emit_operand(ASM_INST_MOV(self->inst_blk.inst[i])->src, stream);
                fprintf(stream, "\n");
                break;
            case ASM_Inst_Type_Neg:
                fprintf(stream, "\tneg ");
                asm_emit_operand(ASM_INST_UNARY(self->inst_blk.inst[i])->operand, stream);
                fprintf(stream, "\n");
                break;
            case ASM_Inst_Type_Not:
                fprintf(stream, "\tnot ");
                asm_emit_operand(ASM_INST_UNARY(self->inst_blk.inst[i])->operand, stream);
                fprintf(stream, "\n");
                break;
            case ASM_Inst_Type_Add:
                fprintf(stream, "\tadd ");
                asm_emit_operand(ASM_INST_BINARY(self->inst_blk.inst[i])->dest, stream);
                fprintf(stream, ", ");
                asm_emit_operand(ASM_INST_BINARY(self->inst_blk.inst[i])->src, stream);
                fprintf(stream, "\n");
                break;
            case ASM_Inst_Type_Subtract:
                fprintf(stream, "\tsub ");
                asm_emit_operand(ASM_INST_BINARY(self->inst_blk.inst[i])->dest, stream);
                fprintf(stream, ", ");
                asm_emit_operand(ASM_INST_BINARY(self->inst_blk.inst[i])->src, stream);
                fprintf(stream, "\n");
                break;
            case ASM_Inst_Type_Multiply:
                fprintf(stream, "\timul ");
                asm_emit_operand(ASM_INST_BINARY(self->inst_blk.inst[i])->dest, stream);
                fprintf(stream, ", ");
                asm_emit_operand(ASM_INST_BINARY(self->inst_blk.inst[i])->src, stream);
                fprintf(stream, "\n");
                break;
            case ASM_Inst_Type_IDiv:
                fprintf(stream, "\tidiv ");
                asm_emit_operand(ASM_INST_UNARY(self->inst_blk.inst[i])->operand, stream);
                fprintf(stream, "\n");
                break;
            case ASM_Inst_Type_Cdq:
                fprintf(stream, "\tcdq\n");
                break;
            case ASM_Inst_Type_AllocStack:
                fprintf(stream, "\tpush rsp\n");
                fprintf(stream, "\tmov rbp, rsp\n");
                
                if(ASM_INST_ALLOC_STACK(self->inst_blk.inst[i])->size > 0) {
                    fprintf(stream, "\tsub rsp, %zu\n", ASM_INST_ALLOC_STACK(self->inst_blk.inst[i])->size);
                }
                break;
        }
    }
}


size_t asm_pseudo_pass(ASM * self) {
    size_t stack_size = 0;
    /* 
     * Zde se nahradí pseudo-registry adresamy lokálních proměnných na zásobníku
     * TODO: improve pseudo-register map for handling string names
     */
    size_t pseudo_reg_map = 0;

    for(size_t i = 0; i < self->inst_blk.size; i++) {
        if(*self->inst_blk.inst[i] == ASM_Inst_Type_Mov) {
            if(*ASM_INST_MOV(self->inst_blk.inst[i])->src == ASM_Operand_Type_PseudoID) {
                ASM_Operand * src = asm_operand_stack_new(self->alloc, (ASM_OPERAND_PSEUDO_ID(ASM_INST_MOV(self->inst_blk.inst[i])->src)->id + 1) * 8);
                delete(self->alloc, ASM_INST_MOV(self->inst_blk.inst[i])->src);
                ASM_INST_MOV(self->inst_blk.inst[i])->src = src;
            }

            if(*ASM_INST_MOV(self->inst_blk.inst[i])->dest == ASM_Operand_Type_PseudoID) {
                ASM_Operand * dest = asm_operand_stack_new(self->alloc, (ASM_OPERAND_PSEUDO_ID(ASM_INST_MOV(self->inst_blk.inst[i])->dest)->id + 1) * 8);
                delete(self->alloc, ASM_INST_MOV(self->inst_blk.inst[i])->dest);
                ASM_INST_MOV(self->inst_blk.inst[i])->dest = dest;
            }
        } else if(*self->inst_blk.inst[i] == ASM_Inst_Type_Neg || *self->inst_blk.inst[i] == ASM_Inst_Type_Not 
                || *self->inst_blk.inst[i] == ASM_Inst_Type_IDiv) {
            if(*ASM_INST_UNARY(self->inst_blk.inst[i])->operand == ASM_Operand_Type_PseudoID) {
                ASM_Operand * operand = asm_operand_stack_new(self->alloc, (ASM_OPERAND_PSEUDO_ID(ASM_INST_UNARY(self->inst_blk.inst[i])->operand)->id + 1) * 8);
                delete(self->alloc, ASM_INST_UNARY(self->inst_blk.inst[i])->operand);
                ASM_INST_UNARY(self->inst_blk.inst[i])->operand = operand;
                stack_size += 8;
            }
        } else if(*self->inst_blk.inst[i] == ASM_Inst_Type_Add || *self->inst_blk.inst[i] == ASM_Inst_Type_Subtract 
                || *self->inst_blk.inst[i] == ASM_Inst_Type_Multiply) {
            if(*ASM_INST_BINARY(self->inst_blk.inst[i])->src == ASM_Operand_Type_PseudoID) {
                ASM_Operand * src = asm_operand_stack_new(self->alloc, (ASM_OPERAND_PSEUDO_ID(ASM_INST_BINARY(self->inst_blk.inst[i])->src)->id + 1) * 8);
                delete(self->alloc, ASM_INST_BINARY(self->inst_blk.inst[i])->src);
                ASM_INST_BINARY(self->inst_blk.inst[i])->src = src;
            }

            if(*ASM_INST_BINARY(self->inst_blk.inst[i])->dest == ASM_Operand_Type_PseudoID) {
                ASM_Operand * dest = asm_operand_stack_new(self->alloc, (ASM_OPERAND_PSEUDO_ID(ASM_INST_BINARY(self->inst_blk.inst[i])->dest)->id + 1) * 8);
                delete(self->alloc, ASM_INST_BINARY(self->inst_blk.inst[i])->dest);
                ASM_INST_BINARY(self->inst_blk.inst[i])->dest = dest;
                stack_size += 8;
            }
        }
    }

    return stack_size;
}


void asm_fix_assembly(ASM * self, size_t stack_size) {
    asm_inst_blk_insert(self->alloc, &self->inst_blk, 0, asm_inst_alloc_stack_new(self->alloc, stack_size));

    for(size_t i = 0; i < self->inst_blk.size; i++) {
        if(*self->inst_blk.inst[i] == ASM_Inst_Type_Mov) {
            /*
             * u instrukce mov se nesmí v obou operandech nacházet adresa do pamět a zároveň nelze přímo vložit konstantu do paměti, 
             * ale musí se vložit nejprve do registru a obsah registru teprve do paměti zásobníku
             */
            if((*ASM_INST_MOV(self->inst_blk.inst[i])->src == ASM_Operand_Type_Stack) 
                    && (*ASM_INST_MOV(self->inst_blk.inst[i])->dest == ASM_Operand_Type_Stack)) {
                ASM_Inst * mov = asm_inst_mov_new(self->alloc, ASM_INST_MOV(self->inst_blk.inst[i])->src, asm_operand_reg_new(self->alloc, ASM_Reg_R10));
                ASM_INST_MOV(self->inst_blk.inst[i])->src = asm_operand_reg_new(self->alloc, ASM_Reg_R10);
                asm_inst_blk_insert(self->alloc, &self->inst_blk, i, mov);
            } else if((*ASM_INST_MOV(self->inst_blk.inst[i])->src == ASM_Operand_Type_Imm) 
                    && (*ASM_INST_MOV(self->inst_blk.inst[i])->dest == ASM_Operand_Type_Stack)) {
                ASM_Inst * mov = asm_inst_mov_new(self->alloc, ASM_INST_MOV(self->inst_blk.inst[i])->src, asm_operand_reg_new(self->alloc, ASM_Reg_R10));
                ASM_INST_MOV(self->inst_blk.inst[i])->src = asm_operand_reg_new(self->alloc, ASM_Reg_R10);
                asm_inst_blk_insert(self->alloc, &self->inst_blk, i, mov);
            }
        } else if(*self->inst_blk.inst[i] == ASM_Inst_Type_Add || *self->inst_blk.inst[i] == ASM_Inst_Type_Subtract) {
            /*
             * u instrukcí binárních operací se nesmí v obou operandech nacházet adresa do pamět a zároveň nelze přímo vložit konstantu do paměti, 
             * ale musí se vložit nejprve do registru a obsah registru teprve do paměti zásobníku
             */
            if((*ASM_INST_BINARY(self->inst_blk.inst[i])->src == ASM_Operand_Type_Stack) 
                    && (*ASM_INST_BINARY(self->inst_blk.inst[i])->dest == ASM_Operand_Type_Stack)) {
                ASM_Inst * mov = asm_inst_mov_new(self->alloc, ASM_INST_BINARY(self->inst_blk.inst[i])->src, asm_operand_reg_new(self->alloc, ASM_Reg_R10));
                ASM_INST_BINARY(self->inst_blk.inst[i])->src = asm_operand_reg_new(self->alloc, ASM_Reg_R10);
                asm_inst_blk_insert(self->alloc, &self->inst_blk, i, mov);
            } else if((*ASM_INST_BINARY(self->inst_blk.inst[i])->src == ASM_Operand_Type_Imm) 
                    && (*ASM_INST_BINARY(self->inst_blk.inst[i])->dest == ASM_Operand_Type_Stack)) {
                ASM_Inst * mov = asm_inst_mov_new(self->alloc, ASM_INST_BINARY(self->inst_blk.inst[i])->src, asm_operand_reg_new(self->alloc, ASM_Reg_R10));
                ASM_INST_BINARY(self->inst_blk.inst[i])->src = asm_operand_reg_new(self->alloc, ASM_Reg_R10);
                asm_inst_blk_insert(self->alloc, &self->inst_blk, i, mov);
            }
        } else if(*self->inst_blk.inst[i] == ASM_Inst_Type_Multiply) {
            if(*ASM_INST_BINARY(self->inst_blk.inst[i])->dest == ASM_Operand_Type_Stack) {
                /*
                 * v případě instrukce imul nesmí být na pozici dest adresa paměti, tedy lokální proměnná na zásobníku. 
                 * je třeba hodnotu ze zásobníku načíst do registru r11 provést násobení s druhým parametrem a 
                 * vložit výsledek do cílové adresy v paměti
                 */
                ASM_Operand * dest = ASM_INST_BINARY(self->inst_blk.inst[i])->dest;
                
                ASM_Inst * mov_in = asm_inst_mov_new(self->alloc, dest, asm_operand_reg_new(self->alloc, ASM_Reg_R11));
                
                ASM_INST_BINARY(self->inst_blk.inst[i])->dest = asm_operand_reg_new(self->alloc, ASM_Reg_R11);
                asm_inst_blk_insert(self->alloc, &self->inst_blk, i, mov_in);

                ASM_Inst * mov_back = asm_inst_mov_new(self->alloc, asm_operand_reg_new(self->alloc, ASM_Reg_R11), dest);
                asm_inst_blk_insert(self->alloc, &self->inst_blk, i+2, mov_back);
            }
        } else if(*self->inst_blk.inst[i] == ASM_Inst_Type_IDiv) {
            if(*ASM_INST_UNARY(self->inst_blk.inst[i])->operand == ASM_Operand_Type_Imm) {
                ASM_Inst * idiv_reg = asm_inst_mov_new(self->alloc, ASM_INST_UNARY(self->inst_blk.inst[i])->operand, asm_operand_reg_new(self->alloc, ASM_Reg_R10));
                ASM_INST_UNARY(self->inst_blk.inst[i])->operand = asm_operand_reg_new(self->alloc, ASM_Reg_R10);
                asm_inst_blk_insert(self->alloc, &self->inst_blk, i, idiv_reg);
            }
        }
    }
}


