#ifndef _PARSER_H_
#define _PARSER_H_

#include "lexer.h"
#include "ast.h"

#include <alloc/alloc.h>


typedef struct {
    Alloc * alloc;
    Lexer lexer;    
} Parser;


Parser parser_init(Alloc * alloc, Lexer lexer);


AST * parse(Parser * self);




#endif

