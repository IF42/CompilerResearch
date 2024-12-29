#ifndef _IR_H_
#define _IR_H_

#include <stddef.h>
#include <stdbool.h>
#include <alloc/alloc.h>

/*
 * PROGRAM ::= <INSTRUCTION>*
 * INSTRUCTION ::= <FUNCTION> | <PROLOG> | <EPILOG> | <NET> | <RET> | <RET>
 * FUNCTION ::= <NAME>
 *
 *
 *
 */


typedef enum {
    IR_Value_Type_Const
    , IR_Value_Type_Var
    , IR_Value_Type_ID
} IR_Value;


typedef struct {
    IR_Value type;
    struct {
        size_t length;
        char * c_str;
    } value;
} IR_Value_Const;


#define IR_VALUE_CONST(T) ((IR_Value_Const*) (T))


IR_Value * ir_value_const_new(Alloc * alloc, size_t length, char * c_str);


typedef struct {
    IR_Value type;

    struct {
        size_t length;
        char * c_str;
    } identifier;
} IR_Value_Var;


#define IR_VALUE_VAR(T) ((IR_Value_Var*) (T))


IR_Value * ir_value_var_new(Alloc * alloc, size_t length, char * c_str);


typedef struct {
    IR_Value type;
    size_t id;
} IR_Value_ID;


#define IR_VALUE_ID(T) ((IR_Value_ID*) (T))


IR_Value * ir_value_var_id_new(Alloc * alloc, size_t id);


typedef enum {
    IR_Type_Inst_Neg
    , IR_Type_Inst_Not
    , IR_Type_Inst_Ret
    , IR_Type_Inst_Add
    , IR_Type_Inst_Subtract
    , IR_Type_Inst_Multiply
    , IR_Type_Inst_Divide
    , IR_Type_Inst_Reminder
    , IR_Type_FuncProlog
    , IR_Type_FuncEpilog
    , IR_Type_Func
} IR;


typedef struct {
    IR type;
    IR_Value * src;
    IR_Value * dest;
} IR_Inst_Unary;


#define IR_INST_UNARY(T) ((IR_Inst_Unary*) (T))
#define IR_INST_NEG(T) ((IR_Inst_Unary*) (T))
#define IR_INST_NOT(T) ((IR_Inst_Unary*) (T))


IR * ir_inst_neg_new(Alloc * alloc, IR_Value * dest, IR_Value * src);


IR * ir_inst_not_new(Alloc * alloc, IR_Value * dest, IR_Value * src);


typedef struct {
    IR type;
    IR_Value * src1;
    IR_Value * src2;
    IR_Value * dest;
} IR_Inst_Binary;


#define IR_INST_BINARY(T) ((IR_Inst_Binary*) (T))
#define IR_INST_ADD(T) ((IR_Inst_Binary*) (T))
#define IR_INST_SUBTRACT(T) ((IR_Inst_Binary*) (T))
#define IR_INST_MULTIPLY(T) ((IR_Inst_Binary*) (T))
#define IR_INST_DIVIDE(T) ((IR_Inst_Binary*) (T))
#define IR_INST_REMINDER(T) ((IR_Inst_Binary*) (T))


IR * ir_inst_add_new(Alloc * alloc, IR_Value * dest, IR_Value * src1, IR_Value * src2);


IR * ir_inst_subtract_new(Alloc * alloc, IR_Value * dest, IR_Value * src1, IR_Value * src2);


IR * ir_inst_multiply_new(Alloc * alloc, IR_Value * dest, IR_Value * src1, IR_Value * src2);


IR * ir_inst_divide_new(Alloc * alloc, IR_Value * dest, IR_Value * src1, IR_Value * src2);


IR * ir_inst_reminder_new(Alloc * alloc, IR_Value * dest, IR_Value * src1, IR_Value * src2);


typedef struct {
    IR type;
    IR_Value * value;
} IR_Inst_Ret;


#define IR_INST_RET(T) ((IR_Inst_Ret*) (T))


IR * ir_inst_ret_new(Alloc * alloc, IR_Value * value);


typedef struct {
    IR type;

    struct {
        size_t length;
        char * c_str;
    } identifier;
} IR_Func;


#define IR_FUNC(T) ((IR_Func*) (T))


IR * ir_func_new(Alloc * alloc, size_t length, char * c_str);


typedef struct {
    IR type;
} IR_Func_Prolog;


#define IR_FUNC_PROLOG(T) ((IR_Func_Prolog*) (T))


IR * ir_func_prolog_new(Alloc * alloc);


typedef struct {
    IR type;
} IR_Func_Epilog;


#define IR_FUNC_EPILOG(T) ((IR_Func_Epilog*) (T))


IR * ir_func_epilog_new(Alloc * alloc);


typedef struct IR_Inst_Blk {
    size_t capacity;
    size_t size;
    IR ** inst;
} IR_Inst_Blk;


void ir_inst_blk_append(Alloc * alloc, IR_Inst_Blk * self, IR * inst);


#include <stdio.h>


void ir_show(IR_Inst_Blk * self, FILE * stream);


#endif


