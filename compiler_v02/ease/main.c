#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include <ir.h>
#include <asm_x86_64.h>
#include <asm_x86_64_emit.h>

#include "lexer.h"
#include "parser.h"
#include "ast.h"
#include "ir_emit.h"


typedef enum {
    OPT_Dump_Tok
    , OPT_Dump_AST
    , OPT_Dump_IR
    , OPT_Dump_ASM
} OPT_Dump;


static void usage(void) {
    printf("ease [dump_tok/dump_ast/dump_asm] <infile>\n");
}


int main(int argc, char ** argv) {
    OPT_Dump dump = OPT_Dump_ASM;
    char * infile = NULL;

    if(argc <= 1) {
        fprintf(stderr, "expected input file\n");
        usage();
        return EXIT_FAILURE;
    } else if(argc == 2) {
        infile = argv[1];
    } else if(argc == 3) {
        if(strcmp(argv[1], "dump_tok") == 0) {
            dump = OPT_Dump_Tok;
        } else if(strcmp(argv[1], "dump_ast") == 0) {
            dump = OPT_Dump_AST;
        } else if(strcmp(argv[1], "dump_ir") == 0) {
            dump = OPT_Dump_IR;
        } else if(strcmp(argv[1], "dump_asm") == 0) {
            dump = OPT_Dump_ASM;
        } else {
            fprintf(stderr, "unknown option: %s\n", argv[1]);
            usage();
            return EXIT_FAILURE;
        }

        infile = argv[2];
    }

    FILE * file = fopen(infile, "r");
    
    if(file == NULL) {
        fprintf(stderr, "can't open file %s\n", infile);
        return EXIT_FAILURE;
    }

    Lexer lexer = lexer_init(infile, file);
    if(dump == OPT_Dump_Tok) {
        for(Token t = lexer_next_token(&lexer); t.type != Token_Type_EOF; t = lexer_next_token(&lexer)) {
            token_show(&t);
        }

        lexer_finalize(&lexer);
        return EXIT_SUCCESS;
    }

    Parser parser = parser_init(lexer);
    AST * ast = parse(&parser);

    if(dump == OPT_Dump_AST) {
        ast_show(ast);
        ast_finalize(ast);
        lexer_finalize(&lexer);
        return EXIT_SUCCESS;
    }
    
    IR_Inst_Blk ir = ir_emit(ast);

    if(dump == OPT_Dump_IR) {
        ir_show(&ir, stdout);
        ast_finalize(ast);
        lexer_finalize(&lexer);
        return EXIT_SUCCESS;
    }

    ASM_Inst_Blk assembly = {0};

    if(dump == OPT_Dump_ASM) {
        assembly = ir_emit_to_asm_win_x86_64(&ir);
        size_t stack_size = asm_pseudo_pass(&assembly);
        asm_fix_assembly(&assembly, stack_size);
        asm_x86_64_emit(&assembly, stdout);
        //ir_finalize(ir);
        //asm_finalize(assembly);       
    }
    
    ast_finalize(ast);
    lexer_finalize(&lexer);
    fclose(file);

    return EXIT_SUCCESS;
}




