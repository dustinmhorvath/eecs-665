// The name of the grammar. The name here needs to match the name of the
// file, including capitalization.
grammar Lab5;

// Define literals, keywords or operators, here as tokens.
tokens {
  PLUS = '+';
  MINUS = '-';
  COS = 'cos' ;
  TAN = 'tan' ;
  SIN = 'sin' ;
  LOG = 'log' ;
  LN  = 'ln'  ;

  LPAREN = '(' ;
  RPAREN = ')' ;
  PLUS   = '+' ;
  MINUS  = '-' ;
  TIMES  = '*' ;
  DIVIDE = '/' ;
  POWER  = '^' ;
}

// Written in the target language. The header section can be
// used to import any Java classes that may be required.
@header {
}

// A main function to the parser. This function will setup the
// parsers input stream and execute the rule named "top".
@members {
    public static void main(String[] args) throws Exception {
        Lab5Lexer lex = new Lab5Lexer(new ANTLRInputStream(System.in));
       	CommonTokenStream tokens = new CommonTokenStream(lex);
        Lab5Parser parser = new Lab5Parser(tokens);

        try {
            parser.top();
        } catch (RecognitionException e)  {
            e.printStackTrace();
        }
    }
}

funcname
  : COS
  | TAN
  | SIN
  | LOG
  | LN
  ;


// Some example lexer fragments. These fragments don't produce any
// tokens themselves but can be used inside of other lexer rules.
fragment BIN: '0' .. '1';
fragment OCT: '0' .. '7';
fragment DEC: '0' .. '9';
fragment TENSTART: '1' .. '9';
fragment HEX: ('0' .. '9' | 'A' .. 'F' | 'a' .. 'f');
fragment DIGIT : '0' .. '9';
fragment LETTER : 'a' .. 'z' | 'A' .. 'Z' ;
OCTAL : '0'(OCT)* ;
HEXID : '0x'(HEX)* ;
INTEGER : DIGIT+ ;
REAL : INTEGER'.'INTEGER ;
ID : LETTER(LETTER | DIGIT)* ;

// The white space lexer rule. Match any number of white space characters
// and hide the results from the parser.
WS : (' ' | '\t' | '\r' | '\n')+ { $channel=HIDDEN; };

// The decimal value lexer rule. Match one or more decimal digits.
//DECIMAL : DEC+ ;

// The top rule. You should replace this with your own rule definition to
// parse expressions according to the assignment.


top : arith
//  | subExpr EOF
//  | mulExpr EOF
//  | divExpr EOF
  | EOF
  ;

arith : term { System.out.println( $term.value ); } ;
term returns [float value] : l = digit { $value = $l.value; } (
    ( PLUS r = digit { $value += $r.value; } )* 
  | ( MINUS r = digit { $value -= $r.value; } )* 
  | ( TIMES r = digit { $value *= $r.value; } )* 
  | ( DIVIDE r = digit { $value /= $r.value; } )* 
  | ( POWER r = digit { $value = (float)Math.pow($value, $r.value); } )* 

  );



digit returns [float value] : INTEGER { $value = Integer.parseInt( $INTEGER.getText(), 10 ); } ;


