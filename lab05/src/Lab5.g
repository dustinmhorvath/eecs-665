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
fragment LETTER : 'a' .. 'z' | 'A' .. 'Z' ;
OCTAL : '0'(OCT)* ;
HEXID : '0x'(HEX)* ;
BINARY : '0b'(BIN)* ;
INTEGER : TENSTART(DEC)* ;
REAL : INTEGER'.'INTEGER ;
ID : LETTER(LETTER | DEC)* ;

// The white space lexer rule. Match any number of white space characters
// and hide the results from the parser.
WS : (' ' | '\t' | '\r' | '\n')+ { $channel=HIDDEN; };

// The decimal value lexer rule. Match one or more decimal digits.
//DECIMAL : DEC+ ;

// The top rule. You should replace this with your own rule definition to
// parse expressions according to the assignment.


top : l = expr { System.out.println( $l.value ); }
  | EOF ;

// Groups low-precedence addition and subtraction, else only higher priority
expr returns [float value] : 
    LPAREN l = expr RPAREN 
      { $value = $l.value ; }
  | l = mulExpr (PLUS r = mulExpr)+
      { $value = $l.value + $r.value; }
  | l = mulExpr (MINUS r = mulExpr)+
      { $value = $l.value - $r.value; }
  | l = mulExpr
      { $value = $l.value; }
   ;

// Groups mid-precedence mult and division, else only higher priority
mulExpr returns [float value] :
    l = powExpr (TIMES r = powExpr)+
      { $value = $l.value * $r.value; }
  | l = powExpr (DIVIDE r = powExpr)+
      { $value = $l.value / $r.value; }
  | l = powExpr
      { $value = $l.value ; }
   ;

// Groups moderately-high precedence exponentials, else value or single fxn
powExpr returns [float value] :
    l = atomic POWER r = atomic
      { $value = (float)Math.pow($l.value, $r.value ) ; }
  | l = atomic
      { $value = $l.value ; }
   ;

// Handles atomic functions and values
atomic returns [float value] :
    COS r = digit {$value = (float)Math.cos($r.value); }
  | TAN r = digit {$value = (float)Math.tan($r.value); }
  | SIN r = digit {$value = (float)Math.sin($r.value); }
  | LOG r = digit {$value = (float)Math.log($r.value); }
  | l = digit { $value = $l.value; } 
  ;

// Reads all manner of input values, converts them to base 10 float
digit returns [float value] : 
    REAL { $value = (float)Float.parseFloat( $REAL.getText() ); }
  | HEXID { 
        $value = (float)Integer.parseInt( ($HEXID.getText()).substring(2), 16 ); 
      }
  | OCTAL { 
        $value = (float)Integer.parseInt( ($OCTAL.getText()).substring(1), 8 ); 
      }
  | BINARY { 
        $value = (float)Integer.parseInt( ($BINARY.getText()).substring(2), 2 ); 
      }
  | INTEGER { $value = (float)Integer.parseInt( $INTEGER.getText(), 10 ); }
  ;
