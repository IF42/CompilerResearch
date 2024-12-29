#ifndef _ASM_X86_64_H_
#define _ASM_X86_64_H_

#include <stddef.h>
#include <alloc/alloc.h>


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


ASM_Operand * asm_operand_imm_new(Alloc * alloc, size_t length, char * c_str);


typedef enum {
    ASM_Reg_AX
    , ASM_Reg_CX
    , ASM_Reg_DX
    , ASM_Reg_R10
    , ASM_Reg_R11
} ASM_Reg;


typedef struct {
    ASM_Operand type;
    ASM_Reg reg_id;
} ASM_Operand_Reg;


#define ASM_OPERAND_REG(T) ((ASM_Operand_Reg*) (T))


ASM_Operand * asm_operand_reg_new(Alloc * alloc, ASM_Reg reg_id);


typedef struct {
    ASM_Operand type;
    struct {
        size_t length;
        char * c_str;
    } identifier;
} ASM_Operand_PseudoVar;


#define ASM_OPERAND_PSEUDO_VAR(T) ((ASM_Operand_PseudoVar*) (T))


ASM_Operand * asm_operand_pseudo_var_new(Alloc * alloc, size_t length, char * c_str);


typedef struct {
    ASM_Operand type;
    size_t id;
} ASM_Operand_PseudoID;


#define ASM_OPERAND_PSEUDO_ID(T) ((ASM_Operand_PseudoID*) (T))


ASM_Operand * asm_operand_pseudo_id_new(Alloc * alloc, size_t id);


typedef struct {
    ASM_Operand type;
    size_t size;
} ASM_Operand_Stack;


#define ASM_OPERAND_STACK(T) ((ASM_Operand_Stack*) (T))


ASM_Operand * asm_operand_stack_new(Alloc * alloc, size_t size);


typedef enum {
    ASM_Inst_Type_Ret
    , ASM_Inst_Type_Mov
    , ASM_Inst_Type_Neg
    , ASM_Inst_Type_Not
    , ASM_Inst_Type_Add
    , ASM_Inst_Type_Subtract
    , ASM_Inst_Type_Multiply
    , ASM_Inst_Type_IDiv
    , ASM_Inst_Type_Cdq
    , ASM_Inst_Type_AllocStack
} ASM_Inst;


typedef struct {
    ASM_Inst type;
} ASM_Inst_Ret;


ASM_Inst * asm_inst_ret_new(Alloc * alloc);


typedef struct {
    ASM_Inst type;
    ASM_Operand * src;
    ASM_Operand * dest;
} ASM_Inst_Mov;


#define ASM_INST_MOV(T) ((ASM_Inst_Mov*)(T)) 


ASM_Inst * asm_inst_mov_new(Alloc * alloc, ASM_Operand * src, ASM_Operand * dest);


typedef struct {
    ASM_Inst type;
    ASM_Operand * operand;
} ASM_Inst_Unary;


#define ASM_INST_UNARY(T) ((ASM_Inst_Unary*) (T))


ASM_Inst * asm_inst_neg_new(Alloc * alloc, ASM_Operand * operand);


ASM_Inst * asm_inst_not_new(Alloc * alloc, ASM_Operand * operand);


ASM_Inst * asm_inst_idiv_new(Alloc * alloc, ASM_Operand * operand);


typedef struct {
    ASM_Inst type;
    ASM_Operand * src;
    ASM_Operand * dest;
} ASM_Inst_Binary;



#define ASM_INST_BINARY(T) ((ASM_Inst_Binary*)(T))


ASM_Inst * asm_inst_add_new(Alloc * alloc, ASM_Operand * src, ASM_Operand * dest);


ASM_Inst * asm_inst_subtract_new(Alloc * alloc, ASM_Operand * src, ASM_Operand * dest);


ASM_Inst * asm_inst_multiply_new(Alloc * alloc, ASM_Operand * src, ASM_Operand * dest);


typedef struct {
    ASM_Inst type;
    size_t size;
} ASM_Inst_AllocStack;


#define ASM_INST_ALLOC_STACK(T) ((ASM_Inst_AllocStack*) (T))


ASM_Inst * asm_inst_alloc_stack_new(Alloc * alloc, size_t size);


typedef struct {
    ASM_Inst type;
} ASM_Inst_Cdq;


ASM_Inst * asm_inst_cdq_new(Alloc * alloc);


typedef struct ASM_Inst_Blk {
    struct {
        size_t length;
        char * c_str;
    } identifier;

    size_t capacity;
    size_t size;
    ASM_Inst ** inst;
} ASM_Inst_Blk;


void asm_inst_blk_set_func(ASM_Inst_Blk * self, size_t length, char * c_str);


void asm_inst_blk_append(Alloc * alloc, ASM_Inst_Blk * self, ASM_Inst * inst);


void asm_inst_blk_insert(Alloc * alloc, ASM_Inst_Blk * self, size_t index, ASM_Inst * inst);


#endif




