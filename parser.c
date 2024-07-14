#include <stdio.h>
#include <stdbool.h>
#include "include/parser.h"
#include "include/lexer.h" 
//Recursive descent parser for the basic expression grammar:
/*
List -> ExprList
Expr -> termExpr'
Expr' -> +termExpr' | -termExpr' | term
term -> factorterm'
term' -> *factorterm' | /factorterm' | factor
factor -> num | name | (Expr)
*/
bool list();
bool term();
bool expr();
bool exprP();
bool termP();
bool factor();


token_t currentToken = {0, 0, 0};

void getNextToken() { 
  currentToken = scan();
  printf("CURRENT TOKEN: ");
  if(currentToken.tag == NUM) printf("%d\n", currentToken.numval);
  else printf("%s\n", currentToken.lexeme);
}

bool list() {
  if(expr()) {
    if(currentToken.tag == ENDOFFILE) return true;
    else return list();
  }
  return false;
}

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
  else if(currentToken.tag == CL_PAR || currentToken.tag == ENDOFFILE || currentToken.tag == OP_PAR || currentToken.tag == NUM || currentToken.tag == ID) return true;
  else printf("Error. Expected expr' prodcution\n");
}

bool termP() {
  if(currentToken.lexeme[0] == '*' || currentToken.lexeme[0] == '/') {
    getNextToken();
    if(factor()) return termP();
    else {
      printf("Error. Expected number or id\n");
      return false;
    } 
  }
  else if(currentToken.tag == CL_PAR || currentToken.tag == ENDOFFILE || currentToken.lexeme[0] == '+' || currentToken.lexeme[0] == '-' || currentToken.tag == NUM || currentToken.tag == ID || currentToken.tag == OP_PAR) {
    return true;
  }
  else printf("Error. Expected ), EOF, + or - production\n");
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
  if(list()) {
    printf("Success parsing!\n");
  }
  else {
    printf("Error parsing!\n");
  }
}