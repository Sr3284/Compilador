#include <stdio.h>
#include <stdlib.h>
#include "tokenizer.h"

#define ID_TAM 15

FILE *f;
char **tSimbolo;
int lastSimb = 0,
	currLimit = 10;

int main(int argc, char *argv[]) {

	int i,
		teste;

	if (argc < 2) {
		printf("\nUso: tokenizer <arquivo>\n");
		return 1;
	}

	f = fopen(argv[1], "r");

	if (!f) {
		printf("\nErro ao abrir o arquivo %s\n", argv[1]);
		return 1;
	}

	tSimbolo = malloc(currLimit * sizeof(char *));
	for (i = 0; i < currLimit; i++) {
		tSimbolo[i] = malloc((ID_TAM) * sizeof(char));
	}

	do {
		teste = tokenizer();
		if (teste == ERR) {
			printf("Simbolo inválido: < linha %d, %s >\n", linhas, lexema);
		} else {
			printf("< linha %d, Token: %s, %s >\n", linhas, terminais[teste], lexema);
		}

		if(lastSimb == (currLimit - 1)) {
			currLimit *= 2;
			tSimbolo = realloc(tSimbolo, currLimit * sizeof(char *));

			for (i = (lastSimb + 1); i < currLimit; i++) {
				tSimbolo[i] = malloc((ID_TAM) * sizeof(char));
			}
		}

	} while(teste != FIM);

	printf("\nTabela de simbolos\n");

	for (i = 0; i < currLimit; i++) {
		printf("ID_%d : %s\n", i, tSimbolo[i]);
		free(tSimbolo[i]);
	}
	free(tSimbolo);
	fclose(f);

	return 0;
}