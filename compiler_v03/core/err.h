#ifndef _ERR_H_
#define _ERR_H_

#include <stddef.h>


#define ASCII_RED "\x1b[31m"
#define ASCII_PURPLE  "\x1b[35m"
#define ASCII_RESET "\x1b[m"
#define ASCII_BOLD "\x1b[m"


void _assert_err(char * path, size_t row, size_t column, char * msg, ...);


#define assert_err(_exp, ... ) \
    (_exp) == 0 ? _assert_err(__VA_ARGS__) : ((void) 0)


void _assert_warn(char * path, size_t row, size_t column, char * msg, ...);


#define assert_warn(_exp, ... ) \
    (_exp) == 0 ? _assert_warn(__VA_ARGS__) : ((void) 0)

#endif

