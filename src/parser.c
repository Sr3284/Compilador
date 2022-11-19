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

/* ----------------------------------------------------------------
	Verifica se o próximo terminal é t e avança para o próximo to-
ken, no caso de erro informa se é léxico ou sintático, no caso de
erro sintático informa a linha do erro, token esperado pela produ-
ção e token encontrado
---------------------------------------------------------------- */
void verifica(int t, const char* f) {

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
		printf("Erro Sintático(linha: %d): token \"%s\" esperado na função %s. (%s): \"%s\" encontrado.\n", linhas, terminais[t], f, terminais[la], lexema);
		exit(1);
	}
}

void S() {
	#ifdef DEBUG
		printf("S: (%s): %s\n", terminais[la], lexema);
	#endif
	P();
}

void P() {
	#ifdef DEBUG
		printf("P: (%s): %s\n", terminais[la], lexema);
	#endif
	F();

	switch (la) {
		case ID:
		case INTEGER:
		case BOLEAN:
		case VAZIO:
			P();
			break;
	}
}

void F() {
	#ifdef DEBUG
		printf("F: (%s): %s\n", terminais[la], lexema);
	#endif
	switch (la) {
		case ID:
			verifica(ID, __FUNCTION__);
			break;
		case INTEGER:
			verifica(INTEGER, __FUNCTION__);
			break;
		case BOLEAN:
			verifica(BOLEAN, __FUNCTION__);
			break;
		case VAZIO:
			verifica(VAZIO, __FUNCTION__);
			break;
	}

	verifica(ID, __FUNCTION__);
	FPS();
	B();
}

void FPS() {
	#ifdef DEBUG
		printf("FPS: (%s): %s\n", terminais[la], lexema);
	#endif
	verifica(EPAREN, __FUNCTION__);
	FP();
	verifica(DPAREN, __FUNCTION__);
}

void FP() {
	#ifdef DEBUG
		printf("FP: (%s): %s\n", terminais[la], lexema);
	#endif
	switch (la) {
		case ID:
			verifica(ID, __FUNCTION__);
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
	#ifdef DEBUG
		printf("FP_: (%s): %s\n", terminais[la], lexema);
	#endif
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
	#ifdef DEBUG
		printf("EP: (%s): %s\n", terminais[la], lexema);
	#endif
	switch (la) {
		case VAR:
			verifica(VAR, __FUNCTION__);
		case VIRG:
			IL();
		case DPOINT:
			verifica(DPOINT, __FUNCTION__);
			switch (la) {
				case ID:
					verifica(ID, __FUNCTION__);
					break;
				case INTEGER:
					verifica(INTEGER, __FUNCTION__);
					break;
				case BOLEAN:
					verifica(BOLEAN, __FUNCTION__);
					break;
				case VAZIO:
					verifica(VAZIO, __FUNCTION__);
					break;
			}
			break;
	}
}

void FUP() {
	#ifdef DEBUG
		printf("FUP: (%s): %s\n", terminais[la], lexema);
	#endif
	switch (la) {
		case ID:
			verifica(ID, __FUNCTION__);

			if (la == PVIRG) {
				FPS();
			}
			break;
		case INTEGER:
			verifica(INTEGER, __FUNCTION__);
			verifica(ID, __FUNCTION__);
			
			if (la == PVIRG) {
				FPS();
			}
			break;
		case BOLEAN:
			verifica(BOLEAN, __FUNCTION__);
			verifica(ID, __FUNCTION__);
			
			if (la == PVIRG) {
				FPS();
			}
			break;
		case VAZIO:
			verifica(VAZIO, __FUNCTION__);
			verifica(ID, __FUNCTION__);
			
			if (la == PVIRG) {
				FPS();
			}
			break;
	}
}

void B() {
	#ifdef DEBUG
		printf("B: (%s): %s\n", terminais[la], lexema);
	#endif
	L();
	TS();
	VS();
	FN();
	BD();
}

void L() {
	#ifdef DEBUG
		printf("L: (%s): %s\n", terminais[la], lexema);
	#endif
	if (la == LABELS) {
		verifica(LABELS, __FUNCTION__);
		IL();
		verifica(PVIRG, __FUNCTION__);
	}
}

void IL() {
	#ifdef DEBUG
		printf("IL: (%s): %s\n", terminais[la], lexema);
	#endif
	switch (la) {
		case ID:
			verifica(ID, __FUNCTION__);

			if (la == VIRG) {
				IL();
			}
			break;
		case VIRG:
			verifica(VIRG, __FUNCTION__);
			verifica(ID, __FUNCTION__);

			if (la == VIRG) {
				IL();
			}
			break;
	}
}

void TS() {
	#ifdef DEBUG
		printf("TS: (%s): %s\n", terminais[la], lexema);
	#endif
	if (la == TYPES) {
		verifica(TYPES, __FUNCTION__);
		TSK();
	}
}

void TSK() {
	#ifdef DEBUG
		printf("TSK: (%s): %s\n", terminais[la], lexema);
	#endif
	verifica(ID, __FUNCTION__);
	verifica(OP_ATRB, __FUNCTION__);
	T();
	verifica(PVIRG, __FUNCTION__);

	if (la == ID) {
		TSK();
	}
}

void T() {
	#ifdef DEBUG
		printf("T: (%s): %s\n", terminais[la], lexema);
	#endif
	switch (la) {
		case ID:
			verifica(ID, __FUNCTION__);
			TK();
			break;
		case INTEGER:
			verifica(INTEGER, __FUNCTION__);
			TK();
			break;
		case BOLEAN:
			verifica(BOLEAN, __FUNCTION__);
			TK();
			break;
	}
}

void TK() {
	#ifdef DEBUG
		printf("TK: (%s): %s\n", terminais[la], lexema);
	#endif
	if (la == EBRACK) {
		verifica(EBRACK, __FUNCTION__);
		verifica(NUM, __FUNCTION__);
		verifica(DBRACK, __FUNCTION__);
		TK();
	}
}

void VS() {
	#ifdef DEBUG
		printf("VS: (%s): %s\n", terminais[la], lexema);
	#endif
	if (la == VARS) {
		verifica(VARS, __FUNCTION__);
		VSK();
	}
}

void VSK() {
	#ifdef DEBUG
		printf("VSK: (%s): %s\n", terminais[la], lexema);
	#endif
	IL();
	verifica(DPOINT, __FUNCTION__);
	T();
	verifica(PVIRG, __FUNCTION__);

	if (la == ID) {
		VSK();
	}
}

void FN() {
	#ifdef DEBUG
		printf("FN: (%s): %s\n", terminais[la], lexema);
	#endif
	if (la == FUNCTIONS) {
		verifica(FUNCTIONS, __FUNCTION__);
		FNK();
	}
}

void FNK() {
	#ifdef DEBUG
		printf("FNK: (%s): %s\n", terminais[la], lexema);
	#endif
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
	#ifdef DEBUG
		printf("BD: (%s): %s\n", terminais[la], lexema);
	#endif
	verifica(ECHAVE, __FUNCTION__);
	BDK();
	verifica(DCHAVE, __FUNCTION__);
}

void BDK() {
	#ifdef DEBUG
		printf("BDK: (%s): %s\n", terminais[la], lexema);
	#endif
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
	#ifdef DEBUG
		printf("ST: (%s): %s\n", terminais[la], lexema);
	#endif
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
		case OP_PLUSPLUS:
		case OP_MINUSMINUS:
			US();
			break;
	}
}

void STO() {
	#ifdef DEBUG
		printf("STO: (%s): %s\n", terminais[la], lexema);
	#endif
	verifica(ID, __FUNCTION__);

	switch(la) {
		case DPOINT:
			verifica(DPOINT, __FUNCTION__);
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
	#ifdef DEBUG
		printf("CP: (%s): %s\n", terminais[la], lexema);
	#endif
	verifica(ECHAVE, __FUNCTION__);
	CPK();
	verifica(DCHAVE, __FUNCTION__);
}

void CPK() {
	#ifdef DEBUG
		printf("CPK: (%s): %s\n", terminais[la], lexema);
	#endif
	switch (la) {
		case ID:
			verifica(ID, __FUNCTION__);

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
				case ID:										// gambiarra que permite declaração de variavel dentro de compound
					DEC();
					CPK();
					break;
				case OP_PLUSPLUS:								// bagunça que aceita ++ variável, favor não mexa
					verifica(OP_PLUSPLUS, __FUNCTION__);
					VK();
					verifica(PVIRG, __FUNCTION__);
					break;
				case OP_MINUSMINUS:								// bagunça que aceita -- variável, favor não mexa
					verifica(OP_MINUSMINUS, __FUNCTION__);
					VK();
					verifica(PVIRG, __FUNCTION__);
					break;
			}
			break;
		case READ:
			verifica(READ, __FUNCTION__);
			FCS();
			CPK();
			break;
		case WRITE:
			verifica(WRITE, __FUNCTION__);
			FCS();
			CPK();
			break;
		case INTEGER:		// <--- esses dois também fazem parte da gambiarra
		case BOLEAN:		// <----|
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

void DEC() {
	#ifdef DEBUG
		printf("DEC: (%s): %s\n", terminais[la], lexema);
	#endif
	verifica(ID, __FUNCTION__);
	verifica(PVIRG, __FUNCTION__);
}

void FCS() {
	#ifdef DEBUG
		printf("FCS: (%s): %s\n", terminais[la], lexema);
	#endif
	FC();
	verifica(PVIRG, __FUNCTION__);
}

void FC() {
	#ifdef DEBUG
		printf("FC: (%s): %s\n", terminais[la], lexema);
	#endif
	verifica(EPAREN, __FUNCTION__);
	EL();
	verifica(DPAREN, __FUNCTION__);
}

void AS() {
	#ifdef DEBUG
		printf("AS: (%s): %s\n", terminais[la], lexema);
	#endif
	switch (la) {
		case EBRACK:
			VK();
		case OP_ATRB:
			verifica(OP_ATRB, __FUNCTION__);

			switch (la) {
				case TRU:
					verifica(TRU, __FUNCTION__);
					break;
				case FALS:
					verifica(FALS, __FUNCTION__);
					break;
				default:
					EX();
			}
			verifica(PVIRG, __FUNCTION__);
	}
}

void VK() {
	#ifdef DEBUG
		printf("VK: (%s): %s\n", terminais[la], lexema);
	#endif
	if (la == EBRACK) {	
		verifica(EBRACK, __FUNCTION__);
		EX();
		verifica(DBRACK, __FUNCTION__);
		VK();
	}
}

void US() {
	#ifdef DEBUG
		printf("US: (%s): %s\n", terminais[la], lexema);
	#endif
	switch (la) {
		case OP_PLUSPLUS:
			verifica(OP_PLUSPLUS, __FUNCTION__);
			verifica(ID, __FUNCTION__);
			VK();
			verifica(PVIRG, __FUNCTION__);
			break;
		case OP_MINUSMINUS:
			verifica(OP_MINUSMINUS, __FUNCTION__);
			verifica(ID, __FUNCTION__);
			VK();
			verifica(PVIRG, __FUNCTION__);
			break;
		case INTEGER:
			verifica(INTEGER, __FUNCTION__);
			DEC();
			break;
		case BOLEAN:
			verifica(BOLEAN, __FUNCTION__);
			DEC();
			break;
		case GOTO:
			verifica(GOTO, __FUNCTION__);
			verifica(ID, __FUNCTION__);
			verifica(PVIRG, __FUNCTION__);
			break;
		case RETURN:
			verifica(RETURN, __FUNCTION__);
			REO();
			verifica(PVIRG, __FUNCTION__);
			break;
		case IF:
			verifica(IF, __FUNCTION__);
			verifica(EPAREN, __FUNCTION__);
			EX();
			verifica(DPAREN, __FUNCTION__);
			CP();

			if (la == ELSE) {
				verifica(ELSE, __FUNCTION__);
				CP();
			}
			break;
		case WHILE:
			verifica(WHILE, __FUNCTION__);
			verifica(EPAREN, __FUNCTION__);
			EX();
			verifica(DPAREN, __FUNCTION__);
			CP();
			break;
		case PVIRG:
			verifica(PVIRG, __FUNCTION__);
			break;
	}
}

void REO() {
	#ifdef DEBUG
		printf("REO: (%s): %s\n", terminais[la], lexema);
	#endif
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
	#ifdef DEBUG
		printf("EL: (%s): %s\n", terminais[la], lexema);
	#endif
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
	#ifdef DEBUG
		printf("ELK: (%s): %s\n", terminais[la], lexema);
	#endif
	if (la == VIRG) {	
		verifica(VIRG, __FUNCTION__);
		EX();
		ELK();
	}
}

void EX() {
	#ifdef DEBUG
		printf("EX: (%s): %s\n", terminais[la], lexema);
	#endif
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
	#ifdef DEBUG
		printf("SE: (%s): %s\n", terminais[la], lexema);
	#endif
	switch (la) {
		case OP_PLUS:
			verifica(OP_PLUS, __FUNCTION__);
			break;
		case OP_MINUS:
			verifica(OP_PLUS, __FUNCTION__);
			break;
	}

	TR();
	SEK();
}

void SEK() {
	#ifdef DEBUG
		printf("SEK: (%s): %s\n", terminais[la], lexema);
	#endif
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
	#ifdef DEBUG
		printf("TR: (%s): %s\n", terminais[la], lexema);
	#endif
	FAC();
	TRK();
}

void TRK() {
	#ifdef DEBUG
		printf("TRK: (%s): %s\n", terminais[la], lexema);
	#endif
	switch (la) {
		case OP_MULT:
		case OP_DIV:
		case OP_AND:
			MO();
			FAC();

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
	#ifdef DEBUG
		printf("FAC: (%s): %s\n", terminais[la], lexema);
	#endif
	switch (la) {
		case ID:
			verifica(ID, __FUNCTION__);

			switch (la) {
				case EBRACK:
					VK();

					switch (la) {
						case OP_PLUSPLUS:
							verifica(OP_PLUSPLUS, __FUNCTION__);
							break;
						case OP_MINUSMINUS:
							verifica(OP_MINUSMINUS, __FUNCTION__);
							break;
					}
					break;
				case EPAREN:
					FC();
					break;
			}
			break;
		case OP_PLUSPLUS:
			verifica(OP_PLUSPLUS, __FUNCTION__);

			switch (la) {
				case NUM:
					verifica(NUM, __FUNCTION__);
					break;
				case ID:
					verifica(ID, __FUNCTION__);
					VK();
					break;
			}
			break;
		case OP_MINUSMINUS:
			verifica(OP_MINUSMINUS, __FUNCTION__);

			switch (la) {
				case NUM:
					verifica(NUM, __FUNCTION__);
					break;
				case ID:
					verifica(ID, __FUNCTION__);
					VK();
					break;
			}
			break;
		case NUM:
			verifica(NUM, __FUNCTION__);

			switch (la) {
				case OP_PLUSPLUS:
					verifica(OP_PLUSPLUS, __FUNCTION__);
					break;
				case OP_MINUSMINUS:
					verifica(OP_MINUSMINUS, __FUNCTION__);
					break;
			}
			break;
		case EPAREN:
			verifica(EPAREN, __FUNCTION__);
			EX();
			verifica(DPAREN, __FUNCTION__);
			break;
		case OP_NOT:
			verifica(OP_NOT, __FUNCTION__);
			FAC();
			break;
	}
}

/*void N() {
	#ifdef DEBUG
		printf("N: (%s): %s\n", terminais[la], lexema);
	#endif
	switch (la) {
		case OP_PLUSPLUS:
			verifica(OP_PLUSPLUS, __FUNCTION__);
			verifica(NUM, __FUNCTION__);
			break;
		case OP_MINUSMINUS:
			verifica(OP_MINUSMINUS, __FUNCTION__);
			verifica(NUM, __FUNCTION__);
			break;
		case NUM:
			verifica(NUM, __FUNCTION__);

			switch (la) {
				case OP_PLUSPLUS:
					verifica(OP_PLUSPLUS, __FUNCTION__);
					break;
				case OP_MINUSMINUS:
					verifica(OP_MINUSMINUS, __FUNCTION__);
					break;
			}
			break;
	}
}*/

void AO() {
	#ifdef DEBUG
		printf("AO: (%s): %s\n", terminais[la], lexema);
	#endif
	switch (la) {
		case OP_PLUS:
			verifica(OP_PLUS, __FUNCTION__);
			break;
		case OP_MINUS:
			verifica(OP_MINUS, __FUNCTION__);
			break;
		case OP_OR:
			verifica(OP_OR, __FUNCTION__);
			break;
	}
}

void MO() {
	#ifdef DEBUG
		printf("MO: (%s): %s\n", terminais[la], lexema);
	#endif
	switch (la) {
		case OP_MULT:
			verifica(OP_MULT, __FUNCTION__);
			break;
		case OP_DIV:
			verifica(OP_DIV, __FUNCTION__);
			break;
		case OP_AND:
			verifica(OP_AND, __FUNCTION__);
			break;
	}
}

void RO() {
	#ifdef DEBUG
		printf("RO: (%s): %s\n", terminais[la], lexema);
	#endif
	switch (la) {
		case OP_EQUAL:
			verifica(OP_EQUAL, __FUNCTION__);
			break;
		case OP_DIFF:
			verifica(OP_DIFF, __FUNCTION__);
			break;
		case OP_GREAT:
			verifica(OP_GREAT, __FUNCTION__);
			break;
		case OP_GEQ:
			verifica(OP_GEQ, __FUNCTION__);
			break;
		case OP_LEQ:
			verifica(OP_LEQ, __FUNCTION__);
			break;
		case OP_LESS:
			verifica(OP_LESS, __FUNCTION__);
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

	printf("%s\n", parser());

	fclose(f);

	return 0;
}