main: scanner.l
	bison -d parser.y
	flex scanner.l
	gcc lex.yy.c parser.tab.c -o myDAO 

clean:
	rm lex.* parser.tab.* myDAO
	rm -r Model/ DAO/