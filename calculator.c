/***********************************************************
*        calculator.c - Provides Calculator Services       *
*                         *  *  *  *                       *
* Coding:                                                  *
*   LSXCommand - Visigoth (Shaheen Gandhi in real life)    *
*                e-mail: sgandhi@andrew.cmu.edu            *
*   Original LSCommand - limpid                            *
*                         *  *  *  *                       *
* Last Update:  June 2, 1999  2:30 AM                      *
*                         *  *  *  *                       *
* Copyright (c) 1999 Shaheen Gandhi                        *
* Ported from Visual Basic code by SoftCircuits, Inc.      *
***********************************************************/

#include <windows.h>
#include <stdio.h>
#include <math.h>
#include "lsxcommand.h"


/***********************************************************
* void StackPop()                                          *
*                         *  *  *  *                       *
*    Arguments:                                            *
*                                                          *
*    - struct History **stack                              *
*      Pointer to a pointer to an element of the list of   *
*      stack elements                                      *
*    - char *buf                                           *
*      Pointer to the string to capture the pop            *
*    - int *nStackSize                                     *
*      Pointer to where the number of elements is stored.  *
*    - int noremove                                        *
*      Remove the last element in the stack, or not.       *
*                         *  *  *  *                       *
* Pops a value from the stack and saves it in buf.         *
***********************************************************/

void StackPop(struct History **stack, char *buf, int *nStackSize, int noremove)
{
  HistoryMoveToTail(stack);
  strcpy(buf, (*stack)->path);
  if(!noremove)
    HistoryRemoveEntry(stack, nStackSize);
}


/***********************************************************
* void StackPush()                                         *
*                         *  *  *  *                       *
*    Arguments:                                            *
*                                                          *
*    - struct History **stack                              *
*      Pointer to a pointer to an element of the list of   *
*      stack elements                                      *
*    - char *szVal                                         *
*      Pointer to the string to push onto the stack        *
*    - int *nStackSize                                     *
*      Pointer to where the number of elements is stored.  *
*                         *  *  *  *                       *
* Pushes a value onto the stack.                           *
***********************************************************/

void StackPush(struct History **stack, char *szVal, int *nStackSize)
{
  HistoryAdd(stack, szVal, nStackSize);
  HistoryMoveToTail(stack);
}


/***********************************************************
* int GetPrecedence()                                      *
*                         *  *  *  *                       *
*    Arguments:                                            *
*                                                          *
*    - char *expr                                          *
*      Pointer to the string to compare                    *
*                         *  *  *  *                       *
* Finds the precedence of the operator pointed to by expr  *
***********************************************************/

int GetPrecedence(char *expr)
{
  if(expr[0] == '+' || expr[0] == '-')
    return 1;
  else if(expr[0] == '*' || expr[0] == '/' || expr[0] == '\\')
    return 2;
  else if(expr[0] == '^')
    return 3;
  else if(!strcmp(expr, UNARY_NEG))
    return 4;
  else return 0;
}


/***********************************************************
* int InfixToPostfix()                                     *
*                         *  *  *  *                       *
*    Arguments:                                            *
*                                                          *
*    - char *expr                                          *
*      Pointer to the string to analyze                    *
*    - char *buf                                           *
*      Pointer to the buffer to save the postfix           *
*      expression.                                         *
*                         *  *  *  *                       *
* Analyzes the string pointed to by expr and builds a      *
* postfix expression based on it.                          *
***********************************************************/

int InfixToPostfix(char *expr, char *buf)
{
  int i = 1, nCurState = STATE_NONE, nParenCount = 0, nStackSize = 0;
  char ch[2], temp[130];
  BOOL DecPoint;
  struct History *stack = NULL;

  strcpy(buf, "");
  strcpy(temp, "");
  ch[1] = '\0';

  while(*expr) {
    
    switch(*expr) {
    case '(':
      if(!(nCurState == STATE_OPERAND)) {
        if(nCurState == STATE_UNARYOP)
          nCurState = STATE_OPERATOR;

        ch[0] = *expr;
        StackPush(&stack, ch, &nStackSize);
        ++nParenCount;
      } else return -1;
      break;

    case ')':
      if(nCurState == STATE_OPERAND && nParenCount) {
        StackPop(&stack, temp, &nStackSize, FALSE);
        while(!(*temp == '(')) {
          strcat(buf, temp);
          strcat(buf, " ");
          StackPop(&stack, temp, &nStackSize, FALSE);
        }
        --nParenCount;
      } else return -1;
      break;

    case '+':
    case '-':
    case '*':
    case '/':
    case '\\':
    case '^':
      if(nCurState == STATE_OPERAND) {
        while(nStackSize) {
          ch[0] = *expr;
          StackPop(&stack, temp, &nStackSize, TRUE);
          if(GetPrecedence(temp) < GetPrecedence(ch)) {
            break;
          }
          StackPop(&stack, temp, &nStackSize, FALSE);
          strcat(buf, temp);
          strcat(buf, " ");
        }
        ch[0] = *expr;
        StackPush(&stack, ch, &nStackSize);
        nCurState = STATE_OPERATOR;
      } else if(nCurState == STATE_UNARYOP) {
        return -1;
      } else {
        if(*expr == '-') {
          StackPush(&stack, UNARY_NEG, &nStackSize);
          nCurState = STATE_UNARYOP;
        } else if(*expr == '+') {
          nCurState = STATE_UNARYOP;
        }
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
    case '.':
      if(!(nCurState == STATE_OPERAND)) {
        strcpy(temp, "");
        DecPoint = FALSE;
        while(*expr >= '0' && *expr <= '9' || *expr == '.') {
          if(*expr == '.') {
            if(DecPoint)
              return -1;
            else
              DecPoint = TRUE;
          }
          ch[0] = *expr;
          strcat(temp, ch);
          ++expr;
          if(!(*expr))
            break;
        }
        --expr;
        if(strcmp(temp, ".")) {
          strcat(buf, temp);
          strcat(buf, " ");
          nCurState = STATE_OPERAND;
        } else return -1;
      } else return -1;
      break;

    case ' ':
      break;
    }

    ++expr;
  }

  if(*expr)
    return -1;

  if(!(nCurState == STATE_OPERATOR || nCurState == STATE_UNARYOP) && !nParenCount) {
    while(nStackSize) {
      StackPop(&stack, temp, &nStackSize, FALSE);
      strcat(buf, temp);
      strcat(buf, " ");
    }

    HistoryRemoveAll(&stack, &nStackSize);
    return 0;
  } else return -1;
}


/***********************************************************
* double DoEvaluate()                                      *
*                         *  *  *  *                       *
*    Arguments:                                            *
*                                                          *
*    - char *expr                                          *
*      Pointer to the postfix expression to analyze        *
*                         *  *  *  *                       *
* Calculates the value of an expression by postfix         *
* analysis.                                                *
***********************************************************/

double DoEvaluate(char *expr)
{
  int nStackSize = 0;
  struct History *stack = NULL;
  char temp[512], *p, num[128];
  double op1, op2;

  p = strtok(expr, " ");
  while(p) {
    if(!(atof(p) == 0.0))
      StackPush(&stack, p, &nStackSize);
    else {
      if(*p == '+') {
        StackPop(&stack, temp, &nStackSize, FALSE);
        op1 = atof(temp);
        StackPop(&stack, temp, &nStackSize, FALSE);
        op2 = atof(temp);
        sprintf(num, "%f", op1 + op2);
        StackPush(&stack, num, &nStackSize);
      } else if(*p == '-') {
        StackPop(&stack, temp, &nStackSize, FALSE);
        op1 = atof(temp);
        StackPop(&stack, temp, &nStackSize, FALSE);
        op2 = atof(temp);
        sprintf(num, "%f", op2 - op1);
        StackPush(&stack, num, &nStackSize);
      } else if(*p == '*') {
        StackPop(&stack, temp, &nStackSize, FALSE);
        op1 = atof(temp);
        StackPop(&stack, temp, &nStackSize, FALSE);
        op2 = atof(temp);
        sprintf(num, "%f", op1 * op2);
        StackPush(&stack, num, &nStackSize);
      } else if(*p == '/' || *p == '\\') {
        StackPop(&stack, temp, &nStackSize, FALSE);
        op1 = atof(temp);
        StackPop(&stack, temp, &nStackSize, FALSE);
        op2 = atof(temp);
        sprintf(num, "%f", op2 / op1);
        StackPush(&stack, num, &nStackSize);
      } else if(*p == '^') {
        StackPop(&stack, temp, &nStackSize, FALSE);
        op1 = atof(temp);
        StackPop(&stack, temp, &nStackSize, FALSE);
        op2 = atof(temp);
        sprintf(num, "%f", pow(op2, op1));
        StackPush(&stack, num, &nStackSize);
      } else if(!strcmp(p, UNARY_NEG)) {
        StackPop(&stack, temp, &nStackSize, FALSE);
        op1 = atof(temp);
        sprintf(num, "%f", -1*op1);
        StackPush(&stack, num, &nStackSize);
      }
    }
    p = strtok(NULL, " ");
  }

  if(nStackSize) {
    StackPop(&stack, temp, &nStackSize, FALSE);
    return atof(temp);
  } else return 0;
}


/***********************************************************
* double Evaluate()                                        *
*                         *  *  *  *                       *
*    Arguments:                                            *
*                                                          *
*    - char *expr                                          *
*      Pointer to the expression to evaluate               *
*                         *  *  *  *                       *
* Calculates the value of an expression by first           *
* converting to postfix notation, then calculating it.     *
***********************************************************/

double Evaluate(char *expr, struct CommandSettings *cs, BOOL *error)
{
  char buf[1024];
  int errPos;

  if(expr) {
    errPos = InfixToPostfix(expr, buf);
    if(errPos && !(cs->NoWarnOnError)) {
      MessageBox(NULL, "There was an error while parsing the expression.  Please check it and try again.", "LSXCommand Expression Evaluator", MB_OK | MB_ICONERROR | MB_APPLMODAL);
      *error = TRUE;
      return 0;
    }
    return DoEvaluate(buf);
  } else {
    errPos = -1;
    *error = TRUE;
    return 0;
  }
}