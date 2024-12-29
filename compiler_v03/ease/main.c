#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <alloc/sys_alloc.h>

#include <ir.h>
#include <asm_x86_64.h>
#include <asm_win_x86_64_emit.h>
#include <asm_linux_x86_64_emit.h>

#include "lexer.h"
#include "parser.h"
#include "ast.h"
#include "ir_emit.h"

typedef enum {
    Platform_x86_64_Windows
    , Platform_x86_64_Linux
} Platform;


typedef enum {
    OPT_Dump_Tok
    , OPT_Dump_AST
    , OPT_Dump_IR
    , OPT_Dump_ASM
} OPT_Dump;


static void usage(void) {
    printf("ease [dump_tok/dump_ast/dump_asm] <infile> [outfile]\n");
}


int main(int argc, char ** argv) {
    OPT_Dump dump = OPT_Dump_ASM;
    char * infile = NULL;
    char * outfile = NULL;

    if(argc <= 1) {
        fprintf(stderr, "expected input file\n");
        usage();
        return EXIT_FAILURE;
    } else if(argc == 2) {
        infile = argv[1];
    } else if(argc >= 3) {
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

    if(argc == 4) {
        outfile = argv[3];
    }

    FILE * f_in = fopen(infile, "r");
    FILE * f_out = NULL;

    if(f_in == NULL) {
        fprintf(stderr, "can't open file %s\n", infile);
        return EXIT_FAILURE;
    }

    if(outfile != NULL) {
        f_out = fopen(outfile, "w");

        if(f_out == NULL) {
            fprintf(stderr, "can't write file %s\n", outfile);
        }
    } else {
        f_out = stdout;
    }

    SysAlloc lexer_alloc = sys_alloc();
    Lexer lexer = lexer_init(ALLOC(&lexer_alloc), infile, f_in);

    if(dump == OPT_Dump_Tok) {
        for(Token t = lexer_next_token(&lexer); t.type != Token_Type_EOF; t = lexer_next_token(&lexer)) {
            token_show(&t, f_out);
        }

        finalize(ALLOC(&lexer_alloc));
        return EXIT_SUCCESS;
    }

    SysAlloc parser_alloc = sys_alloc();
    Parser parser = parser_init(ALLOC(&parser_alloc), lexer);
    AST * ast = parse(&parser);

    fclose(f_in);

    if(dump == OPT_Dump_AST) {
        ast_show(ast, f_out);
        finalize(ALLOC(&parser_alloc));
        finalize(ALLOC(&lexer_alloc));
        return EXIT_SUCCESS;
    }
    
    SysAlloc ir_alloc = sys_alloc();
    IR_Inst_Blk ir = ir_emit(ALLOC(&ir_alloc), ast);
    
    finalize(ALLOC(&parser_alloc));

    if(dump == OPT_Dump_IR) {
        ir_show(&ir, f_out);
        finalize(ALLOC(&ir_alloc));
        finalize(ALLOC(&lexer_alloc));
        return EXIT_SUCCESS;
    }

    SysAlloc asm_alloc = sys_alloc();
    ASM assembly;
    
    assembly = asm_linux_x86_64(ALLOC(&asm_alloc), &ir);
    //assembly = asm_win_x86_64(ALLOC(&asm_alloc), &ir);

    finalize(ALLOC(&ir_alloc));

    asm_x86_64_emit(&assembly, f_out);

    finalize(ALLOC(&asm_alloc));
    finalize(ALLOC(&lexer_alloc));

    return EXIT_SUCCESS;
}




