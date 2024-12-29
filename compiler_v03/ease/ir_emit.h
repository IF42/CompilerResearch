#ifndef _IR_EMIT_H_
#define _IR_EMIT_H_


#include "ast.h"

#include <ir.h>
#include <alloc/alloc.h>


IR_Inst_Blk ir_emit(Alloc * alloc, AST * ast);


#endif

