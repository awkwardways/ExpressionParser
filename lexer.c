#include "include/lexer.h"
#include <stdbool.h>

int lineNumber = 1, charNumber = 1;
char* currentCharacter = NULL;
FILE* source = NULL;
char buffer[BUFFER_SIZE] = {};
bool endOfFile = 0;

void reloadBuffer() {
  fgets(buffer, BUFFER_SIZE, source);
  currentCharacter = buffer;
  charNumber = 1;
  lineNumber++;
}

void skipLineComment() {
  while(*currentCharacter != '\n') {
    currentCharacter++;
  }
  reloadBuffer();
}

void skipBlockComment() {
  while(1) {
    if(*currentCharacter == '*' && *(currentCharacter + 1) == '/') {
      currentCharacter += 2;
      charNumber += 2;
      return;
    }
    else if(*currentCharacter == '\n') reloadBuffer();
    else {currentCharacter++; charNumber++;}
  }
}

void skipWhitespace() {
  while(1) {
    if(*currentCharacter == '\n' || *currentCharacter == '\0') reloadBuffer();
    else if(*currentCharacter == '/') {
      switch(*(currentCharacter + 1)) {
        case '/': 
          skipLineComment();
        break;
        case '*': 
          skipBlockComment();
        break;
        default:
          return;
        break;
      }
    }
    else if(*currentCharacter != ' ' && *currentCharacter != '\t') return;
    else {currentCharacter++; charNumber++;}
  }
}

bool isNumber() {
  return(*currentCharacter <= 57 && *currentCharacter >= 48);
}

bool isCharacter() {
  return((*currentCharacter >= 65 && *currentCharacter <= 90) || (*currentCharacter >= 97 && *currentCharacter <= 122));
}

bool isOperator() {
  return(*currentCharacter == '+' || *currentCharacter == '-' || *currentCharacter == '*' || *currentCharacter == '/' );
}

void getOperator(token_t *token) {
  token->lexeme[0] = *currentCharacter;
  currentCharacter++;
  charNumber++;
}

void getId(token_t *token) {
  int i = 0;
  while(isCharacter()) {
    token->lexeme[i] = *currentCharacter;
    currentCharacter++;
    charNumber++;
    i++;
  }
}

void getNumber(token_t *token) {
  token->numval = 0;
  while(isNumber()) {
    token->numval = (token->numval * 10) + (*currentCharacter - 48);
    currentCharacter++;
    charNumber++;
  }
}

void initLexer(FILE* src) {
  source = src;
  reloadBuffer();
}

token_t scan() {
  token_t currentToken = {0, 0, 0};
  if(endOfFile) {
    currentToken.tag = ENDOFFILE;
    return currentToken;
  }
  skipWhitespace();
  //current character may be pointing to a number, operator or an ID
  if(isNumber()) {
    currentToken.tag = NUM;
    getNumber(&currentToken);
  }
  else if(isCharacter()) {
    currentToken.tag = ID;
    getId(&currentToken);
  }
  else if(isOperator()) {
    currentToken.tag = OPERATOR;
    getOperator(&currentToken);
  }
  else if(*currentCharacter == '(' || *currentCharacter == ')') {
    if(*currentCharacter % 2) {
      currentToken.tag = OP_PAR;
      currentToken.lexeme[0] = '(';
    }
    else {
      currentToken.tag = CL_PAR;
      currentToken.lexeme[0] = ')';
    }
    currentCharacter++;
    charNumber++;
  }
  else {
    printf("CHARACTER %d AT LINE %d NOT RECOGNIZED\n");
    return currentToken;
  }
  if(*currentCharacter == '\0') {
    endOfFile = 1;
  }
  return currentToken;
}
