#include "err.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>


static void show_message(char * source, size_t row, size_t column, char * assert_label, char * msg, va_list args) {
    fprintf(stderr, "%s:%zu:%zu: %s: ", source, row, column, assert_label);
    vfprintf(stderr, msg, args);
    fprintf(stderr, "\n");
}


void _assert_err(char * path, size_t row, size_t column, char * msg, ...) {
    va_list args;
    va_start(args, msg);
    show_message(path, row, column, " error", msg, args);
    va_end(args);
    exit(1);
}


void _assert_warn(char * path, size_t row, size_t column, char * msg, ...) {
    va_list args;
    va_start(args, msg);
    show_message(path, row, column, "warning", msg, args);
    va_end(args);
}


