#include "ir.h"

#include <stdlib.h>


IR_Value * ir_value_const_new(size_t length, char * c_str) {
    IR_Value_Const * self = malloc(sizeof(IR_Value_Const));

    *self = (IR_Value_Const) {
        .type = IR_Value_Type_Const
        , .value = {
            .length = length
            , .c_str = c_str
        }
    };

    return &self->type;
}


IR_Value * ir_value_var_new(size_t length, char * c_str) {
    IR_Value_Var * self = malloc(sizeof(IR_Value_Var));

    *self = (IR_Value_Var) {
        .type = IR_Value_Type_Var
        , .identifier = {
            .length = length
            , .c_str = c_str
        }
    };

    return &self->type;
}


IR_Value * ir_value_var_id_new(size_t id) {
    IR_Value_ID * self = malloc(sizeof(IR_Value_ID));

    *self = (IR_Value_ID) {
        .type = IR_Value_Type_ID
        , .id = id
    };

    return &self->type;
}



static IR * ir_inst_unary_new(IR type, IR_Value * src, IR_Value * dest) {
    IR_Inst_Unary * self = malloc(sizeof(IR_Inst_Unary));

    *self = (IR_Inst_Unary) {
        .type = type
        , .src = src
        , .dest = dest
    };

    return &self->type;
}


IR * ir_inst_neg_new(IR_Value * dest, IR_Value * src) {
    return ir_inst_unary_new(IR_Type_Inst_Neg, src, dest);
}


IR * ir_inst_not_new(IR_Value * dest, IR_Value * src) {
    return ir_inst_unary_new(IR_Type_Inst_Not, src, dest);
}


IR * ir_inst_ret_new(IR_Value * value) {
    IR_Inst_Ret * self = malloc(sizeof(IR_Inst_Ret));

    *self = (IR_Inst_Ret) {
        .type = IR_Type_Inst_Ret
        , .value = value
    };

    return &self->type;
}


IR * ir_func_new(size_t length, char * c_str) {
    IR_Func * self = malloc(sizeof(IR_Func));

    *self = (IR_Func) {
        .type = IR_Type_Func
        , .identifier = {.length = length, .c_str = c_str}
    };

    return &self->type;
}


IR * ir_func_prolog_new(void) {
    IR_Func_Prolog * self = malloc(sizeof(IR_Func_Prolog));

    *self = (IR_Func_Prolog) {
        .type = IR_Type_FuncProlog
    };

    return &self->type;
}


IR * ir_func_epilog_new(void) {
    IR_Func_Epilog * self = malloc(sizeof(IR_Func_Epilog));

    *self = (IR_Func_Epilog) {
        .type = IR_Type_FuncEpilog
    };

    return &self->type;
}


void ir_inst_blk_append(IR_Inst_Blk * self, IR * inst) {
    if(self->size >= self->capacity) {
        self->capacity = (self->capacity + 1) * 2;
        self->inst = realloc(self->inst, sizeof(IR*) * self->capacity);
    }

    self->inst[self->size++] = inst;
}


static void ir_show_value(IR_Value * value, FILE * stream) {
    if(value != NULL) {
        switch(*value) {
            case IR_Value_Type_Const:
                fprintf(stream, "%.*s", (int) IR_VALUE_CONST(value)->value.length, IR_VALUE_CONST(value)->value.c_str);
                break;
            case IR_Value_Type_Var:
                fprintf(stream, "%%%.*s", (int) IR_VALUE_VAR(value)->identifier.length, IR_VALUE_VAR(value)->identifier.c_str);
                break;
            case IR_Value_Type_ID:
                fprintf(stream, "%%%zu", IR_VALUE_ID(value)->id);
                break;
        }
    }
}


static void ir_show_inst(IR * self, FILE * stream) {
    switch(*self) {
        case IR_Type_Inst_Neg:
            fprintf(stream, "\t");
            ir_show_value(IR_INST_UNARY(self)->dest, stream);
            fprintf(stream, " = -");
            ir_show_value(IR_INST_UNARY(self)->src, stream);
            fprintf(stream, "\n");
            break;
        case IR_Type_Inst_Not:
            fprintf(stream, "\t");
            ir_show_value(IR_INST_UNARY(self)->dest, stream);
            fprintf(stream, " = ~");
            ir_show_value(IR_INST_UNARY(self)->src, stream);
            fprintf(stream, "\n");
            break;
        case IR_Type_Inst_Ret:
            fprintf(stream, "\tret ");
            ir_show_value(IR_INST_RET(self)->value, stream);
            fprintf(stream, "\n");
            break;
        case IR_Type_Func:
            fprintf(stream, "define @%.*s ", (int) IR_FUNC(self)->identifier.length, IR_FUNC(self)->identifier.c_str);
            break;
        case IR_Type_FuncProlog:
            fprintf(stream, "{\n");
            break;
        case IR_Type_FuncEpilog:
            fprintf(stream, "}\n");
            break;
    } 
}


void ir_show(IR_Inst_Blk * self, FILE * stream) {
    for(size_t i = 0; i < self->size && self->inst[i] != NULL; i++) {
        ir_show_inst(self->inst[i], stream);
    }
}




