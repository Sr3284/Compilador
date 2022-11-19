CFLAGS= -D $(DEBUG)
DEBUG= N

parser: tokenizer.o parser.o
	gcc $(CFLAGS) tokenizer.o parser.o -g -o parser -ansi -Wall -pedantic
tokenTeste: tokenizer.o tokenTeste.o
	gcc $(CFLAGS) tokenizer.o tokenTeste.o -g -o tokenTeste -ansi -Wall -pedantic
parser2.o: parser.h parser.c
	gcc $(CFLAGS) -c parser.c -g
tokenizer.o: tokenizer.h tokenizer.c
	gcc $(CFLAGS) -c tokenizer.c -g
tokenTest.o: tokenTeste.c
	gcc $(CFLAGS) -c tokenTeste.c -g

clean:
	rm *.o