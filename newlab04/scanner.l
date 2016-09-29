%{
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "vars.h"


//Define the tokens that you want to return from your lex as follows
/*
#define ID 256
#define INTVAL 257
#define PREPROC 258
#define PUNC 259
#define PRIM 260
*/

int list_type;
%}

%option noyywrap nounput yylineno
ID [a-zA-Z][a-zA-Z0-9_]*
LPAREN "("
RPAREN ")"
LCBRACKET "{"
RCBRACKET "}"
LBRACKET "["
RBRACKET "]"
COMMA ","
SEMICOLON ";"
INTVAL [0-9]+
DBLVAL [0-9]+"."[0-9]+
FLTVAL [0-9]+"."[0-9]+"f"
CHARVAL "\'"[0-9a-zA-Z]+"\'"
STRVAL "\""[0-9a-zA-Z]+"\""
PREPROC ^#.*
WHITE [ \n\t\r]+
VOID  "void"  
CHAR  "char"
SHORT "short" 
INT "int"
LONG  "long"  
FLOAT "float"
DOUBLE  "double"
RETURN  "return" 
DO  "do"
WHILE "while" 
FOR "for"
SWITCH  "switch"
CASE  "case"
DEFAULT "default"
IF  "if"
ELSE  "else"  
CONTINUE  "continue"
BREAK "break"
GOTO  "goto"
UNSIGNED  "unsigned"
TYPEDEF "typedef"
STRUCT  "struct"
UNION "union"
CONST "const"
STATIC  "static"
EXTERN  "extern" 
AUTO  "auto"
REGISTER  "register"
SIZEOF  "sizeof"
EQ  "=="
NE  "!=" 
GE  ">="
LE  "<="  
GT  ">"
LT  "<"
ADD "+"
SUB "-"
MUL "*"
DIV "/" 
MOD "%" 
OR  "\|\|"
AND "\&\&"  
BITOR "|" 
BITAND  "&"
BITXOR  "^" 
NOT "!" 
COM "~"
LSH "<<"  
RSH ">>"  
SET "="
SETADD  "+="  
SETSUB  "-="  
SETMUL  "*="
SETDIV  "/="  
SETMOD  "%="  
SETOR "|="
SETAND  "&="  
SETXOR  "^="  
SETLSH  "<<="
SETRSH  ">>="    

%x COMMENT

%%
"//" {
/****************************************
* Matching with Single line comments in our 
* C code
******************************************/
BEGIN COMMENT;
}

<COMMENT>[\n](\r)? {
/****************************************
* A new line indicates end of a single 
* line comment in C
****************************************/

BEGIN 0;
}

<COMMENT>[ \t] {
/****************************************
* Lexer ignores the comments section
* by Ignoring anything inside a comment
****************************************/
}



<COMMENT>. {
}

{WHITE} {
// remove whitespace
}





{PREPROC} {
printf("PREPROC\n");
return PREPROC;
}



{VOID} {
int i = 0;
for(i = 0; i < yyleng; i++){
  yytext[i] = toupper(yytext[i]);
}
printf("%s\n", yytext);
return VOID;
}
{CHAR} {
int i = 0;
for(i = 0; i < yyleng; i++){
  yytext[i] = toupper(yytext[i]);
}
printf("%s\n", yytext);
return CHAR;
}
{SHORT} {
int i = 0;
for(i = 0; i < yyleng; i++){
  yytext[i] = toupper(yytext[i]);
}
printf("%s\n", yytext);
return SHORT;
}
{INT} {
int i = 0;
for(i = 0; i < yyleng; i++){
  yytext[i] = toupper(yytext[i]);
}
printf("%s\n", yytext);
return INT;
}
{LONG} {
int i = 0;
for(i = 0; i < yyleng; i++){
  yytext[i] = toupper(yytext[i]);
}
printf("%s\n", yytext);
return LONG;
}
{FLOAT} {
int i = 0;
for(i = 0; i < yyleng; i++){
  yytext[i] = toupper(yytext[i]);
}
printf("%s\n", yytext);
return FLOAT;
}
{DOUBLE} {
int i = 0;
for(i = 0; i < yyleng; i++){
  yytext[i] = toupper(yytext[i]);
}
printf("%s\n", yytext);
return DOUBLE;
}




{LPAREN} {
printf("%s\n", yytext);
return LPAREN;
}
{RPAREN} {
printf("%s\n", yytext);
return RPAREN;
}
{RCBRACKET} {
printf("%s\n", yytext);
return RCBRACKET;
}
{LCBRACKET} {
printf("%s\n", yytext);
return LCBRACKET;
}
{LBRACKET} {
printf("%s\n", yytext);
return LBRACKET;
}
{RBRACKET} {
printf("%s\n", yytext);
return RBRACKET;
}
{COMMA} {
printf("%s\n", yytext);
return COMMA;
}
{SEMICOLON} {
printf("%s\n", yytext);
return SEMICOLON;
}

{EQ} {
printf("%s\n", yytext);
return EQ;
}
{NE} {
printf("%s\n", yytext);
return NE;
}
{GE} {
printf("%s\n", yytext);
return GE;
}
{LE} {
printf("%s\n", yytext);
return LE;
}
{GT} {
printf("%s\n", yytext);
return GT;
}
{LT} {
printf("%s\n", yytext);
return LT;
}
{ADD} {
printf("%s\n", yytext);
return ADD;
}
{SUB} {
printf("%s\n", yytext);
return SUB;
}
{MUL} {
printf("%s\n", yytext);
return MUL;
}
{DIV} {
printf("%s\n", yytext);
return DIV;
}
{MOD} {
printf("%s\n", yytext);
return MOD;
}
{OR} {
printf("%s\n", yytext);
return OR;
}
{AND} {
printf("%s\n", yytext);
return AND;
}
{BITOR} {
printf("%s\n", yytext);
return BITOR;
}
{BITAND} {
printf("%s\n", yytext);
return BITAND;
}
{BITXOR} {
printf("%s\n", yytext);
return BITXOR;
}
{NOT} {
printf("%s\n", yytext);
return NOT;
}
{COM} {
printf("%s\n", yytext);
return COM;
}
{LSH} {
printf("%s\n", yytext);
return LSH;
}
{RSH} {
printf("%s\n", yytext);
return RSH;
}
{SET} {
printf("%s\n", yytext);
return SET;
}
{SETADD} {
printf("%s\n", yytext);
return SETADD;
}
{SETSUB} {
printf("%s\n", yytext);
return SETSUB;
}
{SETMUL} {
printf("%s\n", yytext);
return SETMUL;
}
{SETDIV} {
printf("%s\n", yytext);
return SETDIV;
}
{SETMOD} {
printf("%s\n", yytext);
return SETMOD;
}
{SETOR} {
printf("%s\n", yytext);
return SETOR;
}
{SETAND} {
printf("%s\n", yytext);
return SETAND;
}
{SETXOR} {
printf("%s\n", yytext);
return SETXOR;
}
{SETLSH} {
printf("%s\n", yytext);
return SETLSH;
}
{SETRSH} {
printf("%s\n", yytext);
return SETRSH;
}



{RETURN} {
printf("%s\n", yytext);
return RETURN;
}
{DO} {
printf("%s\n", yytext);
return DO;
}
{WHILE} {
printf("%s\n", yytext);
return WHILE;
}
{FOR} {
printf("%s\n", yytext);
return FOR;
}
{SWITCH} {
printf("%s\n", yytext);
return SWITCH;
}
{CASE} {
printf("%s\n", yytext);
return CASE;
}
{DEFAULT} {
printf("%s\n", yytext);
return DEFAULT;
}
{IF} {
printf("%s\n", yytext);
return IF;
}
{ELSE} {
printf("%s\n", yytext);
return ELSE;
}
{CONTINUE} {
printf("%s\n", yytext);
return CONTINUE;
}
{BREAK} {
printf("%s\n", yytext);
return BREAK;
}
{GOTO} {
printf("%s\n", yytext);
return GOTO;
}



{UNSIGNED} {
int i = 0;
for(i = 0; i < yyleng; i++){
  yytext[i] = toupper(yytext[i]);
}
printf("%s\n", yytext);
return UNSIGNED;
}
{TYPEDEF} {
int i = 0;
for(i = 0; i < yyleng; i++){
  yytext[i] = toupper(yytext[i]);
}
printf("%s\n", yytext);
return TYPEDEF;
}
{STRUCT} {
int i = 0;
for(i = 0; i < yyleng; i++){
  yytext[i] = toupper(yytext[i]);
}
printf("%s\n", yytext);
return STRUCT;
}
{UNION} {
int i = 0;
for(i = 0; i < yyleng; i++){
  yytext[i] = toupper(yytext[i]);
}
printf("%s\n", yytext);
return UNION;
}
{CONST} {
int i = 0;
for(i = 0; i < yyleng; i++){
  yytext[i] = toupper(yytext[i]);
}
printf("%s\n", yytext);
return CONST;
}
{STATIC} {
int i = 0;
for(i = 0; i < yyleng; i++){
  yytext[i] = toupper(yytext[i]);
}
printf("%s\n", yytext);
return STATIC;
}
{EXTERN} {
int i = 0;
for(i = 0; i < yyleng; i++){
  yytext[i] = toupper(yytext[i]);
}
printf("%s\n", yytext);
return EXTERN;
}
{AUTO} {
int i = 0;
for(i = 0; i < yyleng; i++){
  yytext[i] = toupper(yytext[i]);
}
printf("%s\n", yytext);
return AUTO;
}
{REGISTER} {
int i = 0;
for(i = 0; i < yyleng; i++){
  yytext[i] = toupper(yytext[i]);
}
printf("%s\n", yytext);
return REGISTER;
}
{SIZEOF} {
int i = 0;
for(i = 0; i < yyleng; i++){
  yytext[i] = toupper(yytext[i]);
}
printf("%s\n", yytext);
return SIZEOF;
}




{ID} {
/**************************************
* If an identifier is matched, then 
* print ID and return a token ID
**************************************/

printf("ID\n");
return ID;
}






%%


int main(int argc, const char* argv[])
{
  /*Make a call to yylex() recursively. Whenever yylex() is called, it scans tokens from the 
    global input file yyin (which defaults to stdin). It continues until it either reaches an 
    end-of-file (at which point it returns the value 0) or one of its actions executes a return statement.*/


  while(yylex());



return 0;
}
