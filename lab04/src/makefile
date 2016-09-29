all: scanner

scanner: scanner.lex.o
	gcc -o $@ $+

%.o: %.c
	gcc -c -o $@ $<

%.lex.c: %.l
	lex -o $@ $<



clean:
	rm -f *~ *.o scanner
