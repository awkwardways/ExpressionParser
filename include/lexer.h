#ifndef LEXER_H
#define LEXER_H

#include <stdio.h>
#define MAX_LEXEME_LENGTH 256
#define BUFFER_SIZE 4096

typedef enum {
  NUM = 256, ID, OPERATOR, OP_PAR, CL_PAR, ENDOFFILE, ERROR 
} tokenTag;

typedef struct {
  tokenTag tag;
  union {
    char lexeme[MAX_LEXEME_LENGTH];
    int numval;
  };
} token_t;

void initLexer(FILE* src);
token_t scan();
#endif