#ifndef _ASM_X86_64_EMIT_H_
#define _ASM_X86_64_EMIT_H_


#include "ir.h"
#include "asm_x86_64.h"

#include <alloc/alloc.h>
#include <stdio.h>


typedef struct ASM {
    Alloc * alloc;
    //ASM_Operand ** context;
    ASM_Inst_Blk inst_blk;
    void (*emit)(struct ASM *, FILE *);
} ASM;


ASM_Operand * ir_value_to_asm_operand(Alloc * alloc, IR_Value * value);


void asm_x86_64_emit(ASM * self, FILE * stream);


void asm_x86_64_inst_emit(ASM * self, FILE * stream);


size_t asm_pseudo_pass(ASM * self);


void asm_fix_assembly(ASM * self, size_t stack_size);

#endif


