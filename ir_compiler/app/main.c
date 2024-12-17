#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

/*
 * Strukturu pro reprezentaci IR je třeba navrhnout s ohledem na jdnoduchost optimalizace a 
 * následný překlad na ASM konkrétní platformy
 * Zároveň by měla být schopna zakrýt detaily nad hardwarem
 */

typedef enum {
    Value_Const
    , Value_Identifier
} Value_Type;


typedef struct {
    Value_Type type;
    char * c_str;
} Value;


#define value(T, V) (Value) {.type = (T), .c_str = (V)}


typedef enum {
    Unary_Op_Compl
    , Unary_Op_Neg
} Unary_Op_Type;


typedef struct {
    Unary_Op_Type type;
    Value src;
    Value dest;
} Unary_Op;


typedef enum {
    Opcode_Unary
    , Opcode_Return
} Opcode;


typedef struct {
    Value value;
} Return;


typedef struct {
    Opcode opcode;

    union {
        Unary_Op unary;
        Return ret;
    };
} Instruction;


#define inst_return(_value) (Instruction) {.opcode = Opcode_Return, .ret = (Return) {.value = (_value)}}
#define inst_unary_compl(_src, _dest) (Instruction) {.opcode = Opcode_Unary, .unary = (Unary_Op) {.type = Unary_Op_Compl, .src = (_src), .dest = (_dest)}}
#define inst_unary_neg(_src, _dest) (Instruction) {.opcode = Opcode_Unary, .unary = (Unary_Op) {.type = Unary_Op_Neg, .src = (_src), .dest = (_dest)}}


typedef struct {
    bool global;
    char * identifier;
    size_t capacity;
    size_t size;
    Instruction * instruction;
} Function;


#define function_init(_global, _identifier) (Function) {.global = (_global), .identifier = (_identifier)}


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


typedef struct {
    size_t capacity;
    size_t size;
    Function * function;
} Program;


#define program_init() (Program) {0}


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

    fprintf(f, "\nsection .text\n");
    for(size_t i = 0; i < self->size; i ++) {
        function_emit_asm(&self->function[i], f);
    } 
}


int main(void) {
    Program program = program_init();

    Function f_start = function_init(true, "_start");
    function_push_instruction(&f_start, inst_return(value(Value_Const, "0")));
    program_push_function(&program, f_start);

    Function function = function_init(false, "main");
    function_push_instruction(&function, inst_unary_neg(value(Value_Const, "2"), value(Value_Identifier, "t1")));
    function_push_instruction(&function, inst_unary_compl(value(Value_Identifier, "t1"), value(Value_Identifier, "t2")));
    function_push_instruction(&function, inst_return(value(Value_Identifier, "t2")));
    program_push_function(&program, function);

    program_emit(&program, stdout);
    program_finalize(&program);

    return EXIT_SUCCESS;
}



