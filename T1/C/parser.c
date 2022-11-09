/* ----------------------------------------------------------------------
	Implementação do parser recursivo
---------------------------------------------------------------------- */

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
	printf("S\n");
	P();
}

void P() {
	if (la == ID) {
		printf("P: la == ID\n");
		verifica(ID);
		verifica(ID);
		FPS();
		B();
	} else if (la == VAZIO) {
		printf("P: la == VAZIO\n");
		verifica(VAZIO);
		verifica(ID);
		FPS();
		B();
	}
}

void FPS() {
	printf("FPS\n");
	verifica(EPAREN);
	FPS_();
}

void FPS_() {
	if (la == DPAREN) {
		printf("FPS_: DPAREN\n");
		verifica(DPAREN);
	} else {
		printf("FPS_: else\n");
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
		IL();
		verifica(DPOINT);
		verifica(ID);
	} else if (la == DPOINT) {
		verifica(DPOINT);
		verifica(ID);
	} else if (la == VIRG) {
		IL();
		verifica(DPOINT);
		verifica(ID);
	}
}

void EPO() {
	verifica(VAR);
}

void FUP() {
	if (la == VAZIO) {
		verifica(VAZIO);
		verifica(ID);
		FPS();
	} else if (la == ID) {
		verifica(ID);
		FPS();
	}
}

void B() {
	LO();
	TSO();
	VSO();
	FNO();
	BD();
}

void LO() {
	if (la == LABELS) {
		L();
	}
}

void L() {
		verifica(LABELS);
		IL();
		verifica(PVIRG);
}

void IL() {
	if (la == ID) {				// Foi invocado por L() ou VSK ou por EP() com la == VAR
		verifica(ID);
		ILK();
	} else if (la == VIRG) {	//Foi invocado por EP() onde la == ID
		ILK();
	}
}

void ILK() {
	if (la == VIRG) {
		verifica(VIRG);
		verifica(ID);
		ILK();
	}
}

void TSO() {
	if (la == TYPES) {
		TS();
	}
}

void TS() {
	verifica(TYPES);
	TSK();
} 

void TSK() {
	verifica(ID);
	verifica(OP_ATRB);
	T();
	verifica(PVIRG);
	TSK_();
}

void TSK_() {
	if (la == ID) {
		TSK();
	}
}

void T() {
	verifica(ID);
	TK();
}

void TK() {
	if (la == EBRACK) {
		verifica(EBRACK);
		verifica(NUM);
		verifica(DBRACK);
	}
}

void VSO() {
	if (la == VARS) {
		VS();
	}
}

void VS() {
	verifica(VARS);
	VSK();
}

void VSK() {
	IL();
	verifica(DPOINT);
	T();
	verifica(PVIRG);
	VSK_();
}

void VSK_() {
	if (la == ID) {
		VSK();
	}
}

void FNO() {
	if (la == FUNCTIONS) {
		FN();
	}
}

void FN() {
	verifica(FUNCTIONS);
	FNK();
}

void FNK() {
	P();
	FNK_();
}

void FNK_() {
	if (la == ID || la == VAZIO) {
		FNK();
	}
}

void BD() {
	verifica(ECHAVE);
	BDK();
	verifica(DCHAVE);
}

void BDK() {
	if (la == ID || la == EBRACK || la == GOTO || 
		la == RETURN ||	la == IF || la == WHILE || la == PVIRG) {
		ST();
		BDK();
	}
}

void ST() {
	if (la == ID) {		// <- outra dor de cabeça
		verifica(ID);

		if (la == DPOINT) {
			STO();
		}

		if (la == EBRACK || la == OP_ATRB || la == EPAREN) {
			US();
		}
	} else if (la == GOTO || la == RETURN || la == IF ||
				la == WHILE || la == PVIRG){
		US();
	} else if (la == EBRACK) {
		CP();
	}
}

void STO() {
	if (la == DPOINT) {
		verifica(DPOINT);
	}
}

void US() {
	switch (la) {
		case EBRACK:
		case OP_ATRB:
			AS();
			break;
		case EPAREN:
			FCS();
		case GOTO:
			GO();
			break;
		case RETURN:
			RE();
			break;
		case IF:
			CD();
			break;
		case WHILE:
			RP();
			break;
		case PVIRG:
			ES();
			break;
	}
}

void CP() {
	verifica(EBRACK);
	CPK();
	verifica(DBRACK);
}

void CPK() {
	if (la == ID) {
		verifica(ID);

		US();
		CPK_();
	} else {
		US();
		CPK_();
	}
}

void CPK_() {
	if (la == ID || la == GOTO || la == RETURN ||
		 la == IF || la == WHILE || la == PVIRG) {
		CPK();
	}
}

void AS() {
	if (la == OP_ATRB) {
		verifica(OP_ATRB);
		EX();
		verifica(PVIRG);
	} else {
		V();
		verifica(OP_ATRB);
		EX();
		verifica(PVIRG);
	}
}

void V() {
	VK();
}

void VK() {
	if (la == EBRACK) {
		verifica(EBRACK);
		EX();
		verifica(DBRACK);
		VK();
	}
}

void EX() {
	SE();
	EXO();
}

void EXO() {
	if (la == OP_EQUAL || la == OP_DIFF || la == OP_GREAT ||
		la == OP_GEQ || la == OP_LESS || OP_LEQ) {
		RO();
		SE();
	}
}

void SE() {
	SEO();
	TR();
	SEK();
}

void SEO() {
	if (la == OP_PLUS) {
		verifica(OP_PLUS);
	} else if (la == OP_MINUS) {
		verifica(OP_MINUS);
	}
}

void SEK() {
	if (la == OP_PLUS || la == OP_MINUS || la == OP_AND) {
		AO();
		TR();
		SEK();
	}
}

void TR() {
	FAC();
	TRK();
}

void FAC() {
	if (la == ID) {
		verifica(ID);

		if (la == EPAREN) {
			FC();
		} else if (la == EBRACK || la == DBRACK || la == DPAREN || la == PVIRG || la == VIRG || la == OP_ATRB ||
					la == OP_MULT || la == OP_DIV || la == OP_AND || la == OP_PLUS || la == OP_MINUS || la == OP_OR ||
					la == OP_EQUAL || la == OP_DIFF || la == OP_GREAT || la == OP_GEQ || la == OP_LESS || la == OP_LEQ) {
			V();
		}
	} else if (la == OP_PLUSPLUS || la == OP_MINUSMINUS || la == NUM) {
		N();
	} else if (la == EPAREN) {
		verifica(EPAREN);
		EX();
		verifica(DPAREN);
	} else if (la == OP_NOT) {
		verifica(OP_NOT);
		FAC();
	}
}

void TRK() {
	if (la == OP_MULT || la == OP_DIV || OP_AND) {
		MO();
		FC();
		TRK();
	}
}

void N() {
	if (la == OP_PLUSPLUS) {
		verifica(OP_PLUSPLUS);
		verifica(NUM);
	} else if (la == OP_MINUSMINUS) {
		verifica(OP_MINUSMINUS);
		verifica(NUM);
	} else if (la == NUM) {
		verifica(NUM);
		NPOS();
	}
}

void NPOS() {
	if (la == OP_PLUSPLUS) {
		verifica(OP_PLUSPLUS);
	} else if (la == OP_MINUSMINUS) {
		verifica(OP_MINUSMINUS);
	}
}

void FC() {
	if (la == EPAREN) {
		verifica(EPAREN);
		EL();
		verifica(DPAREN);
	} else {
		verifica(ID);
		verifica(EPAREN);
		EL();
		verifica(DPAREN);
	}
}

void EL() {
	if (la == OP_PLUS || la == OP_MINUS || la == EPAREN || la == OP_NOT ||
		la == ID || la == OP_PLUSPLUS || la == OP_MINUSMINUS || la == NUM) {
		EX();
		ELK();
	}
}

void ELK() {
	if (la == VIRG) {
		verifica(VIRG);
		EX();
		ELK();
	}
}

void MO() {
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

void AO() {
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

void RO() {
	switch (la) {
		case OP_EQUAL:
			verifica(OP_EQUAL);
			break;
		case OP_DIFF:
			verifica(OP_DIFF);
			break;
		case OP_LESS:
			verifica(OP_LESS);
			break;
		case OP_GREAT:
			verifica(OP_GREAT);
			break;
		case OP_LEQ:
			verifica(OP_LEQ);
			break;
		case OP_GEQ:
			verifica(OP_GEQ);
			break;
	}
}

void FCS() {
	if (la == EPAREN) {
		FC();
		verifica(PVIRG);
	}
}

void CD() {
	verifica(IF);
	verifica(EPAREN);
	EX();
	verifica(DPAREN);
	CP();
	CDO();
}

void CDO() {
	if (la == ELSE) {
		verifica(ELSE);
		CP();
	}
}

void RP() {
	verifica(WHILE);
	verifica(EPAREN);
	EX();
	verifica(DPAREN);
	CP();
}

void ES() {
	verifica(PVIRG);
}

void GO() {
	verifica(GOTO);
	verifica(ID);
	verifica(PVIRG);
}

void RE() {
	verifica(RETURN);
	REO();
	verifica(PVIRG);
}

void REO() {
	if (la == OP_PLUS || la == OP_MINUS || la == EPAREN || la == OP_NOT ||
		la == ID || la == OP_PLUSPLUS || la == OP_MINUSMINUS || la == NUM) {
		EX();
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