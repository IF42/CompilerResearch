#include "parser.h"
#include "err.h"

#include <string.h>
#include <stdbool.h>


Parser parser_init(Lexer lexer) {
    return (Parser) {.lexer = lexer};
}


static bool parser_expect(Parser * self, Token_Type token_type) {
    size_t context = lexer_store(&self->lexer);
    Token token = lexer_next_token(&self->lexer);

    if(token.type == token_type) {
        return true;
    } else {
        lexer_restore(&self->lexer, context);
        return false;
    }
}


static AST * parse_stmt(Parser * self);
static AST * parse_expr(Parser * self);


static AST * parse_primitive_dtype(Parser * self) {
    if(parser_expect(self, Token_Type_KeywordI8) == true) {
        return ast_dtype_i8_new();
    } else if(parser_expect(self, Token_Type_KeywordI16) == true) {
        return ast_dtype_i16_new();
    } else if(parser_expect(self, Token_Type_KeywordI32) == true) {
        return ast_dtype_i32_new();
    } else if(parser_expect(self, Token_Type_KeywordI64) == true) {
        return ast_dtype_i64_new();
    } else if(parser_expect(self, Token_Type_KeywordU8) == true) {
        return ast_dtype_u8_new();
    } else if(parser_expect(self, Token_Type_KeywordU16) == true) {
        return ast_dtype_u16_new();
    } else if(parser_expect(self, Token_Type_KeywordU32) == true) {
        return ast_dtype_u32_new();
    } else if(parser_expect(self, Token_Type_KeywordU64) == true) {
        return ast_dtype_u64_new();
    } else if(parser_expect(self, Token_Type_KeywordChar) == true) {
        return ast_dtype_char_new();     
    } else {
        return NULL;
    }
}


static AST * parse_dtype(Parser * self) {
    AST * ast = NULL;

    if((ast = parse_primitive_dtype(self)) != NULL) {
        return ast;
    } else {
        return NULL;
    }
}


static AST * parse_const_expr(Parser * self) {
    size_t context = lexer_store(&self->lexer);
    Token token = lexer_next_token(&self->lexer);

    if(token.type == Token_Type_ConstNumber) {
        return ast_expr_const_number_new(token.length, token.c_str);
    } else {
        lexer_restore(&self->lexer, context);
        return NULL;
    }
}


static AST * parse_dtype_spec(Parser * self) {
    if(parser_expect(self, Token_Type_SymbolColon) == true) {
        AST * dtype = parse_dtype(self);
        assert_err(dtype != NULL, self->lexer.filename, self->lexer.state.row, self->lexer.state.column, "expected data type");
        return dtype;
    } else {
        return NULL;
    }
}


static AST * parse_expr_lambda(Parser * self) {
    if(parser_expect(self, Token_Type_SymbolOpenParenthesis) == true) {
        assert_err(parser_expect(self, Token_Type_SymbolCloseParenthesis)
                , self->lexer.filename, self->lexer.state.row, self->lexer.state.column, "expected close parenthesis ')'");
        
        AST * return_dtype = parse_dtype_spec(self);
        AST * stmt = parse_stmt(self);
        
        return ast_expr_lambda_new(return_dtype, stmt);
    } else {
        return NULL;
    }
}


static AST * parse_expr_precedence(Parser * self) {
    size_t context = lexer_store(&self->lexer);

    if(parser_expect(self, Token_Type_SymbolOpenParenthesis) == false) {
        return NULL;
    } 
   
    AST * expr = parse_expr(self);

    if(expr == NULL) {
        lexer_restore(&self->lexer, context);  
        return NULL;
    } 

    assert_err(parser_expect(self, Token_Type_SymbolCloseParenthesis)
            , self->lexer.filename, self->lexer.state.row, self->lexer.state.column, "expected close parenthesis ')'");

    return expr;
}


static AST * parse_expr_factor(Parser * self) {
    AST * ast = NULL;
    if((ast = parse_const_expr(self)) != NULL) {
        return ast;
    } else if((ast = parse_expr_precedence(self)) != NULL) {
        return ast;
    } else if((ast = parse_expr_lambda(self)) != NULL) {
        return ast;
    } else {
        return NULL;
    }
}


static AST * parse_expr_unary_op(Parser * self) {
    AST * expr = NULL;

    if(parser_expect(self, Token_Type_SymbolTilda) == true) {
        expr = parse_expr(self);
        assert_err(expr != NULL, self->lexer.filename, self->lexer.state.row, self->lexer.state.column, "expected expression");
        return ast_expr_unary_op_not_new(expr);
    } else if(parser_expect(self, Token_Type_SymbolMinus) == true) {
        expr = parse_expr(self);
        assert_err(expr != NULL, self->lexer.filename, self->lexer.state.row, self->lexer.state.column, "expected expression");
        return ast_expr_unary_op_neg_new(expr);
    } else {
        return NULL;
    }
}


static AST * parse_expr(Parser * self) {
    AST * ast = NULL;

    if((ast = parse_expr_unary_op(self)) != NULL) {
        return ast;
    } else if ((ast = parse_expr_factor(self))) {
        return ast;
    } else {
        return NULL;
    }
}


static AST * parse_stmt_return(Parser * self) {
    if(parser_expect(self, Token_Type_KeywordReturn) == true) {
        return ast_stmt_return_new(parse_expr(self));
    } else {
        return NULL;
    }
}


static AST * parse_stmt_compound(Parser * self) {
    if(parser_expect(self, Token_Type_SymbolOpenBrace) == true) {
        AST * stmt_compond = ast_stmt_compound_new();

        while(parser_expect(self, Token_Type_SymbolCloseBrace) == false) {
            Lexer_State state = self->lexer.state;
            AST * stmt = parse_stmt(self);
            assert_err(stmt != NULL, self->lexer.filename, state.row, state.column, "expected statement");

            if(stmt != NULL) {
                ast_stmt_compound_append((AST_Stmt_Compound*) stmt_compond, stmt);
            } else {
                break;
            }
        }

        return stmt_compond;
    } else {
        return NULL;
    }
}


static AST * parse_stmt(Parser * self) {
    AST * ast = NULL;

    if((ast = parse_stmt_return(self)) != NULL) {
        return ast;
    } else if((ast = parse_stmt_compound(self)) != NULL) {
        return ast;
    } else {
        return NULL;
    }
}


static AST * parse_decl_alias(Parser * self) {
    size_t context = lexer_store(&self->lexer);
    Token token = lexer_next_token(&self->lexer);

    if(token.type == Token_Type_Identifier) {
        AST * expr = NULL;

        if((expr = parse_expr_lambda(self)) != NULL) {
            return ast_decl_alias_new(token.length, token.c_str, expr);
        } else if((expr = parse_const_expr(self)) != NULL) {
            return ast_decl_alias_new(token.length, token.c_str, expr);
        } 

        assert_err(expr != NULL, self->lexer.filename, self->lexer.state.row, self->lexer.state.column, "expected expression");
        return NULL;
    } else {
        lexer_restore(&self->lexer, context);
        return NULL;
    }
}


static AST * parse_decl(Parser * self) {
    AST * ast = NULL;

    if((ast = parse_decl_alias(self)) != NULL) {
        return ast;
    } else {
        return NULL;
    }
}


AST * parse(Parser * self) {
    return parse_decl(self);
}


