#include "asm.h"

#include <stdlib.h>


void asm_inst_seq_append_instruction(ASM_Inst_Seq * self, ASM_Instruction instruction) {
    if(self->size >= self->capacity) {
        self->capacity = (self->capacity + 1) * 2;
        self->instruction = realloc(self->instruction, sizeof(ASM_Instruction) * self->capacity);
    }

    self->instruction[self->size++] = instruction;
}


void asm_emit(ASM_Prog * self, FILE * stream) {
    fprintf(stream, "section .text\n");
    fprintf(stream, "extern %.*s\n", (int) self->function.identifier.length, self->function.identifier.c_str);
    fprintf(stream, "%.*s:\n", (int) self->function.identifier.length, self->function.identifier.c_str);

    for(size_t i = 0; i < self->function.inst_seq.size; i++) {
        switch(self->function.inst_seq.instruction[i].type) {
            case Instruction_Type_Mov:
                fprintf(stream, "\tmov ");

                switch(self->function.inst_seq.instruction[i].mov.src.type) {
                    case Operand_Type_Imm:
                        fprintf(stream, "%.*s", (int) self->function.inst_seq.instruction[i].mov.src.imm.length, self->function.inst_seq.instruction[i].mov.src.imm.c_str);
                        break;
                    case Operand_Type_Reg:
                        fprintf(stream, "%s", self->function.inst_seq.instruction[i].mov.src.reg);
                        break;
                }

                switch(self->function.inst_seq.instruction[i].mov.dest.type) {
                    case Operand_Type_Imm:
                        fprintf(stream, ", %.*s\n", (int) self->function.inst_seq.instruction[i].mov.dest.imm.length, self->function.inst_seq.instruction[i].mov.dest.imm.c_str);
                        break;
                    case Operand_Type_Reg:
                        fprintf(stream, ", %s\n", self->function.inst_seq.instruction[i].mov.dest.reg);
                        break;
                }

                break;
            case Instruction_Type_Ret:
                fprintf(stream, "\tret\n");
                break;
        }
    }
}


void asm_finalize(ASM_Prog * self) {
    if(self->function.inst_seq.instruction != NULL) {
        free(self->function.inst_seq.instruction); 
    }
}



