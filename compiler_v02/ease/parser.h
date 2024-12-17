#ifndef _PARSER_H_
#define _PARSER_H_

#include "lexer.h"
#include "ast.h"


typedef struct {
    Lexer lexer;    
} Parser;


Parser parser_init(Lexer lexer);


AST * parse(Parser * self);




#endif

