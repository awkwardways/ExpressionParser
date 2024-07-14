#include <stdio.h>
#include <stdlib.h>
#include "include/parser.h"
#include "include/lexer.h"

int main(int argc, char* argv[]) {
  if(argc < 2) {
    printf("NO FILE PROVIDED");
    return EXIT_FAILURE;
  }
  FILE* fptr = fopen(argv[1], "r");
  if(fptr == NULL) {
    printf("ERROR OPENING FILE");
    return EXIT_FAILURE;
  }
  // token_t token;
  // initLexer(fptr);
  // do {
  //   token = scan();
  //   if(token.tag == NUM) {
  //     printf("<NUM, %d>\n", token.numval);
  //   }
  //   else {
  //     printf("<%d, %s>\n", token.tag, token.lexeme);
  //   }
  // } while (token.tag != ENDOFFILE);
  parse(fptr);
  fclose(fptr);
  return EXIT_SUCCESS;
}