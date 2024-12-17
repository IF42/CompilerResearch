#include <stdio.h>
#include <stdlib.h>


/*
 * je třeb vytvořit správnou strukturu pro reprezentování IR, který je 
 * možné jednoduše rekurzivně skládat při syntaktické analýze a zároveň je možné 
 * ji jednoduše procházet pro optimalizaci
 */


typedef struct {
    char ** local_symbol_list;
    Instruction * instruction;
}Block;


typedef enum {
    Opcode_Unary
    , Opcode_Return
} Opcode;


typedef struct Instruction {
    Opcode opcode;
    Instruction * tail;
} Instruction;


typedef struct {
    Instruction instruction
    Value value;
} Instruction_Return;


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
} Op_Unary_Type;


typedef struct {
    Instruction instruction
    Unary_Op_Type type;
    Value src;
    Value dest;
} Instruction_Op_Unary;








int main(void) {
    printf("Program exit..\n");
    return EXIT_SUCCESS;
}

