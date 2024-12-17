#ifndef _EMIT_H_
#define _EMIT_H_

#include "ast.h"
#include "asm.h"


ASM_Prog emit_to_win_x86_64_asm(AST * ast);

#endif
