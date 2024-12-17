#include "ir.h"

#include <stdlib.h>


void function_push_instruction (Function * self, Instruction instruction) {
    if(self->size >= self->capacity) {
        self->capacity = (self->capacity + 1) * 2;
        self->instruction = realloc(self->instruction, sizeof(Instruction) * self->capacity);
    }

    self->instruction[self->size++] = instruction;
}


void function_finalize(Function * self) {
    if(self->instruction != NULL) {
        free(self->instruction);
    }
}


void function_emit(Function * self, FILE * f) {
    fprintf(f, "\nfunction %s {\n", self->identifier);
    
    for(size_t i = 0; i < self->size; i ++) {
        switch(self->instruction[i].opcode) {
            case Opcode_Return:
                fprintf(f, "\treturn %s\n", self->instruction[i].ret.value.c_str);
                break;
            case Opcode_Unary:
                if(self->instruction[i].unary.type == Unary_Op_Compl) {
                    fprintf(f, "\t%s = ~%s\n", self->instruction[i].unary.dest.c_str, self->instruction[i].unary.src.c_str);
                } else if(self->instruction[i].unary.type == Unary_Op_Neg) {
                    fprintf(f, "\t%s = -%s\n", self->instruction[i].unary.dest.c_str, self->instruction[i].unary.src.c_str);
                }
                break;
        }
    }

    fprintf(f, "}\n");
}

size_t function_count_tmp_var(Function * self) {
    size_t count = 0;

    for(size_t i = 0; i < self->size; i ++) {
        if(self->instruction[i].opcode == Opcode_Unary) {
            count ++;
        }
    }

    return count;
}


void function_emit_asm(Function * self, FILE * f) {
    size_t tmp_var_count = function_count_tmp_var(self);
    size_t index = 0;
    fprintf(f, "%s:\n", self->identifier);
    
    //prolog
    fprintf(f, "\tpush rbp\n");
    fprintf(f, "\tmov rbp, rsp\n");
    fprintf(f, "\tsub rsp, %zu\n\n", tmp_var_count*8);

    for(size_t i = 0; i < self->size; i++) {
        switch(self->instruction[i].opcode) {
            case Opcode_Return:
                //epilog
                fprintf(f, "\tmov rsp, rbp\n");
                fprintf(f, "\tpop rbp\n");
                
                if(self->instruction[i].ret.value.type == Value_Const) {
                    fprintf(f, "\tmov rcx, %s\n", self->instruction[i].ret.value.c_str);
                } else {
                    fprintf(f, "\tmov rcx, [rbp-%zu]\n", index*8);
                }
                fprintf(f, "\tret\n\n");
                break;
            case Opcode_Unary:
                index ++;

                if(self->instruction[i].unary.src.type == Value_Const) {
                    fprintf(f, "\tmov rax, %s\n", self->instruction[i].unary.src.c_str);
                    fprintf(f, "\tmov [rbp-%zu], rax\n", index*8);
                }

                if(self->instruction[i].unary.type == Unary_Op_Compl) {
                    fprintf(f, "\tnot qword [rbp-%zu]\n\n", index*8);
                } else if(self->instruction[i].unary.type == Unary_Op_Neg) {
                    fprintf(f, "\tneg qword [rbp-%zu]\n\n",  index*8);
                }
                break;
        }
    }
}


void program_push_function(Program * self, Function function) {
    if(self->size >= self->capacity) {
        self->capacity = (self->capacity + 1) * 2;
        self->function = realloc(self->function, sizeof(Function) * self->capacity);
    } 

    self->function[self->size ++] = function;
}


void program_finalize(Program * self) {
    if(self->function != NULL) {
        free(self->function);
    }
}


void program_emit(Program * self, FILE * f) {
    for(size_t i = 0; i < self->size; i++) {
        if(self->function[i].global == true) {
            fprintf(f, "global %s\n", self->function[i].identifier);
        }
    }

    for(size_t i = 0; i < self->size; i ++) {
        function_emit(&self->function[i], f);
    } 
}


void program_emit_asm(Program * self, FILE * f) {
    for(size_t i = 0; i < self->size; i++) {
        if(self->function[i].global == true) {
            fprintf(f, "global %s\n", self->function[i].identifier);
        }
    }

    fprintf(f, "section .text\n");
    for(size_t i = 0; i < self->size; i ++) {
        function_emit_asm(&self->function[i], f);
    } 
}
