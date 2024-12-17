#ifndef _ASM_H_
#define _ASM_H_

#include <stddef.h>
#include <stdio.h>


typedef enum {
    Operand_Type_Imm
    , Operand_Type_Reg
} ASM_Operand_Type;

typedef struct {
    size_t length;
    char * c_str;
}Imm;



typedef struct {
    ASM_Operand_Type type;

    union {
        Imm imm;
        char * reg;
    };    
} ASM_Operand;

#define asm_operand_reg(REG) (ASM_Operand) {.type = Operand_Type_Reg, .reg = (REG)}
#define asm_operand_imm(LEN, CSTR) (ASM_Operand) {.type = Operand_Type_Imm, .imm = (Imm) {.length=(LEN), .c_str=(CSTR)}}


typedef struct {
    ASM_Operand src;
    ASM_Operand dest;
} ASM_Instruction_Mov;


typedef enum {
    Instruction_Type_Ret
    , Instruction_Type_Mov
} ASM_Instruction_Type;


typedef struct {
    ASM_Instruction_Type type;

    union {
        ASM_Instruction_Mov mov;
    };
} ASM_Instruction;


#define asm_instruction_mov(SRC, DEST) (ASM_Instruction) {.type=Instruction_Type_Mov, .mov={.src=(SRC), .dest=(DEST)}}
#define asm_instruction_ret()(ASM_Instruction) {.type=Instruction_Type_Ret}


typedef struct {
    size_t capacity;
    size_t size;
    ASM_Instruction * instruction;
} ASM_Inst_Seq;


typedef struct {
    struct {
        size_t length;
        char * c_str;
    } identifier;

    ASM_Inst_Seq inst_seq;
} ASM_Function;


#define asm_function(L, CSTR, T) (ASM_Function) {.identifier = {.length=(L), .c_str = (CSTR)}, .inst_seq = (T)}


void asm_inst_seq_append_instruction(ASM_Inst_Seq * self, ASM_Instruction instruction);


typedef struct {
    ASM_Function function;
} ASM_Prog;


void asm_emit(ASM_Prog * self, FILE * stream);


void asm_finalize(ASM_Prog * self);


#endif


