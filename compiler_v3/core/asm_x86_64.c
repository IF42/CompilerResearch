#include "asm_x86_64.h"

#include <stdlib.h>
#include <assert.h>


ASM_Operand * asm_operand_imm_new(size_t length, char * c_str) {
    ASM_Operand_Imm * self = malloc(sizeof(ASM_Operand_Imm));

    *self = (ASM_Operand_Imm) {
        .type = ASM_Operand_Type_Imm
        , .value = {.length = length, .c_str = c_str}
    };

    return &self->type;
} 


ASM_Operand * asm_operand_reg_new(ASM_Reg reg_id) {
    ASM_Operand_Reg * self = malloc(sizeof(ASM_Operand_Reg));
    
    *self = (ASM_Operand_Reg) {
        .type = ASM_Operand_Type_Reg
        , .reg_id = reg_id
    };

    return &self->type;
}


ASM_Operand * asm_operand_pseudo_var_new(size_t length, char * c_str) {
    ASM_Operand_PseudoVar * self = malloc(sizeof(ASM_Operand_PseudoVar));

    *self = (ASM_Operand_PseudoVar) {
        .type = ASM_Operand_Type_PseudoVar
        , .identifier = {.length = length, .c_str = c_str}
    };

    return &self->type;
}


ASM_Operand * asm_operand_pseudo_id_new(size_t id) {
    ASM_Operand_PseudoID * self = malloc(sizeof(ASM_Operand_PseudoID));

    *self = (ASM_Operand_PseudoID) {
        .type = ASM_Operand_Type_PseudoID
        , .id = id
    };

    return &self->type;
}


ASM_Operand * asm_operand_stack_new(size_t size) {
    ASM_Operand_Stack * self = malloc(sizeof(ASM_Operand_Stack));

    *self = (ASM_Operand_Stack) {
        .type = ASM_Operand_Type_Stack
        , .size = size
    };

    return &self->type;
}


ASM * asm_inst_ret_new(void) {
    ASM_Inst_Ret * self = malloc(sizeof(ASM_Inst_Ret));

    *self = (ASM_Inst_Ret) {
        .type = ASM_Type_Inst_Ret
    };

    return &self->type;
}


ASM * asm_inst_mov_new(ASM_Operand * src, ASM_Operand * dest) {
    ASM_Inst_Mov * self = malloc(sizeof(ASM_Inst_Mov));

    *self = (ASM_Inst_Mov) {
        .type = ASM_Type_Inst_Mov
        , .src = src
        , .dest = dest
    };

    return &self->type;
}


ASM * asm_inst_neg_new(ASM_Operand * operand) {
    ASM_Inst_Not * self = malloc(sizeof(ASM_Inst_Neg));

    *self = (ASM_Inst_Neg) {
        .type = ASM_Type_Inst_Neg
        , .operand = operand
    };

    return &self->type;
}


ASM * asm_inst_not_new(ASM_Operand * operand) {
    ASM_Inst_Not * self = malloc(sizeof(ASM_Inst_Not));

    *self = (ASM_Inst_Neg) {
        .type = ASM_Type_Inst_Not
        , .operand = operand
    };

    return &self->type;
}


ASM * asm_inst_alloc_stack_new(size_t size) {
    ASM_Inst_AllocStack * self = malloc(sizeof(ASM_Inst_AllocStack));

    *self = (ASM_Inst_AllocStack) {
        .type = ASM_Type_Inst_AllocStack
        , .size = size
    };

    return &self->type;
}


void asm_inst_blk_set_func(ASM_Inst_Blk * self, size_t length, char * c_str) {
    self->identifier.length = length;
    self->identifier.c_str = c_str;
}


void asm_inst_blk_append(ASM_Inst_Blk * self, ASM * inst) {
    if(self->size >= self->capacity) {
        self->capacity = (self->capacity + 1) * 2;
        self->inst = realloc(self->inst, sizeof(ASM*) * self->capacity);
    }

    self->inst[self->size++] = inst;
}


#include <string.h>


void asm_inst_blk_insert(ASM_Inst_Blk * self, size_t index, ASM * inst) {
    assert(index < self->size);

    if(self->size >= self->capacity) {
        self->capacity = (self->capacity + 1) * 2;
        self->inst = realloc(self->inst, sizeof(ASM*) * self->capacity);
    }

    memcpy(&self->inst[index + 1], &self->inst[index], sizeof(ASM*) * (self->size - index));
    self->inst[index] = inst;
    self->size++;
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
                case ASM_Reg_R10:
                    fprintf(stream, "r10");
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


void asm_x86_64_emit(ASM_Inst_Blk * assembly, FILE * stream) {
    fprintf(stream, "section .text\n");
    fprintf(stream, "extern ExitProcess\n");
    fprintf(stream, "extern _start\n");
    fprintf(stream, "_start:\n");
    fprintf(stream, "\tcall %.*s\n", (int) assembly->identifier.length, assembly->identifier.c_str);
    fprintf(stream, "\tmov rcx, rax\n");
    fprintf(stream, "\tcall ExitProcess\n");

    fprintf(stream, "\n%.*s:\n", (int) assembly->identifier.length, assembly->identifier.c_str);

    for(size_t i = 0; i < assembly->size; i++) {
        switch(*assembly->inst[i]) {
            case ASM_Type_Inst_Ret:
                fprintf(stream, "\tmov rsp, rbp\n");
                fprintf(stream, "\tpop rbp\n");
                fprintf(stream, "\tret\n");
                break;
            case ASM_Type_Inst_Mov:
                fprintf(stream, "\tmov ");
                asm_emit_operand(ASM_INST_MOV(assembly->inst[i])->dest, stream);
                fprintf(stream, ", ");
                asm_emit_operand(ASM_INST_MOV(assembly->inst[i])->src, stream);
                fprintf(stream, "\n");
                break;
            case ASM_Type_Inst_Neg:
                fprintf(stream, "\tneg ");
                asm_emit_operand(ASM_INST_NEG(assembly->inst[i])->operand, stream);
                fprintf(stream, "\n");
                break;
            case ASM_Type_Inst_Not:
                fprintf(stream, "\tnot ");
                asm_emit_operand(ASM_INST_NOT(assembly->inst[i])->operand, stream);
                fprintf(stream, "\n");
                break;
            case ASM_Type_Inst_AllocStack:
                fprintf(stream, "\tpush rsp\n");
                fprintf(stream, "\tmov rbp, rsp\n");
                
                if(ASM_INST_ALLOC_STACK(assembly->inst[i])->size > 0) {
                    fprintf(stream, "\tsub rsp, %zu\n", ASM_INST_ALLOC_STACK(assembly->inst[i])->size);
                }
                break;
        }
    }

}


