/***********************************************************
*        calculator.c - Provides Calculator Services       *
*                         *  *  *  *                       *
* Coding:                                                  *
*   LSXCommand - Visigoth (Shaheen Gandhi in real life)    *
*                e-mail: sgandhi@andrew.cmu.edu            *
*   Original LSCommand - limpid                            *
*                         *  *  *  *                       *
* Last Update:  July 16, 1999  2:00 AM                     *
*                         *  *  *  *                       *
* Copyright (c) 1999 Shaheen Gandhi                        *
* Ported from Visual Basic code by SoftCircuits, Inc.      *
***********************************************************/

#ifndef LSXCOMMANDCLOCK_EXPORTS

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
* int bintodec()                                           *
*                         *  *  *  *                       *
*    Arguments:                                            *
*                                                          *
*    - char *val                                           *
*      Pointer to the string to analyze                    *
*                         *  *  *  *                       *
* Converts a binary value stored in a string to a decimal  *
***********************************************************/

int bintodec(char *val)
{
  int i, total=0;

  if(val) {
    i = strlen(val) - 1;

    while(*val) {
      if(*val == '1')
        total += (int)pow(2, i);
      --i;
      ++val;
    }
  }
  return total;
}


/***********************************************************
* int bintodec()                                           *
*                         *  *  *  *                       *
*    Arguments:                                            *
*                                                          *
*    - char *val                                           *
*      Pointer to the string to analyze                    *
*                         *  *  *  *                       *
* Converts a binary value stored in a string to a decimal  *
***********************************************************/

int hextodec(char *val)
{
  int i, digit, total=0;
  char ch[] = "01";

  if(val) {
    i = strlen(val) - 1;

    while(*val) {
      switch(*val) {
      case '1':
      case '2':
      case '3':
      case '4':
      case '5':
      case '6':
      case '7':
      case '8':
      case '9':
        digit = (int)(*val) - 48;
        total += (int)(pow(16, i) * digit);
        break;

      case 'A':
      case 'B':
      case 'C':
      case 'D':
      case 'E':
      case 'F':
        digit = (int)(*val) - 55;
        total += (int)(pow(16, i) * digit);
        break;

      case 'a':
      case 'b':
      case 'c':
      case 'd':
      case 'e':
      case 'f':
        digit = (int)(*val) - 87;
        total += (int)(pow(16, i) * digit);
      }
      --i;
      ++val;
    }
  }

  return total;
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
  else if(!strcmp(expr, UNARY_NEG) || !strcmp(expr, UNARY_BIN) || !strcmp(expr, UNARY_DEC) || !strcmp(expr, UNARY_HEX))
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
    case 'd':
    case 'b':
    case 'h':
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
      //} else if(nCurState == STATE_UNARYOP) {
        //return -1;
      } else {
        if(*expr == '-') {
          StackPush(&stack, UNARY_NEG, &nStackSize);
          nCurState = STATE_UNARYOP;
        } else if(*expr == 'd') {
          nCurState = STATE_UNARYOP;
        } else if(*expr == 'b') {
          nCurState = STATE_UNARYOP;
          StackPush(&stack, UNARY_BIN, &nStackSize);
        } else if(*expr == 'h') {
          nCurState = STATE_UNARYOP;
          StackPush(&stack, UNARY_HEX, &nStackSize);
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
  BOOL conversion = FALSE;

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
      } else if(!strcmp(p, UNARY_BIN)) {
        StackPop(&stack, temp, &nStackSize, FALSE);
        sprintf(num, "%d", bintodec(temp));
        StackPush(&stack, num, &nStackSize);
      } else if(!strcmp(p, UNARY_HEX)) {
        StackPop(&stack, temp, &nStackSize, FALSE);
        sprintf(num, "%d", hextodec(temp));
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
* BOOL DoConversions()                                     *
*                         *  *  *  *                       *
*    Arguments:                                            *
*                                                          *
*    - char *expr                                          *
*      Pointer to the string to analyze                    *
*    - char *buf                                           *
*      Pointer to the buffer to save the converted         *
*      expression.                                         *
*                         *  *  *  *                       *
* Analyzes the string pointed to by expr and builds a      *
* converted expression based upon it.                      *
***********************************************************/

BOOL DoConversions(char *expr, char *buf)
{
  int temp;
  char *p, num[1024];

  if(expr) {
    p = strtok(expr, "\t ");
    if(p) {
      if(!stricmp(p, "db")) {
        p = strtok(NULL, "\t ");
        strcpy(buf, "=");
        while(p) {
          temp = atoi(p);
          _itoa(temp, num, 2);
          strcat(buf, "b");
          strcat(buf, num);
          strcat(buf, " ");
          p = strtok(NULL, "\t ");
        }
        return TRUE;
      } else if(!stricmp(p, "dh")) {
        p = strtok(NULL, "\t ");
        strcpy(buf, "=");
        while(p) {
          temp = atoi(p);
          _itoa(temp, num, 16);
          strcat(buf, "h");
          strcat(buf, num);
          strcat(buf, " ");
          p = strtok(NULL, "\t ");
        }
        return TRUE;
      } else if(!stricmp(p, "hd")) {
        p = strtok(NULL, "\t ");
        strcpy(buf, "=");
        while(p) {
          temp = hextodec(p);
          _itoa(temp, num, 10);
          strcat(buf, num);
          strcat(buf, " ");
          p = strtok(NULL, "\t ");
        }
        return TRUE;
      } else if(!stricmp(p, "hb")) {
        p = strtok(NULL, "\t ");
        strcpy(buf, "=");
        while(p) {
          temp = hextodec(p);
          _itoa(temp, num, 2);
          strcat(buf, "b");
          strcat(buf, num);
          strcat(buf, " ");
          p = strtok(NULL, "\t ");
        }
        return TRUE;
      } else if(!stricmp(p, "bh")) {
        p = strtok(NULL, "\t ");
        strcpy(buf, "=");
        while(p) {
          temp = bintodec(p);
          _itoa(temp, num, 16);
          strcat(buf, "h");
          strcat(buf, num);
          strcat(buf, " ");
          p = strtok(NULL, "\t ");
        }
        return TRUE;
      } else if(!stricmp(p, "bd")) {
        p = strtok(NULL, "\t ");
        strcpy(buf, "=");
        while(p) {
          temp = bintodec(p);
          _itoa(temp, num, 10);
          strcat(buf, num);
          strcat(buf, " ");
          p = strtok(NULL, "\t ");
        }
        return TRUE;
      }
    }
  }

  return FALSE;
}


/***********************************************************
* double Evaluate()                                        *
*                         *  *  *  *                       *
*    Arguments:                                            *
*                                                          *
*    - char *expr                                          *
*      Pointer to the expression to evaluate               *
*    - int *error                                          *
*      Pointer to an integer with which to capture errors  *
*    - char *replacement                                   *
*      Should we need to replace the expression with       *
*      another, we will use this string.                   *
*    - size_t replace_size                                 *
*      The size of the replacement string                  *
*                         *  *  *  *                       *
* Calculates the value of an expression by first           *
* converting to postfix notation, then calculating it.     *
***********************************************************/

double Evaluate(char *expr, int *error, char *replacement, size_t replace_size)
{
  char buf[1024];
  int errPos;
  double retval;

  *error = 0;

  if(expr) {
    // Check for all occurences of number conversion first.
    // See if possible to use as a multi-letter operator.
    if(!DoConversions(expr, buf)) {
      if(!(cs->RPNCalculator)) {
        errPos = InfixToPostfix(expr, buf);
        if(errPos && !(cs->NoWarnOnError)) {
          MessageBox(NULL, "There was an error while parsing the expression.  Please check it and try again.", "LSXCommand Expression Evaluator", MB_OK | MB_ICONERROR | MB_APPLMODAL);
          *error = ERROR_POSTFIX_CONVERSION;
          return 0;
        }
      }
      retval = DoEvaluate(buf);
      if(replacement && replace_size > 0) {
        sprintf(buf, "=%f", retval);
        strcpy(replacement, "");
        strncat(replacement, buf, replace_size);
      }
      return retval;
    } else {
      if(replacement && replace_size > 0)
        strncpy(replacement, buf, replace_size);
      *error = ERROR_CONVERSIONS_DONE;
      return 0;
    }
  } else {
    *error = ERROR_NULL_EXPRESSION;
    return 0;
  }
}

#endif //LSXCOMMANDCLOCK_EXPORTS