parser: tokenizer.o parser.o
	gcc tokenizer.o parser.o -g -o parser -ansi -Wall -pedantic
tokenTeste: tokenizer.o tokenTeste.o
	gcc tokenizer.o tokenTeste.o -g -o tokenTest -ansi -Wall -pedantic
parser.o: parser2.h parser2.c
	gcc -c parser2.c -g
tokenizer.o: tokenizer.h tokenizer.c
	gcc -c tokenizer.c -g
tokenTest.o: tokenTeste.c
	gcc -c tokenTeste.c -g

clean:
	rm *.o