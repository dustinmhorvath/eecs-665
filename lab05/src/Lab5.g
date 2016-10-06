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

expr returns [float value] : 
    LPAREN l = expr RPAREN 
      { $value = $l.value ; }
  | l = multiplyingExpression (PLUS r = multiplyingExpression)+
      { $value = $l.value + $r.value; }
  | l = multiplyingExpression (MINUS r = multiplyingExpression)+
      { $value = $l.value - $r.value; }
  | l = multiplyingExpression
      { $value = $l.value; }
   ;

multiplyingExpression returns [float value] :
    l = powExpression (TIMES r = powExpression)+
      { $value = $l.value * $r.value; }
  | l = powExpression (DIVIDE r = powExpression)+
      { $value = $l.value / $r.value; }
  | l = powExpression
      { $value = $l.value ; }
   ;

powExpression returns [float value] :
    l = atom POWER r = atom
      { $value = (float)Math.pow($l.value, $r.value ) ; }
  | l = atom
      { $value = $l.value ; }
   ;

atom returns [float value] :
  | l = func { $value = $l.value ; }
   ;

func returns [float value] :
    COS r = digit {$value = (float)Math.cos($r.value); }
  | TAN r = digit {$value = (float)Math.tan($r.value); }
  | SIN r = digit {$value = (float)Math.sin($r.value); }
  | LOG r = digit {$value = (float)Math.log($r.value); }
  | l = digit { $value = $l.value; } 
  ;

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
