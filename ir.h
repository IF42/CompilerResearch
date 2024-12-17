#ifndef _IR_H_
#define _IR_H_

#include <stdio.h>
#include <stdbool.h>


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


#define inst_return(_value) \
    (Instruction) {.opcode = Opcode_Return, .ret = (Return) {.value = (_value)}}

#define inst_unary_compl(_src, _dest) \
    (Instruction) {.opcode = Opcode_Unary, .unary = (Unary_Op) {.type = Unary_Op_Compl, .src = (_src), .dest = (_dest)}}

#define inst_unary_neg(_src, _dest) \
    (Instruction) {.opcode = Opcode_Unary, .unary = (Unary_Op) {.type = Unary_Op_Neg, .src = (_src), .dest = (_dest)}}


typedef struct {
    bool global;
    char * identifier;
    size_t capacity;
    size_t size;
    Instruction * instruction;
} Function;


#define function_init(_global, _identifier) \
    (Function) {.global = (_global), .identifier = (_identifier)}


void function_push_instruction (Function * self, Instruction instruction);


void function_finalize(Function * self);


void function_emit(Function * self, FILE * f);


size_t function_count_tmp_var(Function * self);


void function_emit_asm(Function * self, FILE * f);


typedef struct {
    size_t capacity;
    size_t size;
    Function * function;
} Program;


#define program_init() (Program) {0}


void program_push_function(Program * self, Function function);


void program_finalize(Program * self);


void program_emit(Program * self, FILE * f);


void program_emit_asm(Program * self, FILE * f);


#endif
