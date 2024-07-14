#include <stdio.h>
#include <stdbool.h>
#include "include/parser.h"
#include "include/lexer.h" 
//Recursive descent parser for the basic expression grammar:
/*
Expr -> termExpr'
Expr' -> +termExpr' | -termExpr' | term
term -> factorterm'
term' -> *factorterm' | /factorterm' | factor
factor -> num | name | (Expr)
*/

bool term();
bool expr();
bool factor();
bool termP();
bool exprP();


token_t currentToken = {0, 0, 0};

void getNextToken() { currentToken = scan();}

bool term() {
  if(factor()) return termP();
  else {
    printf("Error. Expected factor production\n");
    return false;
  }
}

bool expr() {
  if(term()) return exprP();
  else {
    printf("Error. Expected expr production\n");
    return false;
  }
}

bool exprP() {
  if(currentToken.lexeme[0] == '+' || currentToken.lexeme[0] == '-') {
    getNextToken();
    if(term()) return exprP();
    else {
      printf("Error. Expected expr' production\n");
      return false;
    }
  }
  else if(currentToken.tag == CL_PAR || currentToken.tag == ENDOFFILE) return true;
  else printf("Error. Expected exprr' prodcution\n");
}

bool termP() {
  if(currentToken.lexeme[0] == '*' || currentToken.lexeme[0] == '/') {
    getNextToken();
    if(factor()) return termP();
    else {
      printf("Error. Expected term' production\n");
      return false;
    } 
  }
  else if(currentToken.tag == CL_PAR || currentToken.tag == ENDOFFILE || currentToken.lexeme[0] == '+' || currentToken.lexeme[0] == '-') {
    return true;
  }
  else printf("Error. Expected term' production\n");
}

bool factor() {
  if(currentToken.tag == ID || currentToken.tag == NUM) {
    getNextToken();
    return true;
  }
  else if(currentToken.tag == OP_PAR) {
    getNextToken();
    if(!expr() || currentToken.tag != CL_PAR) {
      printf("Error. Expected expr production\n");
      return false;
    }
    getNextToken();
    return true;
  }
  else {
    printf("Error. Expected factor\n");
    return false;
  }
}

void parse(FILE* src) {
  initLexer(src);
  getNextToken();
  if(expr()) {
    if(currentToken.tag == ENDOFFILE) printf("Success parsing!\n");
  }
  else {
    printf("Error parsing!\n");
  }
}