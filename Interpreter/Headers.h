#ifndef HEADERS_H_INCLUDED
#define HEADERS_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


typedef struct Hash{
   char* variable;
   char* value;
   int isFull;
}Hash;

typedef struct Stack{
   char** arr;
   int size;
   int top;
}Stack;


void initializeHash(Hash* h);
unsigned int hashFunction(char* variable);
void put(Hash* h, char* variable, float value);
char* get(Hash* h, char* variable);
void freeHash(Hash* h);


void initializeStack(Stack *s, int size);
void push(Stack *s, char* value);
char* pop(Stack *s);
char* peek(Stack *s);
int isEmpty(Stack *s);

int isAllDigits(char* expression);
int parseInt(char* input);
char* toString(float input);
int isDigit(char* test);
int isChar(char* test);
int isOperator(char* test);
int getPrecedence(char* input);
int validateOperand(char* operand);
int getCorrespondingBracket(char* bracket);
int checkSpaces(char* input, char* output);
int checkEqual(char* input);
int checkBrackets(char* input);
void extractVariable(char* input, char* variableName);
int validateExpression(char* exp, int start, int end);
void handleNegatives(char* expression, char* exp);
int tokenizeExpression(char* expression, char** infix, Hash* variables);
void infixToPostfix(char** infix, char** postfix);
float evaluatePostfix(char** postfix);

#endif // HEADERS_H_INCLUDED
