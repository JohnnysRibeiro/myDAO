main: scanner.l
	bison -d parser.y
	flex scanner.l
	gcc lex.yy.c parser.tab.c -o salandroid

clean:
		rm lex.* parser.tab.* salandroid
		rm -r Model/ DAO/ 
