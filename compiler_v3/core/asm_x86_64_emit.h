#ifndef _ASM_X86_64_EMIT_H_
#define _ASM_X86_64_EMIT_H_


#include "ir.h"
#include "asm_x86_64.h"


ASM_Inst_Blk ir_emit_to_asm_linux_x86_64(IR_Inst_Blk * ir);


ASM_Inst_Blk ir_emit_to_asm_win_x86_64(IR_Inst_Blk * ir);


size_t asm_pseudo_pass(ASM_Inst_Blk * assembly);


void asm_fix_assembly(ASM_Inst_Blk * assembly, size_t stack_size);

#endif


