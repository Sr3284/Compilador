tokenTeste: tokenizer.o tokenTeste.o
	gcc tokenizer.o tokenTeste.o -g -o tokenTest -ansi -Wall -pedantic
tokenizer.o: tokenizer.h tokenizer.c
	gcc -c tokenizer.c -g
tokenTest.o: tokenTeste.c
	gcc -c tokenTeste.c -g

clean:
	rm *.o