#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#include "Headers.h"

///////////// HASHMAP /////////////

void initializeHash(Hash* h){
   int i;
   h = (Hash*)malloc(512*sizeof(Hash));
   for(i=0; i<512; i++){
      h[i].isFull=0;
      h[i].variable=(char*)malloc(16*sizeof(char));
      h[i].value=(char*)malloc(16*sizeof(char));
   }
}

unsigned int hashFunction(char* variable){

    unsigned int index=0;
    int i;
    for(i=0; variable[i]!='\0'; i++){
       index= (variable[i] + (index << 6) + (index << 16) - index)%512;
    }
    while(index>=512){
        index=index%512;
    }
    return index;
}

void put(Hash* h, char* variable, float value){
    int index = hashFunction(variable);
    while(index%512){
      // If Variable already exists, override it's value
      if(h[index].isFull && strcmp(variable, h[index].variable)==0){
          strcpy(h[index].value, toString(value));
          //printf("Updated!\n");
          return;
      }
      // If index is empty
      if(h[index].isFull==0){
          h[index].isFull=1;
          strcpy(h[index].variable, variable);
          strcpy(h[index].value,toString(value));
          //printf("Added!\n");
          return;
      }
      // check following index
      index++;
    }
}

char* get(Hash* h, char* variable){

    int index = hashFunction(variable);
    int firstIndex=-1;

    while(firstIndex != index)
    {
        if(h[index].isFull!=0)
        {
            if(strcmp(h[index].variable,variable)==0)
                return h[index].value;
        }
        if(firstIndex == -1)
            firstIndex = index;
        index= ( index + 1 ) % 500;
    }
    return "lol";

}

void freeHash(Hash* h){
   int i;
   for(i=0; i<512; i++){
       if(h[i].variable!=NULL){
          free(h[i].variable);
          free(h[i].value);
       }
   }
}

///////////// HASHMAP /////////////

///////////// STRING /////////////

int isAllDigits(char* expression){
   int i=0;
   while(expression[i]!='\0'){
    if(i==0 && expression[i]=='-'){
        i++;
        continue;
    }
    if(expression[i]>'9' || expression[i]<'0'){
      return 0;
    }
    i++;
   }
   return 1;
}

char* toString(float input){
   char* numString= (char*)malloc(16*sizeof(char));
   snprintf(numString, 16, "%f", input);
   return numString;
}

int isDigit(char* test){
   if(*(test)>='0' && *(test)<='9'){
      return 1;
   }
   return 0;
}

int isChar(char* test){
   if(*(test)>='a' && *(test)<='z'){
      return 1;
   }
   if(*(test)>='A' && *(test)<='Z'){
      return 1;
   }
   if(*(test)=='_'){
      return 1;
   }
   return 0;
}

int isOperator(char* test){
   if(*(test)=='+')
     return 1;
   if(*(test)=='-')
     return 1;
   if(*(test)=='*')
     return 1;
   if(*(test)=='/')
     return 1;
   return 0;
}

int getPrecedence(char* input){
   if (*input =='+' || *input == '-') return 1;
   if (*input=='*' || *input=='/') return 2;
   return 0;
}

int validateOperand(char* operand){
   int i=1;
   int dotCount=0;
   if(isChar(operand)){
    return 1;  // It's a variable, checked in function handleExpression()
   }
   //if valid number
   if(!isDigit(operand)){
     //printf("Error - Illegal Input Entered!\n");
     return 0;
   }
   while(*(operand+i)!='\0'){
     if(!isDigit(operand+i) && *(operand+i)!='.'){
        //printf("Error - Illegal Input Entered!\n");
        return 0;
     }
     if(*(operand+i)=='.'){
       if(dotCount==1){ // Already have a point
        //printf("Error - Illegal Input Entered!\n");
        return 0;
       }
       else{
        dotCount++;
       }
     }
     else if(!isDigit(operand+i)){
        //printf("Error - Illegal Operand Entered!\n");
        return 0;
     }
     i++;
   }
   return 1;
}

int getCorrespondingBracket(char* bracket){
    //printf("Brackets Function!\n");
    int bracketsToSkip=0;
    int i=1;
    while(*(bracket+i)!='\0'){
        if(*(bracket+i)=='('){
            bracketsToSkip++;
        }
        if(*(bracket+i)==')'){
            if(bracketsToSkip==0){ // THIS IS THE ONE
                return i;
            }
            else{
                bracketsToSkip--;
            }
        }
        i++;
    }
    printf("K This is the corresponding Brackets function and this should never happen.... Like, ever!");
    return 0;
}

int checkSpaces(char* input, char* output){
   int i=0;
   int k=0;
   while(input[i]!='\n'){
        if(input[i]==' '){
            if(i==0 || isOperator(input+i-1)|| isOperator(input+i+1) || input[i-1]=='=' || input[i+1]=='=' || input[i-1]==')'|| input[i+1]==')' ||input[i-1]=='(' || input[i+1]=='('){
               // Do Nothing
            }
            else {
              //printf("Error - incompatible Space!\n");
              return 0;
            }
        }
        else{
           output[k++]=input[i];
        }
        i++;
   }
   output[k]='\0';
   return 1;
}

int checkEqual(char* input){
    int equal =0;
    int i=0;
    while(input[i]!='\n' && input[i]!='\0'){
        if(input[i]=='='){
            equal++;
        }
        i++;
    }
    if(equal!=1){
        //printf("Error - '=' sign mistake!\n");
        return 0;
    }
    int k;
    for(k=0; input[k]!='='; k++){
        if(k==0 && !isChar(input+k)){
            //printf("Error - Invalid variable Name!\n");
            return 0;
        }
        if(!isChar(input+k)&& !isDigit(input+k)){
            //printf("Error - Invalid variable Name!\n");
            return 0;
        }
    }
    return 1;
}

int checkBrackets(char* input){
    int brackets=0;
    int i=0;
    while(input[i]!='\n'){
        //printf("Brackets = %d, character = %c\n", brackets, input[i]);
        if(input[i]=='('){
           brackets++;
        }
        if(input[i]==')'){
            if (brackets==0){
                //printf("Error - Unbalanced Brackets!\n");
                return 0;
            }
            brackets--;
        }
        i++;
    }
    if(brackets!=0){
        //printf("error - Unbalanced Brackets!\n");
        return 0;
    }
    return 1;
}

void extractVariable(char* input, char* variable){
     int i=0;
     while (*(input+i)!='='){
        *(variable+i)=*(input+i);
        i++;
     }
     *(variable+i)='\0';
}

void handleNegatives(char* expression, char* exp){
    int i=0;
    int k=0;
    while(*(expression+i)!='\0'){
        if(*(expression+i)=='-'){
           if(i==0 || *(expression+i-1)=='*' || *(expression+i-1)=='/'  || *(expression+i-1)=='(' || *(expression+i-1)=='+' || *(expression+i-1)=='-'){
            *(exp+k)='(';
            k++;
            *(exp+k)='0';
            k++;
            *(exp+k)=*(expression+i);
            k++;
            i++;
            while(!isOperator(expression+i)){
              if(*(expression+i)!='\0'){
                *(exp+k)=*(expression+i);
                k++;
                i++;
              }
              else{  // Edge case - Reached end of expression
                *(exp+k)=')';
                k++;
                *(exp+k)='\0';
                return;
              }
            }
            *(exp+k)=')';
            k++;
           }
        }
        *(exp+k)=*(expression+i);
        k++;
        i++;
    }
    *(exp+k)='\0';
}

int validateExpression(char* exp, int start, int end){
  //printf("Expression function!\n");
  int lastOperator=start;
  int expIterator=start;
  int tempIterator=0;

  char* temp = (char*)malloc(16*sizeof(char));

  while(expIterator<= end){
   if(*(exp+expIterator)=='('){
        if(!isOperator(exp+expIterator-1) && exp[expIterator-1]!='('){
           return 0;
        }
        int newStart = expIterator+1;
        int newEnd = newStart + getCorrespondingBracket(exp+expIterator)-1;
        //printf("From:%d , %c --- To:%d , %c\n",newStart, *(exp+newStart), newEnd, *(exp+newEnd));
        if(!validateExpression(exp, newStart, newEnd)){
           return 0;
        }
        else{
           // Check if between them is an operator
           if(isOperator((exp+newEnd+1)) || *(exp+newEnd+1)=='\0' || *(exp+newEnd+1)==')'){
            // Assume this is a valid number and continue;
            //printf("This one is valid!\n");
            lastOperator = newEnd+2;
            expIterator = newEnd+2;
            tempIterator =0;
            continue;
           }
           else{
            //printf("Error - Illegal Input!\n");
            return 0;
           }
        }
   }
   if(isOperator(exp+expIterator) || expIterator==end){
     while(lastOperator<expIterator){
        *(temp+tempIterator) = *(exp+lastOperator);
        lastOperator++;
        tempIterator++;
     }
     *(temp+tempIterator)='\0';
     if(!validateOperand(temp)){
        return 0;
     }
     else{
        lastOperator=expIterator+1;
        tempIterator=0;
     }
   }
  expIterator++;
  }
  return 1;
}

int tokenizeExpression(char* expression, char** infix, Hash* variables){

    int expIterator=0, tempIterator, infixIterator=0, j;
    int lastOperator=0;
    char opString[2];
    char end[16]="EXPRESSION_END";
    char* temp = (char*)malloc(16*sizeof(char));

    while(expression[expIterator]!='\0'){
        if(isOperator(expression+expIterator) || expression[expIterator+1]=='\0' || expression[expIterator]==')'){  /// Possible characters before it: operands(variable/constant) - a ')'
            tempIterator=0;
            for(j=lastOperator; j<expIterator; j++){
                temp[tempIterator]=expression[j];
                tempIterator++;

            }
            temp[tempIterator]='\0';
            // check if temp is constant or variable
            // if is variable
            if(isChar(temp)){
                if(strcmp(get(variables, temp), "lol")!=0){
                 strcpy(temp, get(variables, temp));
              }
              else{
                 printf("Error - Variable doesn't exist!\n");
                 return 0;
              }
            }
            // If is operand : Already validated before!!!
            if(*temp!='\0'){   // In case a ')' was before
              strcpy(*(infix+infixIterator), temp);
              infixIterator++;
            }
            opString[0]=expression[expIterator];
            opString[1]='\0';
            strcpy(*(infix+infixIterator), opString);
            infixIterator++;
            lastOperator=expIterator+1;
        }
        else if(expression[expIterator]=='('){ /// Possible characters before it: operator - another '('
            //If operator is before -> Already Handled.
            opString[0]=expression[expIterator];
            opString[1]='\0';
            strcpy(*(infix+infixIterator), opString);
            infixIterator++;
            lastOperator=expIterator+1;
            //If another '(' is before -> Now handled above
        }
        expIterator++;
    }
    strcpy(*(infix+infixIterator), end);
    free(temp);
    return 1;
}

void infixToPostfix(char** infix, char** postfix){

  Stack operators;
  initializeStack(&operators, 10);
  push(&operators, "lol");

  char* op_ptr = (char*)malloc(16*sizeof(char));

  int i=0;
  int k=0;
  // DEBUGGING
  //printf("infix to postfix function\n");
  //for(i=0; strcmp(*(infix+i), "EXPRESSION_END")!=0; i++){
  //      printf("Infix At Index %d: %s\n", i, *(infix+i));
  //}

  i=0;
  while(strcmp(*(infix+k), "EXPRESSION_END")!=0){

    if(isDigit(*(infix+k))){   // An Operand is Found- put it in
      strcpy(*(postfix+i), *(infix+k));
      //copyString(*(infix+k), *(postfix+i));
      i++;
    }

    else if(*(*(infix+k))=='('){            // A Left Parenthesis is Found
      //printf("Pushing Parentheses!\n");
      push(&operators, *(infix+k));
      //printf("Parentheses pushed!\n");
    }

    else if(*(*(infix+k))==')'){            // A Right Parenthesis is Found
        while(*(peek(&operators))!='('){
            op_ptr = peek(&operators);
            strcpy(*(postfix+i), op_ptr);
            //copyString(op_ptr, *(postfix+i));
            pop(&operators);
            i++;
        }
        pop(&operators);
    }

    else{                            // An Operator is Found
      //printf("operator Found!\n");
      //printf("%s\n", peek(&operators));
      while(getPrecedence((peek(&operators)))>=getPrecedence(*(infix+k))){
         op_ptr = peek(&operators);
         strcpy(*(postfix+i), op_ptr);
         pop(&operators);
         i++;
      }
      push(&operators, *(infix+k));
    }
    //printf("content in postfix of index %d: %s\n", i-1, *(postfix+i-1));
    k++;
  }

  while(!isEmpty(&operators) && strcmp(peek(&operators), "lol")!=0){
    //printf("Emptying operators stack!\n");
    op_ptr = peek(&operators);
    strcpy(*(postfix+i), op_ptr);
    pop(&operators);
    i++;
  }
  //printf("%d\n", i);
  strcpy(*(postfix+i), "EXPRESSION_END");
}

float evaluatePostfix(char** postfix){

    Stack s;
    initializeStack(&s, 128);

    int i=0;
    float a;
    float b;
    while(strcmp(*(postfix+i), "EXPRESSION_END")!=0){
       switch(*(*(postfix+i))){
         case '+':
          a=atof(pop(&s));
          b=atof(pop(&s));
          push(&s, toString(b+a));
          break;
         case '-':
          if(*(*(postfix+i)+1)=='\0'){
            a=atof(pop(&s));
            b=atof(pop(&s));
            push(&s, toString(b-a));
          }
          else{
            push(&s, *(postfix+i));
          }
          break;
         case '*':
          a=atof(pop(&s));
          b=atof(pop(&s));
          push(&s, toString(b*a));
          break;
         case '/':
          a=atof(pop(&s));
          b=atof(pop(&s));
          push(&s, toString(b/a));
          break;
         default: push(&s, *(postfix+i));
       }
       i++;
    }
    return atof(pop(&s));
}

///////////// STRING /////////////

///////////// STACK //////////////

void initializeStack(Stack *s, int size){
    s->arr=(char**)malloc(size*sizeof(char*));
    int i;
    for(i=0; i<size; i++){
        *((s->arr)+i)= (char*)malloc(16*sizeof(char));
    }
    s->size=size;
    s->top=-1;
}

void push(Stack *s, char* value){
    strcpy(s->arr[++(s->top)] ,value);
    // s->arr[++(s->top)]=value;
}

char* pop(Stack *s){
  char* op_ptr = (char*)malloc(16*sizeof(char));
  op_ptr =  s->arr[(s->top)--];
  return op_ptr;
}

char* peek(Stack *s){
  if(isEmpty(s)){
    return '\0';
  }
  char* op_ptr = (char*)malloc(16*sizeof(char));
  op_ptr = s->arr[s->top];
  return op_ptr;
}

int isEmpty(Stack *s){
  if(s->top==-1){
    return 1;
  }
  return 0;
}

///////////// STACK //////////////

int main(){

    int i, c;
    double result;

    Hash* hashMap= (Hash*)malloc(512*sizeof(Hash));
    for(i=0; i<512; i++){
      hashMap[i].isFull=0;
      hashMap[i].variable=(char*)malloc(16*sizeof(char));
      hashMap[i].value=(char*)malloc(16*sizeof(char));
    }
    //initializeHash(&hashMap);

    Stack postfixStack;
    initializeStack(&postfixStack, 128);

    char* variable = (char*)malloc(16*sizeof(char));

    char* command = (char*)malloc(128*sizeof(char));
    char* expression = (char*)malloc(128*sizeof(char));
    char* exp = (char*)malloc(128*sizeof(char));

    char** infix= (char**)malloc(128*sizeof(char*));
    for(c=0; c<128; c++){
        *(infix+c)=(char*)malloc(16*sizeof(char));
    }

    char** postfix= (char**)malloc(128*sizeof(char*));
    for(c=0; c<128; c++){
        *(postfix+c)=(char*)malloc(16*sizeof(char));
    }
    while(1){
      printf(">>>");
      command = fgets(command, 127, stdin);
      if(!checkBrackets(command)){
         printf("Illegal Input - Unbalanced Brackets!\n");
         continue;
      }
      if(!checkSpaces(command, command)){
         printf("Illegal Input - Misplaced Spaces!\n");
         continue;
      }
      if(!checkEqual(command)){
         printf("Illegal Input - Missing or too much '=' signs entered!\n!");
         continue;
      }
      extractVariable(command, variable);

      expression = command+strlen(variable)+1;

      if(isAllDigits(expression)){
        result = atof(expression);
        printf("%s = %f\n", variable, result);
        put(hashMap, variable, result);
        continue;
      }

      handleNegatives(expression, exp);

      if(!validateExpression(exp, 0, strlen(exp))){
        printf("Illegal Expression!\n");
        continue;
      }

      if(!tokenizeExpression(exp, infix, hashMap)){
        printf("Illegal Expression!\n");
        continue;
      }

      infixToPostfix(infix, postfix);

      result = evaluatePostfix(postfix);

      put(hashMap, variable, result);
      printf("%s = %s\n", variable, get(hashMap, variable));
    }
    return 1;
}

/// Wrong Validation Cases: \\\


