#ifndef _ASM_X86_64_H_
#define _ASM_X86_64_H_

#include <stddef.h>


typedef enum {
    ASM_Operand_Type_Imm
    , ASM_Operand_Type_Reg
    , ASM_Operand_Type_PseudoVar
    , ASM_Operand_Type_PseudoID
    , ASM_Operand_Type_Stack
} ASM_Operand;


typedef struct {
    ASM_Operand type;
    struct {
        size_t length;
        char * c_str;
    } value;
} ASM_Operand_Imm;


#define ASM_OPERAND_IMM(T) ((ASM_Operand_Imm*) (T))


ASM_Operand * asm_operand_imm_new(size_t length, char * c_str);


typedef enum {
    ASM_Reg_AX
    , ASM_Reg_CX
    , ASM_Reg_R10
} ASM_Reg;


typedef struct {
    ASM_Operand type;
    ASM_Reg reg_id;
} ASM_Operand_Reg;


#define ASM_OPERAND_REG(T) ((ASM_Operand_Reg*) (T))


ASM_Operand * asm_operand_reg_new(ASM_Reg reg_id);


typedef struct {
    ASM_Operand type;
    struct {
        size_t length;
        char * c_str;
    } identifier;
} ASM_Operand_PseudoVar;


#define ASM_OPERAND_PSEUDO_VAR(T) ((ASM_Operand_PseudoVar*) (T))


ASM_Operand * asm_operand_pseudo_var_new(size_t length, char * c_str);


typedef struct {
    ASM_Operand type;
    size_t id;
} ASM_Operand_PseudoID;


#define ASM_OPERAND_PSEUDO_ID(T) ((ASM_Operand_PseudoID*) (T))


ASM_Operand * asm_operand_pseudo_id_new(size_t id);


typedef struct {
    ASM_Operand type;
    size_t size;
} ASM_Operand_Stack;


#define ASM_OPERAND_STACK(T) ((ASM_Operand_Stack*) (T))


ASM_Operand * asm_operand_stack_new(size_t size);


typedef enum {
    ASM_Type_Inst_Ret
    , ASM_Type_Inst_Mov
    , ASM_Type_Inst_Neg
    , ASM_Type_Inst_Not
    , ASM_Type_Inst_AllocStack
} ASM;


typedef struct {
    ASM type;
} ASM_Inst_Ret;


ASM * asm_inst_ret_new(void);


typedef struct {
    ASM type;
    ASM_Operand * src;
    ASM_Operand * dest;
} ASM_Inst_Mov;


#define ASM_INST_MOV(T) ((ASM_Inst_Mov*)(T)) 


ASM * asm_inst_mov_new(ASM_Operand * src, ASM_Operand * dest);


typedef struct {
    ASM type;
    ASM_Operand * operand;
} ASM_Inst_Neg, ASM_Inst_Not;


#define ASM_INST_NEG(T) ((ASM_Inst_Neg*) (T))
#define ASM_INST_NOT(T) ((ASM_Inst_Not*) (T))


ASM * asm_inst_neg_new(ASM_Operand * operand);


ASM * asm_inst_not_new(ASM_Operand * operand);


typedef struct {
    ASM type;
    size_t size;
} ASM_Inst_AllocStack;


#define ASM_INST_ALLOC_STACK(T) ((ASM_Inst_AllocStack*) (T))


ASM * asm_inst_alloc_stack_new(size_t size);


typedef struct ASM_Inst_Blk {
    struct {
        size_t length;
        char * c_str;
    } identifier;

    //ASM_Operand ** context;

    size_t capacity;
    size_t size;
    ASM ** inst;
} ASM_Inst_Blk;


void asm_inst_blk_set_func(ASM_Inst_Blk * self, size_t length, char * c_str);


void asm_inst_blk_append(ASM_Inst_Blk * self, ASM * inst);


void asm_inst_blk_insert(ASM_Inst_Blk * self, size_t index, ASM * inst);


#include <stdio.h>


void asm_x86_64_emit(ASM_Inst_Blk * assembly, FILE * stream);


#endif




