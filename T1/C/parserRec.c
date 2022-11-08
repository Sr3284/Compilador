/* ----------------------------------------------------------------------
	Implementação do parser recursivo
---------------------------------------------------------------------- */

#include <stdio.h>
#include <stdlib.h>
#include "tokenizer.h"

#define ID_TAM 15

FILE *f;
int la;		//lookahead

// Verifica se o próximo terminal é t e avança para o próximo token
void verifica(int t) {
	
	if (la == COMENT)
		la = tokenizer();

	if (la == ERR) {
		printf("Erro Léxico(linha: %d): símbolo inválido \"%s\" encontrado.", linhas, lexema);
		exit(1);
	}

	if (la == t) {
		la = tokenizer();
	} else {
		printf("Erro Sintático(linha: %d): token \"%s\" esperado.\"%s\" encontrado.\n", linhas, terminais[t], lexema);
		exit(1);
	}
}

void S() {
	P();
}

void P() {
	if (la == ID) {
		verifica(ID);
		verifica(ID);
		FPS();
		B();
	} else if (la == VAZIO) {
		verifica(VAZIO);
		verifica(ID);
		FPS();
		B();
	}
}

void FPS() {
	if (la == EPAREN) {
		verifica(EPAREN);
		FPS_();
	}
}

void FPS_() {
	if (la == DPAREN) {
		verifica(DPAREN);
	} else {
		if(la == VAR || la == VAZIO || la == ID) {
			FP();
			FPK();
			verifica(DPAREN);
		}
	}
}

void FP() {		// <- dor de cabeça provavelmente aqui
	if (la == ID) {
		verifica(ID);

		if (la == VIRG || la == DPOINT) {
			EP();
		}

		if (la == ID) {
			FUP();
		}
	} else if (la == VAR) {
		EP();
	} else	if (la == VAZIO) {
		FUP();
	}
}

void FPK() {
	if (la == VIRG) {
		verifica(VIRG);
		FP();
		FPK();
	}
}

void EP() {
	if (la == VAR) {
		EPO();
	}


}

/* Inicia o processamento e devolve mensagem se a texto estiver 
sintáticamente correto */
char* parser() {

	la = tokenizer();

	S();

	if (la == FIM) {
		return("Fim do arquivo alcançado, a sintaxe está correta!");
	} else {
		return("Fim do arquivo não alcançado.");
	}
}

// Função MAIN
int int main(int argc, char *argv[]) {

	if (argc < 2) {
		printf("Uso: ./parser <arquivo>\n");
	}

	f = fopen(argv[1], "r");

	if (!f) {
		printf("Erro na abertura do arquivo!\n");
		return 1;
	}

	parser();

	fclose(f);

	return 0;
}