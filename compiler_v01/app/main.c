#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <lexer.h>
#include <parser.h>
#include <ast.h>
#include <asm.h>
#include <emit.h>


typedef enum {
    OPT_Dump_Tok
    , OPT_Dump_AST
    , OPT_Dump_ASM
} OPT_Dump;


OPT_Dump dump = OPT_Dump_ASM;
char * infile;


static void usage(void) {
    printf("ease [dump_tok/dump_ast/dump_asm] <infile>\n");
}


int main(int argc, char ** argv) {
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
            if(t.type == Token_Type_Identifier || t.type == Token_Type_ConstNumber) {
                printf("Token {type = %s, length = %zu, c_str = \"%.*s\"}\n", token_type_str[t.type], t.length, (int) t.length, t.c_str);
            } else {
                printf("Token {type = %s}\n", token_type_str[t.type]);
            }
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

    if(dump == OPT_Dump_ASM) {
        ASM_Prog asm_prog = emit_to_win_x86_64_asm(ast);
        asm_emit(&asm_prog, stdout);
        asm_finalize(&asm_prog);       
    }
    
    ast_finalize(ast);
    lexer_finalize(&lexer);
    fclose(file);

    return EXIT_SUCCESS;
}




