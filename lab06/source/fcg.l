/*
 * Dustin Horvath
 * EECS 665 Lab04
 * Lexical Scanner for C-like language
 *
 */


%{

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <fcg.yacc.h>



#ifndef NDEBUG
#    define DEBUGF(...) printf(__VA_ARGS__)
#else
#    define DEBUGF(...)
#endif



int list_type;
%}

%option       noyywrap nounput yylineno
%x            COMMENT

ID         [a-zA-Z][a-zA-Z0-9_]*
whitesp       [ \t\n\r]
nonwhitesp    [^ \t\n\r]
INTVAL         [0-9]
HEXVAL      [0-9a-fA-F]
OCTVAL      [0-7]

%%



"unsigned" {
  DEBUGF( "UNSIGNED\n" );
  return UNSIGNED;
}

"typedef" {
  DEBUGF( "TYPEDEF\n" );
  return TYPEDEF;
}

"struct" {
  DEBUGF( "STRUCT\n" );
  return STRUCT;
}

"union" {
  DEBUGF( "UNION\n" );
  return UNION;
}

"const" {
  DEBUGF( "CONST\n" );
  return CONST;
}

"static" {
  DEBUGF( "STATIC\n" );
  return STATIC;
}

"extern" {
  DEBUGF( "EXTERN\n" );
  return EXTERN;
}

"auto" {
  DEBUGF( "AUTO\n" );
  return AUTO;
}

"register" {
  DEBUGF( "REGISTER\n" );
  return REGISTER;
}

"sizeof" {
  DEBUGF( "SIZEOF\n" );
  return SIZEOF;
}





"return" {
  DEBUGF( "RETURN\n" );
  return RETURN;
}
"default" {
  DEBUGF( "DEFAULT\n" );
  return DEFAULT;
}
"while" {
  DEBUGF( "WHILE\n" );
  return WHILE;
}
"do" {
  DEBUGF( "DO\n" );
  return DO;
}
"if" {
  DEBUGF( "IF\n" );
  return IF;
}
"else" {
  DEBUGF( "ELSE\n" );
  return ELSE;
}
"for" {
  DEBUGF( "FOR\n" );
  return FOR;
}
"switch" {
  DEBUGF( "SWITCH\n" );
  return SWITCH;
}
"case" {
  DEBUGF( "CASE\n" );
  return CASE;
}
"continue" {
  DEBUGF( "CONTINUE\n" );
  return CONTINUE;
}
"break" {
  DEBUGF( "BREAK\n" );
  return BREAK;
}
"goto" {
  DEBUGF( "GOTO\n" );
  return GOTO;
}


"(" {
  DEBUGF( "(\n" );
  return '(';
}
")" {
  DEBUGF( ")\n" );
  return ')';
}
"[" {
  DEBUGF( "[\n" );
  return '[';
}
"]" {
  DEBUGF( "]\n" );
  return ']';
}
"{" {
  DEBUGF( "{\n" );
  return '{';
}
"}" {
  DEBUGF( "}\n" );
  return '}';
}
"," {
  DEBUGF( ",\n" );
  return ',';
}
";" {
  DEBUGF( ";\n" );
  return ';';
}


"==" {
  DEBUGF( "EQ\n" );
  return EQ;
}
"!=" {
  DEBUGF( "NE\n" );
  return NE;
}
">=" {
  DEBUGF( "GE\n" );
  return GE;
}

"<=" {
  DEBUGF( "LE\n" );
  return LE;
}

">" {
  DEBUGF( "GT\n" );
  return GT;
}
"<" {
  DEBUGF( "LT\n" );
  return LT;
}
"+" {
  DEBUGF( "ADD\n" );
  return ADD;
}
"-" {
  DEBUGF( "SUB\n" );
  return SUB;
}
"*" {
  DEBUGF( "MUL\n" );
  return MUL;
}
"/" {
  DEBUGF( "DIV\n" );
  return DIV;
}
"%" {
  DEBUGF( "MOD\n" );
  return MOD;
}
"||" {
  DEBUGF( "OR\n" );
  return OR;
}
"&&" {
  DEBUGF( "ADD\n" );
  return ADD;
}
"|" {
  DEBUGF( "BITOR\n" );
  return BITOR;
}
"&" {
  DEBUGF( "BITAND\n" );
  return BITAND;
}
"^" {
  DEBUGF( "BITXOR\n" );
  return BITXOR;
}
"!" {
  DEBUGF( "NOT\n" );
  return NOT;
}
"~" {
  DEBUGF( "COM\n" );
  return COM;
}
"<<" {
  DEBUGF( "LSH\n" );
  return LSH;
}
">>" {
  DEBUGF( "RSH\n" );
  return RSH;
}
"=" {
  DEBUGF( "SET\n" );
  return SET;
}
"+=" {
  DEBUGF( "SETADD\n" );
  return SETADD;
}
"-=" {
  DEBUGF( "SETSUB\n" );
  return SETSUB;
}
"*=" {
  DEBUGF( "SETMUL\n" );
  return SETMUL;
}
"/=" {
  DEBUGF( "SETDIV\n" );
  return SETDIV;
}
"%=" {
  DEBUGF( "SETMOD\n" );
  return SETMOD;
}
"|=" {
  DEBUGF( "SETOR\n" );
  return SETOR;
}
"&=" {
  DEBUGF( "SETAND\n" );
  return SETAND;
}
"^=" {
  DEBUGF( "SETXOR\n" );
  return SETXOR;
}
"<<=" {
  DEBUGF( "SETLSH\n" );
  return SETLSH;
}
">>=" {
  DEBUGF( "SETRSH\n" );
  return SETRSH;
}




"void" {
  DEBUGF( "VOID\n" );
  return VOID;
}
"char" {
  DEBUGF( "CHAR\n" );
  return CHAR;
}
"short" {
  DEBUGF( "SHORT\n" );
  return SHORT;
}
"int" {
  DEBUGF( "INT\n" );
  return INT;
}
"long" {
  DEBUGF( "LONG\n" );
  return LONG;
}
"float" {
  DEBUGF( "FLOAT\n" );
  return FLOAT;
}
"double" {
  DEBUGF( "DOUBLE\n" );
  return DOUBLE;
}















"0x"{HEXVAL}+ {
  DEBUGF( "INTVAL\n" );
  return INTVAL;
}
"0"{OCTVAL}*  {
  DEBUGF( "INTVAL\n" );
  return INTVAL;
}
[1-9]{INTVAL}* {
  DEBUGF( "INTVAL\n" );
  return INTVAL;
}








{ID} {
  DEBUGF( "ID\n" ); yylval.id = strdup(yytext);
  return ID;
     }



"//".* {
  /* ignored */
}

"/*" {
  BEGIN COMMENT;
}
<COMMENT>"*/" {
  BEGIN 0;
}
<COMMENT>([\n](\r)?)|. {
  /* ignored */ } 



{whitesp}+ {
  /* ignored */
           }


. {
  fprintf( stderr, "Unexpected input at line %d: '%s'\n", yylineno, yytext );
  exit( 1 );
}
