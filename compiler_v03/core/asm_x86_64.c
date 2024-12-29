#include "asm_x86_64.h"

#include <assert.h>


ASM_Operand * asm_operand_imm_new(Alloc * alloc, size_t length, char * c_str) {
    ASM_Operand_Imm * self = new(alloc, sizeof(ASM_Operand_Imm));

    *self = (ASM_Operand_Imm) {
        .type = ASM_Operand_Type_Imm
        , .value = {.length = length, .c_str = c_str}
    };

    return &self->type;
} 


ASM_Operand * asm_operand_reg_new(Alloc * alloc, ASM_Reg reg_id) {
    ASM_Operand_Reg * self = new(alloc, sizeof(ASM_Operand_Reg));
    
    *self = (ASM_Operand_Reg) {
        .type = ASM_Operand_Type_Reg
        , .reg_id = reg_id
    };

    return &self->type;
}


ASM_Operand * asm_operand_pseudo_var_new(Alloc * alloc, size_t length, char * c_str) {
    ASM_Operand_PseudoVar * self = new(alloc, sizeof(ASM_Operand_PseudoVar));

    *self = (ASM_Operand_PseudoVar) {
        .type = ASM_Operand_Type_PseudoVar
        , .identifier = {.length = length, .c_str = c_str}
    };

    return &self->type;
}


ASM_Operand * asm_operand_pseudo_id_new(Alloc * alloc, size_t id) {
    ASM_Operand_PseudoID * self = new(alloc, sizeof(ASM_Operand_PseudoID));

    *self = (ASM_Operand_PseudoID) {
        .type = ASM_Operand_Type_PseudoID
        , .id = id
    };

    return &self->type;
}


ASM_Operand * asm_operand_stack_new(Alloc * alloc, size_t size) {
    ASM_Operand_Stack * self = new(alloc, sizeof(ASM_Operand_Stack));

    *self = (ASM_Operand_Stack) {
        .type = ASM_Operand_Type_Stack
        , .size = size
    };

    return &self->type;
}


static ASM_Inst * asm_inst_new(Alloc * alloc, ASM_Inst type) {
    ASM_Inst * self = new(alloc, sizeof(ASM_Inst));
    *self = type;
    return self;
}


ASM_Inst * asm_inst_ret_new(Alloc * alloc) {
    return asm_inst_new(alloc, ASM_Inst_Type_Ret);
}


ASM_Inst * asm_inst_cdq_new(Alloc * alloc) {
    return asm_inst_new(alloc, ASM_Inst_Type_Cdq);
}


ASM_Inst * asm_inst_alloc_stack_new(Alloc * alloc, size_t size) {
    ASM_Inst_AllocStack * self = new(alloc, sizeof(ASM_Inst_AllocStack));

    *self = (ASM_Inst_AllocStack) {
        .type = ASM_Inst_Type_AllocStack
        , .size = size
    };

    return &self->type;
}


ASM_Inst * asm_inst_mov_new(Alloc * alloc, ASM_Operand * src, ASM_Operand * dest) {
    ASM_Inst_Mov * self = new(alloc, sizeof(ASM_Inst_Mov));

    *self = (ASM_Inst_Mov) {
        .type = ASM_Inst_Type_Mov
        , .src = src
        , .dest = dest
    };

    return &self->type;
}


ASM_Inst * asm_inst_unary_new(Alloc * alloc, ASM_Inst type, ASM_Operand * operand) {
    ASM_Inst_Unary * self = new(alloc, sizeof(ASM_Inst_Unary));

    *self = (ASM_Inst_Unary) {
        .type = type
        , .operand = operand
    };

    return &self->type;
}


ASM_Inst * asm_inst_neg_new(Alloc * alloc, ASM_Operand * operand) {
    return asm_inst_unary_new(alloc, ASM_Inst_Type_Neg, operand);
}


ASM_Inst * asm_inst_not_new(Alloc * alloc, ASM_Operand * operand) {
    return asm_inst_unary_new(alloc, ASM_Inst_Type_Not, operand);
}


ASM_Inst * asm_inst_idiv_new(Alloc * alloc, ASM_Operand * operand) {
    return asm_inst_unary_new(alloc, ASM_Inst_Type_IDiv, operand);
}


static ASM_Inst * asm_inst_binary_new(Alloc * alloc, ASM_Inst type, ASM_Operand * src, ASM_Operand * dest) {
    ASM_Inst_Binary * self = new(alloc, sizeof(ASM_Inst_Binary));

    *self = (ASM_Inst_Binary) {
        .type = type
        , .src = src
        , .dest = dest
    };

    return &self->type;
}


ASM_Inst * asm_inst_add_new(Alloc * alloc, ASM_Operand * src, ASM_Operand * dest) {
    return asm_inst_binary_new(alloc, ASM_Inst_Type_Add, src, dest);
}


ASM_Inst * asm_inst_subtract_new(Alloc * alloc, ASM_Operand * src, ASM_Operand * dest) {
    return asm_inst_binary_new(alloc, ASM_Inst_Type_Subtract, src, dest);
}


ASM_Inst * asm_inst_multiply_new(Alloc * alloc, ASM_Operand * src, ASM_Operand * dest) {
    return asm_inst_binary_new(alloc, ASM_Inst_Type_Multiply, src, dest);
}


void asm_inst_blk_set_func(ASM_Inst_Blk * self, size_t length, char * c_str) {
    self->identifier.length = length;
    self->identifier.c_str = c_str;
}


#include <string.h>


void asm_inst_blk_append(Alloc * alloc, ASM_Inst_Blk * self, ASM_Inst * inst) {
    if(self->size >= self->capacity) {
        self->capacity = (self->capacity + 1) * 2;
        ASM_Inst ** arr = new(alloc, sizeof(ASM_Inst*) * self->capacity);
        memcpy(arr, self->inst, sizeof(ASM_Inst*) * self->size);
        delete(alloc, self->inst);
        self->inst = arr;
    }

    self->inst[self->size++] = inst;
}


void asm_inst_blk_insert(Alloc * alloc, ASM_Inst_Blk * self, size_t index, ASM_Inst * inst) {
    assert(index <= self->size);

    if(self->size >= self->capacity) {
        self->capacity = (self->capacity + 1) * 2;
        ASM_Inst ** arr = new(alloc, sizeof(ASM_Inst*) * self->capacity);
        memcpy(arr, self->inst, sizeof(ASM_Inst*) * self->size);
        delete(alloc, self->inst);
        self->inst = arr;
    }
    
    memcpy(&self->inst[index + 1], &self->inst[index], sizeof(ASM_Inst*) * (self->size - index));
    self->inst[index] = inst;
    self->size++;
}

