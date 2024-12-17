#ifndef _LEXER_H_
#define _LEXER_H_

#include <stdio.h>
#include <stddef.h>


typedef enum {
    Token_Type_Identifier

    , Token_Type_ConstNumber
    
    , Token_Type_SymbolOpenBrace
    , Token_Type_SymbolCloseBrace
    , Token_Type_SymbolOpenParenthesis
    , Token_Type_SymbolCloseParenthesis
    , Token_Type_SymbolColon
    , Token_Type_SymbolTilda
    , Token_Type_SymbolMinus

    , Token_Type_KeywordReturn
    , Token_Type_KeywordI8
    , Token_Type_KeywordI16
    , Token_Type_KeywordI32
    , Token_Type_KeywordI64
    , Token_Type_KeywordU8
    , Token_Type_KeywordU16
    , Token_Type_KeywordU32
    , Token_Type_KeywordU64
    , Token_Type_KeywordChar

    , Token_Type_Undefined
    , Token_Type_EOF
} Token_Type;


typedef struct {
    Token_Type type;
    size_t length;
    char * c_str;
} Token;


extern const char * token_type_str[];


typedef struct {
    size_t row;
    size_t column;
    size_t index;
} Lexer_State;


typedef struct {
    size_t capacity;
    size_t size;
    Token * arr;
} DynArr_Token;


typedef struct {
    FILE * f;
    char * filename;

    Lexer_State state;

    size_t size;
    char * buff;

    size_t iterator;
    DynArr_Token token;
} Lexer;


Lexer lexer_init(char * filename, FILE * f);


void lexer_finalize(Lexer * self);


Token lexer_next_token(Lexer * self);


size_t lexer_store(Lexer * self);


void lexer_restore(Lexer * self, size_t state);



#endif
