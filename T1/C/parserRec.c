/* ----------------------------------------------------------------------
	Implementação do parser recursivo
---------------------------------------------------------------------- */

#include <stdio.h>
#include <stdlib.h>
#include "tokenizer.h"

FILE *f;
int la;		//lookahead

// Verifica se o próximo terminal é t e avança para o próximo token
void verifica(int t) {
	if (la == COMENT)
		la = tokenizer();

	if (la == ERR) {
		printf("Erro Léxico(linha: %d): símbolo inválido \"%s\" encontrado.",);
		exit(1);
	}

	if (la == t) {
		la = tokenizer();
	} else {
		printf("Erro Sintático(linha: %d): token \"%s\" esperado.\"%s\" encontrado.\n",);
		exit(1);
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
			FKP();
			verifica(DPAREN);
		}
	}
}

void FP() {
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
	} else	if (la == VAZIO) {}
}

// Função MAIN
int int main(int argc, char *argv[]) {
	/* code */
	return 0;
}