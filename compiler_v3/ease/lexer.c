#include "lexer.h"

#include <stdlib.h>
#include <string.h>
#include <ctype.h>


static const char * token_type_str[] = {
     "Token_Type_Identifier"
    , "Token_Type_ConstNumber"
    , "Token_Type_SymbolOpenBrace"
    , "Token_Type_SymbolCloseBrace"
    , "Token_Type_SymbolOpenParenthesis"
    , "Token_Type_SymbolCloseParenthesis"
    , "Token_Type_SymbolColon"
    , "Token_Type_SymbolTilda"
    , "Token_Type_SymbolHyphen"
    , "Token_Type_KeywordReturn"
    , "Token_Type_KeywordI8"
    , "Token_Type_KeywordI16"
    , "Token_Type_KeywordI32"
    , "Token_Type_KeywordI64"
    , "Token_Type_KeywordU8"
    , "Token_Type_KeywordU16"
    , "Token_Type_KeywordU32"
    , "Token_Type_KeywordU64"
    , "Token_Type_KeywordChar"
    , "Token_Type_Undefined"
    , "Token_Type_EOF"
};


void token_show(Token * token) {
    if(token->type == Token_Type_Identifier || token->type == Token_Type_ConstNumber) {
        printf("Token {type = %s, length = %zu, c_str = \"%.*s\"}\n", token_type_str[token->type], token->length, (int) token->length, token->c_str);
    } else {
        printf("Token {type = %s}\n", token_type_str[token->type]);
    }
}


static size_t file_size (FILE * f) {
    fseek(f, 0L, SEEK_END);
    size_t size = ftell(f);
    fseek(f, 0L, SEEK_SET);
    return size;
}


Lexer lexer_init(char * filename, FILE * f) {
    size_t size = file_size(f);
    char * buff = malloc(sizeof(char) * (size + 1));

    fread(buff, sizeof(char), size, f);

    return (Lexer) {
        .f = f
        , .filename = filename
        , .state = (Lexer_State) {.row = 1, .column = 1, .index = 0}
        , .buff = buff
        , .size = size
    };
}


void lexer_finalize(Lexer * self) {
    if(self->buff != NULL) {
        free(self->buff);
    }

    if(self->token.arr != NULL) {
        free(self->token.arr);
    }
}


static inline char lexer_character(Lexer * self) {
    return self->buff[self->state.index];
}


static inline char * lexer_string_reference(Lexer * self) {
    return &self->buff[self->state.index];
}


static void lexer_advance(Lexer * self) {
    if(self->buff[self->state.index] == '\n') {
        self->state.row ++;
        self->state.column = 1;
    } else {
        self->state.column ++;
    }

    self->state.index++;
}


static char lexer_peek(Lexer * self) {
    if(self->state.index <= self->size) {
        return self->buff[self->state.index + 1];
    } else {
        return 0;
    }
}


static Token lexer_read_number(Lexer * self) {
    Token token = {.type = Token_Type_ConstNumber, .c_str = lexer_string_reference(self)};

    while(isdigit(lexer_character(self)) || lexer_character(self) == '.') {
        lexer_advance(self);
        token.length ++;
    }

    return token;
}


static const char * keyword[] = {
    "return", "i8", "i16", "i32", "i64", "u8", "u16"
    , "u32", "u64", "char"
};


static Token lexer_read_keyword_or_identifier(Lexer * self) {
    Token token = {.c_str = lexer_string_reference(self)};

    while(isalnum(lexer_character(self)) || lexer_character(self) == '_' ) {
        lexer_advance(self);
        token.length++;
    }

    for(size_t i = 0, max = sizeof(keyword) / sizeof(*keyword); i < max; i++) {
        if(strlen(keyword[i]) == token.length && strncmp(keyword[i], token.c_str, token.length) == 0) {
            token.type = i + Token_Type_KeywordReturn;
            return token;
        }
    }

    token.type = Token_Type_Identifier;
    return token;
}


static inline void lexer_skip_line_comment(Lexer * self) {
    while(lexer_character(self) == 0 && lexer_character(self) != '\n') {
        lexer_advance(self);
    }
}


static inline void lexer_skip_block_comment(Lexer * self) { 
    while(lexer_character(self) != 0 && (lexer_character(self) != '*' || lexer_peek(self) != '/')) {
        lexer_advance(self);
	}

    lexer_advance(self);
    lexer_advance(self);
}


static inline void lexer_skip_white_space(Lexer * self) {
    while(isspace(lexer_character(self)) && lexer_character(self) != 0) {
        lexer_advance(self);
    }
}


static inline Token lexer_read_symbol(Lexer * self, Token_Type type) {
    lexer_advance(self);
    return (Token) {.type = type};
}


static Token lexer_read_token(Lexer * self) {
    while(lexer_character(self) != 0) {
        if(isspace(lexer_character(self))) {
            lexer_skip_white_space(self);
            continue;
        }

        switch(lexer_character(self)) {
            case '(':
                return lexer_read_symbol(self, Token_Type_SymbolOpenParenthesis);
            case ')':
                return lexer_read_symbol(self, Token_Type_SymbolCloseParenthesis);
            case '{':
                return lexer_read_symbol(self, Token_Type_SymbolOpenBrace);
            case '}':
                return lexer_read_symbol(self, Token_Type_SymbolCloseBrace);
            case ':':
                return lexer_read_symbol(self, Token_Type_SymbolColon);
            case '~':
                return lexer_read_symbol(self, Token_Type_SymbolTilda);
            case '-':
                return lexer_read_symbol(self, Token_Type_SymbolHyphen);
            case '/':
                if(lexer_peek(self) == '/') {
                    lexer_skip_line_comment(self);
                    continue;
                } else if(lexer_peek(self) == '*') {
                    lexer_skip_block_comment(self);
                    continue;
                }
                break;
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
                return lexer_read_number(self);
            default:
                return lexer_read_keyword_or_identifier(self);
        }
    }

    return (Token) {.type = Token_Type_EOF};
}


Token lexer_next_token(Lexer * self) {
    while (self->iterator >= self->token.size) {
        if (self->token.size >= self->token.capacity) {
            self->token.capacity = (self->token.capacity + 1) * 2;
            self->token.arr = realloc(self->token.arr, sizeof(Token) * self->token.capacity);    
        }

        self->token.arr[self->token.size++] = lexer_read_token(self);
    } 

    return self->token.arr[self->iterator++];
}


size_t lexer_store(Lexer * self) {
    return self->iterator;
}


void lexer_restore(Lexer * self, size_t state) {
    self->iterator = state;
}



