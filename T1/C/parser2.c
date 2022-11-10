/* ---------------------------------------------------------------- */
/* parser.c                                                         */
/*                                                                  */
/*   Implementação de uma parser descendente recursivo, faz uso de  */
/* tSimbolo para salvar identificadores únicos.                     */
/*                                                                  */
/* ---------------------------------------------------------------- */

#include <stdio.h>
#include <stdlib.h>
#include "tokenizer.h"
#include "parser.h"

#define ID_TAM 15

FILE *f;
int la;		//lookahead

char **tSimbolo;
int lastSimb = 0,
	posTabela = 0,
	currLimit = 10;

// Verifica se o próximo terminal é t e avança para o próximo token
void verifica(int t) {

	int i;

	if (la == COMENT) {
		la = tokenizer();
	}

	if (la == ERR) {
		printf("Erro Léxico(linha: %d): símbolo inválido \"%s\" encontrado.", linhas, lexema);
		exit(1);
	}

	if (la == t) {
		if (la == ID) {		// Responsável pela tabela de simbolo
			if(lastSimb == (currLimit - 1)) {
				currLimit *= 2;
				tSimbolo = realloc(tSimbolo, currLimit * sizeof(char *));

				for (i = (lastSimb + 1); i < currLimit; i++) {
					tSimbolo[i] = malloc((ID_TAM) * sizeof(char));
				}
			}
		}

		la = tokenizer();
	} else {
		printf("Erro Sintático(linha: %d): token \"%s\" esperado. (%s): \"%s\" encontrado.\n", linhas, terminais[t], terminais[la], lexema);
		exit(1);
	}
}

void S() {
	printf("S: (%s): %s\n", terminais[la], lexema);
	P();
}

void P() {
	printf("P: (%s): %s\n", terminais[la], lexema);
	switch (la) {
		case ID:
			verifica(ID);
			break;
		case INTEGER:
			verifica(INTEGER);
			break;
		case BOLEAN:
			verifica(BOLEAN);
			break;
		case VAZIO:
			verifica(VAZIO);
			break;
	}

	verifica(ID);
	FPS();
	B();
}

void FPS() {
	printf("FPS: (%s): %s\n", terminais[la], lexema);
	verifica(EPAREN);
	FP();
	verifica(DPAREN);
}

void FP() {
	printf("FP: (%s): %s\n", terminais[la], lexema);
	switch (la) {
		case ID:
			verifica(ID);
			FP_();
			break;
		case VAR:
			EP();
			break;
		case INTEGER:
		case BOLEAN:
		case VAZIO:
			FUP();
			break;
	}
}

void FP_() {
	printf("FP_: (%s): %s\n", terminais[la], lexema);
	switch (la) {
		case VIRG:
		case DPOINT:
			EP();
			break;
		case ID:
			FUP();
			break;
	}
}

void EP() {
	printf("EP: (%s): %s\n", terminais[la], lexema);
	switch (la) {
		case VAR:
			verifica(VAR);
		case VIRG:
			IL();
		case DPOINT:
			verifica(DPOINT);
			switch (la) {
				case ID:
					verifica(ID);
					break;
				case INTEGER:
					verifica(INTEGER);
					break;
				case BOLEAN:
					verifica(BOLEAN);
					break;
				case VAZIO:
					verifica(VAZIO);
					break;
			}
			break;
	}
}

void FUP() {
	printf("FUP: (%s): %s\n", terminais[la], lexema);
	switch (la) {
		case ID:
			verifica(ID);
			FPS();
			break;
		case INTEGER:
			verifica(INTEGER);
			verifica(ID);
			FPS();
			break;
		case BOLEAN:
			verifica(BOLEAN);
			verifica(ID);
			FPS();
			break;
		case VAZIO:
			verifica(VAZIO);
			verifica(ID);
			FPS();
			break;
	}
}

void B() {
	printf("B: (%s): %s\n", terminais[la], lexema);
	L();
	TS();
	VS();
	FN();
	BD();
}

void L() {
	printf("L: (%s): %s\n", terminais[la], lexema);
	if (la == LABELS) {
		verifica(LABELS);
		IL();
		verifica(PVIRG);
	}
}

void IL() {
	printf("IL: (%s): %s\n", terminais[la], lexema);
	switch (la) {
		case ID:
			verifica(ID);

			if (la == VIRG) {
				IL();
			}
			break;
		case VIRG:
			verifica(VIRG);
			verifica(ID);

			if (la == VIRG) {
				IL();
			}
			break;
	}
}

void TS() {
	printf("TS: (%s): %s\n", terminais[la], lexema);
	if (la == TYPES) {
		verifica(TYPES);
		TSK();
	}
}

void TSK() {
	printf("TSK: (%s): %s\n", terminais[la], lexema);
	verifica(ID);
	verifica(OP_ATRB);
	T();
	verifica(PVIRG);

	if (la == ID) {
		TSK();
	}
}

void T() {
	printf("T: (%s): %s\n", terminais[la], lexema);
	switch (la) {
		case ID:
			verifica(ID);
			TK();
			break;
		case INTEGER:
			verifica(INTEGER);
			TK();
			break;
		case BOLEAN:
			verifica(BOLEAN);
			TK();
			break;
	}
}

void TK() {
	printf("TK: (%s): %s\n", terminais[la], lexema);
	if (la == EBRACK) {
		verifica(EBRACK);
		verifica(NUM);
		verifica(DBRACK);
		TK();
	}
}

void VS() {
	printf("VS: (%s): %s\n", terminais[la], lexema);
	if (la == VARS) {
		verifica(VARS);
		VSK();
	}
}

void VSK() {
	printf("VSK: (%s): %s\n", terminais[la], lexema);
	IL();
	verifica(DPOINT);
	T();
	verifica(PVIRG);

	if (la == ID) {
		VSK();
	}
}

void FN() {
	printf("FN: (%s): %s\n", terminais[la], lexema);
	if (la == FUNCTIONS) {
		verifica(FUNCTIONS);
		FNK();
	}
}

void FNK() {
	printf("FNK: (%s): %s\n", terminais[la], lexema);
	P();

	switch (la) {
		case ID:
		case INTEGER:
		case BOLEAN:
		case VAZIO:
			FNK();
			break;
	}
}

void BD () {
	printf("BD: (%s): %s\n", terminais[la], lexema);
	verifica(ECHAVE);
	BDK();
	verifica(DCHAVE);
}

void BDK() {
	printf("BDK: (%s): %s\n", terminais[la], lexema);
	switch(la) {
		case ID:
		case EBRACK:
		case GOTO:
		case RETURN:
		case IF:
		case WHILE:
		case PVIRG:
			ST();
			BDK();
			break;
	}
}

void ST() {
	printf("ST: (%s): %s\n", terminais[la], lexema);
	switch (la) {
		case ID:
			STO();
			break;
		case EBRACK:
			CP();
			break;
		case GOTO:
		case RETURN:
		case IF:
		case WHILE:
		case PVIRG:
			US();
			break;
	}
}

void STO() {
	printf("STO: (%s): %s\n", terminais[la], lexema);
	verifica(ID);

	switch(la) {
		case DPOINT:
			verifica(DPOINT);
			US();
			break;
		case EPAREN:
			FCS();
			break;
		case EBRACK:
		case OP_ATRB:
			AS();
			break;
	}
}

void CP() {
	printf("CP: (%s): %s\n", terminais[la], lexema);
	verifica(ECHAVE);
	CPK();
	verifica(DCHAVE);
}

void CPK() {
	printf("CPK: (%s): %s\n", terminais[la], lexema);
	switch (la) {
		case ID:
			verifica(ID);

			switch(la) {
				case EPAREN:
					FCS();
					CPK();
					break;
				case EBRACK:
				case OP_ATRB:
					AS();
					CPK();
					break;
			}
			break;
		case GOTO:
		case RETURN:
		case IF:
		case WHILE:
		case PVIRG:
			US();
			CPK();
			break;
	}
}

void FCS() {
	printf("FCS: (%s): %s\n", terminais[la], lexema);
	FC();
	verifica(PVIRG);
}

void FC() {
	printf("FC: (%s): %s\n", terminais[la], lexema);
	verifica(EPAREN);
	EL();
	verifica(DPAREN);
}

void AS() {
	printf("AS: (%s): %s\n", terminais[la], lexema);
	switch (la) {
		case EBRACK:
			VK();
		case OP_ATRB:
			verifica(OP_ATRB);
			EX();
			verifica(PVIRG);
	}
}

void VK() {
	printf("VK: (%s): %s\n", terminais[la], lexema);
	if (la == EBRACK) {	
		verifica(EBRACK);
		EX();
		verifica(DBRACK);
		VK();
	}
}

void US() {
	printf("US: (%s): %s\n", terminais[la], lexema);
	switch (la) {
		case GOTO:
			verifica(GOTO);
			verifica(ID);
			verifica(PVIRG);
			break;
		case RETURN:
			verifica(RETURN);
			REO();
			verifica(PVIRG)
			break;
		case IF:
			verifica(IF);
			verifica(EPAREN);
			EX();
			verifica(DPAREN);
			CP();

			if (la == ELSE) {
				verifica(ELSE);
				CP();
			}
			break;
		case WHILE:
			verifica(WHILE);
			verifica(EPAREN);
			EX();
			verifica(DPAREN);
			CP();
			break;
		case PVIRG:
			verifica(PVIRG);
			break;
	}
}

void REO() {
	printf("REO: (%s): %s\n", terminais[la], lexema);
	switch (la) {
		case ID:
		case EPAREN:
		case OP_PLUSPLUS:
		case OP_MINUSMINUS:
		case OP_PLUS:
		case OP_MINUS:
		case OP_NOT:
		case NUM:
			EX();
			break;
	}
}

void EL() {
	printf("EL: (%s): %s\n", terminais[la], lexema);
	switch (la) {
		case ID:
		case EPAREN:
		case OP_PLUSPLUS:
		case OP_MINUSMINUS:
		case OP_PLUS:
		case OP_MINUS:
		case OP_NOT:
		case NUM:
			EX();
			ELK();
			break;
	}
}

void ELK() {
	printf("ELK: (%s): %s\n", terminais[la], lexema);
	if (la == VIRG) {	
		verifica(VIRG);
		EX();
		ELK();
	}
}

void EX() {
	printf("EX: (%s): %s\n", terminais[la], lexema);
	SE();

	switch (la) {
		case OP_EQUAL:
		case OP_DIFF:
		case OP_LESS:
		case OP_LEQ:
		case OP_GREAT:
		case OP_GEQ:
			RO();
			SE();
			break;
	}
}

void SE() {
	printf("SE: (%s): %s\n", terminais[la], lexema);
	switch (la) {
		case OP_PLUS:
			verifica(OP_PLUS);
			break;
		case OP_MINUS:
			verifica(OP_PLUS);
			break;
	}

	TR();
	SEK();
}

void SEK() {
	printf("SEK: (%s): %s\n", terminais[la], lexema);
	switch (la) {
		case OP_PLUS:
		case OP_MINUS:
		case OP_OR:
			AO();
			TR();

			switch (la) {
				case OP_PLUS:
				case OP_MINUS:
				case OP_OR:
					SEK();
					break;
			}
			break;
	}
}

void TR() {
	printf("TR: (%s): %s\n", terminais[la], lexema);
	FAC();
	TRK();
}

void TRK() {
	printf("TRK: (%s): %s\n", terminais[la], lexema);
	switch (la) {
		case OP_MULT:
		case OP_DIV:
		case OP_AND:
			MO();
			FC();

			switch (la) {
				case OP_MULT:
				case OP_DIV:
				case OP_AND:
					TRK();
					break;
			}
			break;
	}
}

void FAC() {
	printf("FAC: (%s): %s\n", terminais[la], lexema);
	switch (la) {
		case ID:
			verifica(ID);

			switch (la) {
				case EBRACK:
					VK();
					break;
				case EPAREN:
					FC();
					break;
			}
			break;
		case OP_PLUSPLUS:
		case OP_MINUSMINUS:
		case NUM:
			N();
			break;
		case EPAREN:
			verifica(EPAREN);
			EX();
			verifica(DPAREN);
			break;
		case OP_NOT:
			verifica(OP_NOT);
			FAC();
			break;
	}
}

void N() {
	printf("N: (%s): %s\n", terminais[la], lexema);
	switch (la) {
		case OP_PLUSPLUS:
			verifica(OP_PLUSPLUS);
			verifica(NUM);
			break;
		case OP_MINUSMINUS:
			verifica(OP_MINUSMINUS);
			verifica(NUM);
			break;
		case NUM:
			verifica(NUM);

			switch (la) {
				case OP_PLUSPLUS:
					verifica(OP_PLUSPLUS);
					break;
				case OP_MINUSMINUS:
					verifica(OP_MINUSMINUS);
					break;
			}
			break;
	}
}

void AO() {
	printf("AO: (%s): %s\n", terminais[la], lexema);
	switch (la) {
		case OP_PLUS:
			verifica(OP_PLUS);
			break;
		case OP_MINUS:
			verifica(OP_MINUS);
			break;
		case OP_OR:
			verifica(OP_OR);
			break;
	}
}

void MO() {
	printf("MO: (%s): %s\n", terminais[la], lexema);
	switch (la) {
		case OP_MULT:
			verifica(OP_MULT);
			break;
		case OP_DIV:
			verifica(OP_DIV);
			break;
		case OP_AND:
			verifica(OP_AND);
			break;
	}
}

void RO() {
	printf("RO: (%s): %s\n", terminais[la], lexema);
	switch (la) {
		case OP_EQUAL:
			verifica(OP_EQUAL);
			break;
		case OP_DIFF:
			verifica(OP_DIFF);
			break;
		case OP_GREAT:
			verifica(OP_GREAT);
			break;
		case OP_GEQ:
			verifica(OP_GEQ);
			break;
		case OP_LEQ:
			verifica(OP_LEQ);
			break;
		case OP_LESS:
			verifica(OP_LESS);
			break;
	}
}

/* Inicia o processamento e devolve mensagem se a texto estiver 
sintáticamente correto */
char* parser() {

	int i;

	tSimbolo = malloc(currLimit * sizeof(char *));		// Tabela de simbolos genérica
	for (i = 0; i < currLimit; i++) {
		tSimbolo[i] = malloc((ID_TAM) * sizeof(char));
	}

	la = tokenizer();

	S();

	if (la == FIM) {
		return("Fim do arquivo alcançado, a sintaxe está correta!");
	} else {
		return("Fim do arquivo não alcançado.");
	}
}

// Função MAIN
int main(int argc, char *argv[]) {

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