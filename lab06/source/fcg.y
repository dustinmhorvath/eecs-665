%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern int yylineno;
int yydebug = 0;
char* lastFunction = "";
extern void yyerror( char* );
extern int yylex();
%}

/*********************************************************
 ********************************************************/
%union {
    char* id;
}

%token <id> ID
%token INTVAL
%token FLTVAL
%token DBLVAL
%token STRVAL
%token CHARVAL
%token VOID
%token CHAR
%token SHORT
%token INT
%token LONG
%token FLOAT
%token DOUBLE
%token RETURN
%token IF
%token ELSE
%token WHILE
%token EQ
%token NE
%token GE
%token LE
%token GT
%token LT
%token ADD
%token SUB
%token MUL
%token DIV
%token MOD
%token OR
%token AND
%token BITOR
%token BITAND
%token BITXOR
%token NOT
%token COM
%token LSH
%token RSH
%token SET
%token SETADD
%token SETSUB
%token SETMUL
%token SETDIV
%token SETMOD
%token SETOR
%token SETAND
%token SETXOR
%token SETLSH
%token SETRSH
%token UNSIGNED
%token TYPEDEF
%token STRUCT
%token UNION
%token CONST
%token STATIC
%token EXTERN
%token AUTO
%token REGISTER
%token SIZEOF
%token DO
%token FOR
%token SWITCH
%token CASE
%token DEFAULT
%token CONTINUE
%token BREAK
%token GOTO
%nonassoc IF
%nonassoc ELSE
%nonassoc NOT
%nonassoc COM
%nonassoc MUL
%nonassoc DIV
%nonassoc MOD
%nonassoc ADD
%nonassoc SUB
%nonassoc LSH
%nonassoc RSH
%nonassoc LT
%nonassoc LE
%nonassoc GT
%nonassoc GE
%nonassoc EQ
%nonassoc NE
%nonassoc BITAND
%nonassoc BITXOR
%nonassoc BITOR
%nonassoc AND
%nonassoc OR
%nonassoc SET
%nonassoc SETADD
%nonassoc SETSUB
%nonassoc SETMUL
%nonassoc SETDIV
%nonassoc SETMOD
%nonassoc SETLSH
%nonassoc SETRSH
%nonassoc SETAND
%nonassoc SETXOR
%nonassoc SETOR

%start top

%%

/*********************************************************
 * The top level parsing rule, as set using the %start
 * directive above. You should modify this rule to
 * parse the contents of a file.
 ********************************************************/

top :	/*    */
    | function_declaration top;

function_declaration : function_name '{' function_block '}';

function_name : type ID '(' args ')'
                {
                  printf("%s;\n", $2); lastFunction = $2;
                };

function_block  :
      			    | declaration function_block
    	      		| statement function_block
          			;

param : type ID
			| type MUL ID '[' ']'
			| type MUL ID '[' expr ']'
			| type ID '[' expr ']'
			| type ID '[' ']'
			| type MUL ID
			;

functioncall  : ID '(' listexpr ')'
                {
                  printf("%s -> %s; \n", lastFunction, $1);
                };

statement :
    			| ID SET expr ';'
	    		| MUL ID SET expr ';'
    			| ID'['INTVAL']' SET expr ';'
		    	| RETURN expr ';'
    			| '{' liststatement '}'
		    	| functioncall ';'
    			| IF '(' expr ')' statement ELSE statement
		    	| IF '(' expr ')' statement
    			| WHILE '(' expr ')' statement
		    	;

declaration : type ID ';' 
            | type ID'['INTVAL']' 
            | type MUL ID '['']'
            ;

liststatement : /*    */
              | statement ',' liststatement 
              | statement
              ;

listexpr  :	/*    */
          | expr ',' listexpr	
          | expr
          ;

level :	BITAND
      | BITOR
      | BITXOR
      | level2
      ;

level2  : EQ
        | NE
        | level3
        ;

level3  : EQ
        | NOT EQ
        | level4
        ;

level4  : LT
        | GT
        | LE
        | GE
        | level5
        ;

level5  : LSH
        | RSH
        | level6
        ;

level6  : ADD
        | SUB
        | level7
        ;

level7  : MUL
        | DIV
        | MOD
        ;

type  : VOID
      | INT
      | DOUBLE
      | FLOAT
      | SHORT
      | LONG
      | CHAR
      ;

/*********************************************************
 * An example rule used to parse arguments to a
 * function call. The arguments to a function call
 * can either be nothing, one parameter, or multiple
 * parameters separated by commas.
 ********************************************************/
args  : /* empty rule */
      | expr
      | expr ',' args
      | param
	    | param ',' args
      ;

/*********************************************************
 * An example rule used to parse a single expression.
 * Currently this rule parses only an integer value
 * but you should modify the rule to parse the required
 * expressions.
 ********************************************************/
expr  : INTVAL
      | FLTVAL
      | STRVAL
  	  | CHARVAL
      | expr level expr
      | functioncall
      | ID
   	  | MUL ID
	    | ID '[' INTVAL ']'
      ;

%%

/*********************************************************
 * This method is invoked by the parser whenever an
 * error is encountered during parsing; just print
 * the error to stderr.
 ********************************************************/
void yyerror( char *err ) {
    fprintf( stderr, "at line %d: %s\n", yylineno, err );
}

/*********************************************************
 * This is the main function for the function call
 * graph program. We invoke the parser and return the
 * error/success code generated by it.
 ********************************************************/
int main( int argc, const char *argv[] ) {
    printf( "digraph funcgraph {\n" );
    int res = yyparse();
    printf( "}\n" );

    return res;
}
