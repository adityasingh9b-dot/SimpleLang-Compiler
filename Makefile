all:
	gcc src/main.c src/lexer.c src/parser.c -Iinclude -o compiler
	./compiler > output.asm
	cat output.asm

run: all
	./compiler > output.asm
	cat output.asm

clean:
	rm -f compiler output.asm
