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

#define M_PI 3.1415926535
#define M_E exp(1)

/***********************************************************
* struct operat                                            *
*                         *  *  *  *                       *
*    Members:                                              *
*                                                          *
*    - char c                                              *
*      Character for operator                              *
*    - double (*f)(double, double)                         *
*      Pointer to function that calculates the operation   *
*      arguments are passed the order they are typed in    *
*      the experssion                                      *
*    - int plvl                                            *
*      Precedence level for operator. Operators of higher  *
*      precedence are executed before operators of lower.  *
*      Must be >0                                          *
*    - int type                                            *
*      Currently only two types are defined:               *
*          0 - normal operator, takes two arguments (eg +) *
*          1 - post operator, takes one argument (eg !)    *
*                         *  *  *  *                       *
*                                                          *
* Contains information about an operator. All operators    *
* are stored in the ops array below.                       *
***********************************************************/

typedef struct operat
{
    char c;
    double (*f)(double, double);
    int plvl;
    int type;
} operat;


/***********************************************************
* struct func                                              *
*                         *  *  *  *                       *
*    Members:                                              *
*                                                          *
*    - char *name                                          *
*      Pointer to string containing name of function       *
*    - double (*f)(double)                                 *
*      Pointer to function that calculates the function    *
*    - int type                                            *
*      Currently only two types are defined:               *
*          0 - normal function, takes one argument (eg sin)*
*          1 - constant, takes no arguments (eg pi)        *
*                         *  *  *  *                       *
*                                                          *
* Contains information about a function. All functions are *
* stored in the funcs array below.                         *
***********************************************************/

typedef struct func
{
    char *name;
    double (*f)(double);
    int type;
} func;

/***********************************************************
* struct numbase                                           *
*                         *  *  *  *                       *
*    Members:                                              *
*                                                          *
*    - char c                                              *
*      Character used as prefix to indicate number base    *
*    - int base                                            *
*      Numerical base                                      *
*                         *  *  *  *                       *
*                                                          *
* Contains information about a number base. All bases are  *
* stored in the funcs bases array below.                   *
***********************************************************/

typedef struct numbase
{
    char c;
    int base;
} numbase;



/***********************************************************
* BOOL IsNumerical(char c, int base)                       *
*                         *  *  *  *                       *
*    Arguments:                                            *
*                                                          *
*    - char c                                              *
*      number to test if it belongs to numberbase          *
*    - int base                                            *
*      Numerical base                                      *
*                         *  *  *  *                       *
*                                                          *
* Checks whether a character belongs to a numberbase. The  *
* characters '.' and '-' are considered belonging to all   *
* bases.                                                   *
***********************************************************/
BOOL IsNumerical(char c, int base)
{
    if(base <= 10)
		return (c>='0' && c<'0'+base) || c=='.' || c=='-';

    return (c>='0' && c<='9') || (c>='a' && c<'a'+base-10) || (c>='A' && c<'A'+base-10) || c=='.' || c=='-';
}

/***********************************************************
* int NumValue(char c, int base)                           *
*                         *  *  *  *                       *
*    Arguments:                                            *
*                                                          *
*    - char c                                              *
*      number to test if it belongs to numberbase          *
*    - int base                                            *
*      Numerical base that c should belong to              *
*                         *  *  *  *                       *
*                                                          *
* Returns the numerical value of a character. If the       *
* character dosn't belong to that base -1 is returned.     *
***********************************************************/
int NumValue(char c, int base)
{
    int v;

    v = c-'0';
    if(v>=0 && v<10)
	return v<base ? v : -1;

    v = c-'a'+10;
    if(v>=10 && v<36)
	return v<base ? v : -1;

    v = c-'A'+10;
    if(v>=10 && v<36)
	return v<base ? v : -1;

    return -1;
}

/***********************************************************
* char CharNum(int v)                                      *
*                         *  *  *  *                       *
*    Arguments:                                            *
*                                                          *
*    - int v                                               *
*      Integer to convert to ascii                         *
*                         *  *  *  *                       *
*                                                          *
* Converts an integer to a character (integer must fit in  *
* one character)                                           *
***********************************************************/
char CharNum(int v)
{
    if(v<10)
	return '0'+v;

    return 'a'-10+v;
}



/***********************************************************
*           Functions used to calulate expression          *
***********************************************************/
static double ipart(double t1)
{
	double d;
	modf(t1,&d);
	return d;
}

static double add(double t1, double t2)
{
    return t1+t2;
}

static double sub(double t1, double t2)
{
    return t1-t2;
}

static double mul(double t1, double t2)
{
    return t1*t2;
}

static double mydiv(double t1, double t2)
{
    return t1/t2;
}

static double idiv(double t1, double t2)
{
    return ipart(t1/t2);
}

static double factorial(double t1, double t2)
{
    double t,r=1;

    for(t=ipart(t1);t>1;t--)
	r*=t;

    return r;
}

static double pow10(double t1, double t2)
{
    return t1*pow(10,t2);
}

static double pi(double t1)
{
    return M_PI;
}

static double e(double t1)
{
    return M_E;
}

static double rnd(double t1)
{	
	return (double)rand()/RAND_MAX;
}

static double cot(double t1)
{
	return 1/tan(t1);
}

static double acot(double t1)
{
    return M_PI/2-atan(t1);
}

static double fpart(double t1)
{
	double d;
	return modf(t1,&d);
}

static double sign(double t1)
{
	if(t1==0)
		return 0;
	if(t1<0)
		return -1;
	return 1;
}

static double log2(double t1)
{
	return log(t1)/log(2);
}

/***********************************************************
*      Arrays containing operators, functions and bases    *
***********************************************************/
static operat ops[]=
    {{'+', add, 1, 0},
     {'-', sub, 1, 0},
     {'*', mul, 2, 0},
     {'/', mydiv, 2, 0},
     {'\\', idiv, 2, 0},
     {'%', fmod, 2, 0},
     {'^', pow, 4, 0},
     {'!', factorial, 5, 1},
     {'E', pow10, 10, 0},
     {0, NULL, 0, 0}};  // MUST end with this row

static func funcs[]=
    {{"sin",sin,0},
     {"cos",cos,0},
     {"tan",tan,0},
     {"cot",cot,0},
     {"asin",asin,0},
     {"acos",acos,0},
     {"atan",atan,0},
     {"acot",acot,0},
     {"sinh",sinh,0},
     {"cosh",cosh,0},
     {"tanh",tanh,0},
     {"exp",exp,0},
     {"ln",log,0},
     {"log",log,0},
     {"log2",log2,0},
     {"log10",log10,0},
     {"sqrt",sqrt,0},
     {"floor",floor,0},
     {"ceil",ceil,0},
     {"abs",fabs,0},
     {"fpart",fpart,0},
     {"ipart",ipart,0},
     {"sign",sign,0},
     {"rand",rnd,1},
     {"rnd",rnd,1},
     {"pi",pi,1},
     {"e",e,1},
     {NULL,NULL,0}};  // MUST end with this row

static numbase bases[] = {{'d', 10}, {'o', 8}, {'h', 16}, {'b', 2}, {0, 0}}; // MUST end with {0,0}

/***********************************************************
* double GetVal(char *&str, int base)                      *
*                         *  *  *  *                       *
*    Arguments:                                            *
*                                                          *
*    - char *&str                                          *
*      pointer to number to evaluate                       *
*    - int base                                            *
*      Base in which the number is written                 *
*                         *  *  *  *                       *
*                                                          *
* Converts a string to a double. Returns the double and    *
* modifies the str argument to point to first character    *
* not converted.                                           *
***********************************************************/
double GetVal(char *&str, int base)
{
	double s,r,v,m;

	s=1;
	if(*str=='-')
	{
		s=-1;
		str++;
	}
	
	for(r=0;(v=NumValue(*str,base))!=-1;str++)
	{
		r=r*base;
		r+=v;
	}
	
	if(*str=='.')
	{
		str++;
		for(m=1/(double)base;(v=NumValue(*str,base))!=-1;str++,m/=(double)base)
			r+=m*v;
	}
	
	return r*s;
}

/***********************************************************
*            Functions for postfix evaluation              *
***********************************************************/

/***********************************************************
* void spush(double *&stack, int &stackpos,                *
*                                int &stacksize, double r) *
*                         *  *  *  *                       *
*    Arguments:                                            *
*                                                          *
*    - double *&stack                                      *
*      Pointer stack. Null if no stack has been allocated. *
*    - int &stackpos                                       *
*      Position in stack to put the next number            *
*    - int &stacksize                                      *
*      Number of elements allocated for on the stack       *
*    - double r                                            *
*      Number to push onto the stack                       *
*                         *  *  *  *                       *
*                                                          *
* Pushes a value onto the sta pointed to by the 'stack'    *
* argument. Increses the size of the allocated block if    *
* the stack is full                                        *
***********************************************************/
void spush(double *&stack, int &stackpos, int &stacksize, double r)
{
    if(stackpos==stacksize)
    {
		stacksize+=100;
		stack=(double*)realloc(stack,sizeof(double)*stacksize);
    }
    stack[stackpos++]=r;
}

/***********************************************************
* double spop(double *stack, int &stackpos)                *
*                         *  *  *  *                       *
*    Arguments:                                            *
*                                                          *
*    - double *&stack                                      *
*      Pointer stack. Null if no stack has been allocated. *
*    - int &stackpos                                       *
*      Position in stack to put the next number            *
*    - double r                                            *
*      Number to push onto the stack                       *
*                         *  *  *  *                       *
*                                                          *
* Pops the top value of the stack. If the stack is empty   *
* zero is returned.                                        *
***********************************************************/
double spop(double *stack, int &stackpos)
{
    if(stackpos==0)
		return 0;
    return stack[--stackpos];
}

/***********************************************************
* double EvalPF(char *&str)                                *
*                         *  *  *  *                       *
*    Arguments:                                            *
*                                                          *
*    - char *&str                                          *
*      pointer to string containing the expression to      *
*      evaluate.                                           *
*                         *  *  *  *                       *
*                                                          *
* Evaluate an expression written in postfix notation.      *
* Returns the calculated value. Sets the str argument to   *
* NULL if an error occured while evaluating                *
***********************************************************/
double EvalPF(char *&str)
{
    double r;
    int i,j,op,base;

	double *stack=NULL;
	int stackpos=0;
	int stacksize=0;
    
    while(*str==' ')
		str++;
	
    while(*str!=0)
    {
		if(IsNumerical(*str, 10))
			base=10;
		else
		{
			for(i=0;bases[i].c!=0 && !(bases[i].c==*str && IsNumerical(str[1], bases[i].base));i++);
			base=bases[i].base;
			if(base!=0)
				str++;
		}
	
		if(base!=0)
		{
			r=GetVal(str, base);
			spush(stack, stackpos, stacksize, r);
		}
		else
		{
			for(op=0;ops[op].c!=0 && ops[op].c!=*str;op++);
			
			if(ops[op].c!=0)
			{
				if(ops[op].type==0)
				{
					r=spop(stack, stackpos);
					spush(stack, stackpos, stacksize, (ops[op].f)(spop(stack, stackpos),r));
				}
				else
					spush(stack, stackpos, stacksize, (ops[op].f)(spop(stack, stackpos),0));

				str++;
			}
			else
			{
				for(i=0;funcs[i].name!=NULL;i++)
				{
					for(j=0;funcs[i].name[j]==str[j] && funcs[i].name[j]!=0 && str[j]!=0;j++);
					if(funcs[i].name[j]==0 && (str[j]==' ' || str[j]==0))
						break;
				}
			
				if(funcs[i].name!=NULL)
				{
					if(funcs[i].type==0)
						spush(stack, stackpos, stacksize, (funcs[i].f)(spop(stack, stackpos)));
					else
						spush(stack, stackpos, stacksize, (funcs[i].f)(0));
					str+=j;
				}
				else
				{
					free(stack);
					str=NULL;
					return 0;
				}
			}
		}
		while(*str==' ')
			str++;
    }

	r=spop(stack,stackpos);
	free(stack);
	return r;
}

/***********************************************************
*             Functions for base converting                *
***********************************************************/

int MaxDecimals=10;

/***********************************************************
* int BConv(char *&str,char *ostr)                         *
*                         *  *  *  *                       *
*    Arguments:                                            *
*                                                          *
*    - char *&str                                          *
*      pointer to string containing the expression to      *
*      evaluate.                                           *
*    - char *ostr                                          *
*      pointer to string where to put the converted numbers*
*                         *  *  *  *                       *
*                                                          *
* Converts numbers between different bases returns 0 if    *
* the string was not an baseconvert command. Returns 1 on  *
* success and errorcode on error                           *
***********************************************************/
int BConv(char *&str,char *ostr)
{
    int i,fbase,tbase,n;
    double r,m;
    char tbasec;

    while(*str==' ')
		str++;

    if(str[0]==0 || str[1]==0)
		return 0;

    for(i=0;bases[i].c!=*str && bases[i].c!=0;i++);
    fbase=bases[i].base;
    str++;
    for(i=0;bases[i].c!=*str && bases[i].c!=0;i++);
    tbase=bases[i].base;
    tbasec=bases[i].c;
    str++;

    if(fbase==0 || tbase==0)
		return 0;

    while(*str==' ')
		str++;

    while(*str!=0)
    {
		if(tbase!=10)
		{
			*ostr=tbasec;
			ostr++;
		}

		r=GetVal(str, fbase);

		if(r<0)
		{
		    *ostr='-';
			ostr++;
		}
	
		for(m=tbase;m<=r;m*=tbase);
		for(m/=tbase;m>=1;m/=tbase)
		{
			n=(int)(r/m);
			r-=n*m;
			*ostr=CharNum(n);
			ostr++;
		}

		if(r>1E-10)
		{
			*ostr='.';
			ostr++;
			for(r*=tbase,i=0;r>1E-10 && i<MaxDecimals;r*=tbase,i++)
			{
				n=(int)r;
				r-=n;
				*ostr=CharNum(n);
				ostr++;
			}
		}

		if(*str!=' ' && *str!=0)
			return 2;

		*ostr=' ';
		ostr++;

		while(*str==' ')
		    str++;

	}
	*ostr=0;
	ostr++;
	return 1;
}

/***********************************************************
*           Functions evaluating normal expressions        *
***********************************************************/


double Eval(char *&str);
double EvalExp(char *&str, int plvl=0, double r=0);

/***********************************************************
* double EvalExp(char *&str, int plvl, double r)           *
*                         *  *  *  *                       *
*    Arguments:                                            *
*                                                          *
*    - char *&str                                          *
*      pointer to string containing the expression to      *
*      evaluate.                                           *
*    - int plvl                                            *
*      Precedence level to keep above. This arument is set *
*      to zero when evaluating the beginning of an         *
*      expression                                          *
*    - double r                                            *
*      The value of the expression evaluated so far at.    *
*                         *  *  *  *                       *
*                                                          *
* This function and the Eval function calls each other     *
* recursively to evaluate the expression. This function    *
* takes care of operator execution and precedence and      *
* calling the Eval function to evaluate all numbers. See   *
* also description of Eval function.                       *
* When encountering an error the function sets str to NULL *
* and returns                                              *
***********************************************************/
double EvalExp(char *&str, int plvl, double r)
{
    double rn;
    int op,opn;

    if(plvl==0)
	r=Eval(str);
	if(str==NULL) return 0;

    while(*str==' ')
	str++;

    while(*str!=0 && *str!=')')
    {
		for(op=0;ops[op].c!=0 && ops[op].c!=*str;op++);

		if(ops[op].c==*str)
		{
		    if(ops[op].plvl > plvl)
			{
				str++;
				if(ops[op].type==0)
				{
				    rn=Eval(str);
					if(str==NULL) return 0;
		    
					while(*str==' ')
					str++;

					for(opn=0;ops[opn].c!=0 && ops[opn].c!=*str;opn++);
		    
					if(ops[opn].plvl > ops[op].plvl)
					{
						rn=EvalExp(str, ops[op].plvl, rn);
						if(str==NULL) return 0;
					}
		    
					r=(ops[op].f)(r,rn);
				}
				else
					r=(ops[op].f)(r,0);
			}
			else
				return r;
		}
		else
		{
			str=NULL;
			return 0;
		}

		while(*str==' ')
			str++;

    }
    
    return r;
}

/***********************************************************
* double Eval(char *&str)                                  *
*                         *  *  *  *                       *
*    Arguments:                                            *
*                                                          *
*    - char *&str                                          *
*      pointer to string containing the expression to      *
*      evaluate.                                           *
*                         *  *  *  *                       *
*                                                          *
* This function and the EvalExp function calls each other  *
* recursively to evaluate the expression.                  *
* This function main object is to evaluate a number. But if*
* the expression beginns with a parenthesis the expression *
* withing the parenthesis is evaluated by calling EvalExp. *
* The same goes for functions, then the expression within  *
* the functions parenthesis is evaluated by calling EvalExp*
* When encountering an error the function sets str to NULL *
* and returns                                              *
***********************************************************/
double Eval(char *&str)
{
    double r;
    int i,j, base;

    while(*str==' ')
		str++;

    if(IsNumerical(*str, 10))
		base=10;
    else
    {
		for(i=0;bases[i].c!=0 && !(bases[i].c==*str && IsNumerical(str[1], bases[i].base));i++);
		base=bases[i].base;
		if(base!=0)
		    str++;
    }

    if(base!=0)
    {
		r=GetVal(str, base);
    }
	else if(*str=='(')
	{
		str++;
		r=EvalExp(str);
		if(str==NULL) return 0;

		if(*str!=')')
		{
		    str=NULL;
			return 0;
		}
		str++;
    }
	else
	{
		for(i=0;funcs[i].name!=NULL;i++)
		{
			for(j=0;funcs[i].name[j]==str[j] && funcs[i].name[j]!=0 && str[j]!=0;j++);
			if(funcs[i].name[j]==0 && (str[j]=='(' || funcs[i].type==1))
				break;
		}

		if(funcs[i].name!=NULL)
		{
			str+=j;
			if(funcs[i].type==0)
			{
				r=Eval(str);
				if(str==NULL) return 0;
				r=(funcs[i].f)(r);
			}
			else
				r=(funcs[i].f)(0);
		}
		else
		{
			str=NULL;
			return 0;
		}
    }

    return r;
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
extern "C" {
double Evaluate(char *expr, int *error, char *replacement, size_t replace_size, struct CommandSettings *cs)
{
	char buf[1024];
	double retval=0;
	char *str, *ostr;
	int ConvErr,i;

	for(i=0;expr[i]!=0;i++)
	{
		if(expr[i]=='\t')
			expr[i]=' ';
	}

	*error = 0;

	if(expr)
	{
		str = expr;
		ostr = buf;
		if((ConvErr=BConv(str,ostr)) == 0)
		{
			str = expr;
			if(!(cs->RPNCalculator))
				retval = EvalExp(str);
			else
				retval = EvalPF(str);

			if(str==NULL)
			{
				if(!(cs->NoWarnOnError))
					MessageBox(NULL, "There was an error while parsing the expression.  Please check it and try again.", "LSXCommand Expression Evaluator", MB_OK | MB_ICONERROR | MB_APPLMODAL);
				*error = ERROR_EVALUATE;
				return 0;
			}

			if(replacement && replace_size > 0)
			{
				sprintf(buf, "=%f", retval);
				strcpy(replacement, "");
				strncat(replacement, buf, replace_size);
			}
			return retval;
		}
		else
		{
			if(ConvErr!=1)
			{
				if(!(cs->NoWarnOnError))
					MessageBox(NULL, "There was an error while parsing the expression.  Please check it and try again.", "LSXCommand Expression Evaluator", MB_OK | MB_ICONERROR | MB_APPLMODAL);
				*error = ERROR_EVALUATE;
				return 0;
			}
			else
			{
        if(replacement && replace_size > 0) {
          strcpy(replacement, "=");
          strncat(replacement, buf, replace_size - 1);
        }
				*error = ERROR_CONVERSIONS_DONE;
				return 0;
			}
		}
    }
	else
	{
		*error = ERROR_NULL_EXPRESSION;
		return 0;
	}
}
}
#endif //LSXCOMMANDCLOCK_EXPORTS