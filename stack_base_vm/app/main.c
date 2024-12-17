#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>


typedef enum {
    OP_PUSH
   , OP_PLUS
   , OP_MINUS
   , OP_DUMP
   , COUNT_OPS
} OP;


typedef struct {
    OP OP;
    long value;
} Instruction;


#define push(_value) \
    (Instruction) {.OP = OP_PUSH, .value = _value}


#define plus() \
    (Instruction) {.OP = OP_PLUS}


#define minus(void) \
    (Instruction) {.OP = OP_MINUS}


#define dump(void) \
    (Instruction) {.OP = OP_DUMP}


typedef struct {
    size_t capacity;
    size_t size;
    size_t * array;
} ProgramStack;


#define program_stack_init() (ProgramStack) {0}


static void program_stack_finalize(ProgramStack * self) {
    if(self->array != NULL) {
        free(self->array);
    }
}


static void program_stack_push(ProgramStack * self, long value) {
    if(self->size >= self->capacity) {
        self->capacity = (self->capacity + 1) * 2;
        self->array = realloc(self->array, sizeof(size_t) * self->capacity);
    }

    self->array[self->size++] = value;
}


static long program_stack_pop(ProgramStack * self) {
    return self->array[--self->size];
}


static void simulate_program(size_t size, Instruction * program) {
    ProgramStack program_stack = program_stack_init();

    for(size_t i = 0; i < size; i++) {
        switch(program[i].OP) {
            case OP_PUSH:
                program_stack_push(&program_stack, program[i].value);
                break;
            case OP_PLUS: 
                program_stack_push(&program_stack, program_stack_pop(&program_stack) + program_stack_pop(&program_stack));
                break;
            case OP_MINUS: 
                program_stack_push(&program_stack, program_stack_pop(&program_stack) - program_stack_pop(&program_stack));
                break;
            case OP_DUMP:
                printf("%ld\n", program_stack_pop(&program_stack));
                break;
            default:
                fprintf(stderr, "Unknown instruction %d\n", program[i].OP);
        }
    }

    program_stack_finalize(&program_stack);
}


static void compile_program(size_t size, Instruction * program) {
    FILE * f = fopen("output.asm", "w");

    if(f == NULL) {
        fprintf(stderr, "Can't open file 'output.asm'\n");
        exit(EXIT_FAILURE);
    }

    fprintf(f, "extern printf\n");
    fprintf(f, "extern ExitProcess\n");
    fprintf(f, "global _start\n");
    fprintf(f, "global main\n");
    fprintf(f, "\nsection .data\n");
    fprintf(f, "\tfmt db \"%%d\", 10, 0\n");
    fprintf(f, "\nsection .text\n");
    fprintf(f, "\n_start:\n");
    fprintf(f, "\tcall main\n");
    fprintf(f, "\nmain:\n");

    for(size_t i = 0; i < size; i ++) {
        switch(program[i].OP) {
            case OP_PUSH:
                fprintf(f, "\tpush %ld\n", program[i].value);
                break;
            case OP_PLUS: 
                fprintf(f, "\n\tpop rax\n");
                fprintf(f, "\tpop rbx\n");
                fprintf(f, "\tadd rax, rbx\n");
                fprintf(f, "\tpush rax\n");
                break;
            case OP_MINUS: 
                fprintf(f, "\n\tpop rax\n");
                fprintf(f, "\tpop rbx\n");
                fprintf(f, "\tsub rax, rbx\n");
                fprintf(f, "\tpush rax\n");
                break;
            case OP_DUMP:
                fprintf(f, "\n\tpop rax\n");
                fprintf(f, "\tmov rcx, fmt\n");
                fprintf(f, "\tmov rdx, rax\n");
                fprintf(f, "\tcall printf\n\n");
                break;
            default:
                fprintf(stderr, "Unknown instruction %d\n", program[i].OP);
                exit(EXIT_FAILURE);
        }
    }
    
    fprintf(f, "\tmov rcx, rax\n");
    fprintf(f, "\tcall ExitProcess\n");
    
    fclose(f);
}


static void usage(void) {
    printf("Usage: stack_based_vm <SUBCOMMAND> [ARGS]\n");
    printf("SUBCOMMANDS:\n");
    printf("    sim <file>    simulate the program\n");
    printf("    com <file>    compile the program\n");
}


int main(int argc, char** argv) {
    if(argc < 2) {
        usage();
        fprintf(stderr, "No subcomand is provided!\n");
        return EXIT_FAILURE;
    }

    Instruction program[] = {
        push(35)
        , push(34)
        , minus()
        , dump()
        , push(42)
        , push(1)
        , plus()
        , dump()
    };

    int return_code = EXIT_SUCCESS;

    if(strcmp("sim", argv[1]) == 0) {
        simulate_program(sizeof(program)/sizeof(*program), program);
    } else if(strcmp("com", argv[1]) == 0) {
        compile_program(sizeof(program)/sizeof(*program), program);
        system("nasm -fwin64 output.asm -o output.o");
        system("/ucrt64/bin/ld -e _start ./output.o -o output -lkernel32 -lmsvcrt");
    } else {
        usage();
        fprintf(stderr, "\nUnknown subcomand!\n");
        return_code = EXIT_FAILURE;
    }

    return return_code;
}



