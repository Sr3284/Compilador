/* ----------------------------------------------------------------------
Protótipo do parser - LL(1)

Produções até o momento:

S    -> P
P    -> id P'
		void P'
P'   -> id FPS B
FPS  -> eparen FP FPS' dparen	<<<< precisa arrumar essa produção
		eparen dparen
FPS' -> pvirg FP FPS'
		epsilon
FP   -> EP
		FUP
EP   -> var? I dpoint id		<<< esperando confimação do professor
FUP  -> id FUP'
		void FUP'
FUP' -> id FPS
---------------------------------------------------------------------- */

#include <stdio.h>
#include <stdlib.h>
#include "tokenizer.h"

FILE *f;
int lh;		//lookhead

// Verifica se o próximo terminal é t e avança para o próximo token
void verifica(int t) {
	if (lh == COMENT)
		lh = tokenizer();

	if (lh == ERR) {
		printf("Erro Léxico(linha: %d): símbolo inválido \"%s\" encontrado.");
		exit(1);
	}

	if (lh == t) {
		lh = tokenizer();
	} else {
		printf("Erro Sintático(linha: %d): token \"%s\" esperado.\"%s\" encontrado.\n");
		exit(1);
	}
}

void S() {
	P();
}

void P() {
	if (lh == ID) {
		verifica(ID);
		P_();
	} else if (lh == VAZIO) {
		verifica(VAZIO);
		P_();
	}
}

void P_() {
	if (lh == ID) {
		verifica(ID);
		FPS();
		B();
	}
}

void FPS() {

}

/* Inicia o processamento e devolve mensagem se a texto estiver 
sintáticamente correto */
char* parser() {
	lh = tokenizer();
	S();

	if (lh == FIM)
		return("Fim do arquivo alcançado, a sintaxe está correta!");
	else
		return("Fim do arquivo não alcançado.");
}

// Função MAIN
int int main(int argc, char *argv[]) {
	/* code */
	return 0;
}