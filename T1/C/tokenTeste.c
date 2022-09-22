#include <stdio.h>
#include <stdlib.h>
#include "tokenizer.h"

FILE *f;

int main(int argc, char *argv[]) {

	int teste;

	if (argc < 2) {
		printf("\nUso: tokenizer <arquivo>\n");
		return 1;
	}

	f = fopen(argv[1], "r");

	if (!f) {
		printf("\nErro ao abrir o arquivo %s\n", argv[1]);
		return 1;
	}

	do {
		teste = tokenizer();
		if (teste == ERR) {
			printf("Simbolo inválido: < linha %d, %s >\n", linhas, lexema);
		} else {
			printf("< linha %d, Token: %s, %s >\n", linhas, terminais[teste], lexema);
		}
	} while(teste != FIM);

	fclose(f);

	return 0;
}