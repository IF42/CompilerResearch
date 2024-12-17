#ifndef _ERR_H_
#define _ERR_H_

#include <stddef.h>


#define ASCII_RED "\x1b[31m"
#define ASCII_PURPLE  "\x1b[35m"
#define ASCII_RESET "\x1b[m"
#define ASCII_BOLD "\x1b[m"


void assert_err(int expr, char * path, size_t row, size_t column, char * msg, ...);


void assert_warn(int expr, char * path, size_t row, size_t column, char * msg, ...);

#endif

