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
	printf("S: (%s): %s\n", terminais[la], lexema);
	P();
}

void P() {
	if (la == ID) {
		printf("P: (%s): %s\n", terminais[la], lexema);
		verifica(ID);
		printf("P: (%s): %s\n", terminais[la], lexema);
		verifica(ID);
		FPS();
		B();
	} else if (la == VAZIO) {
		printf("P: (%s): %s\n", terminais[la], lexema);
		verifica(VAZIO);
		printf("P: (%s): %s\n", terminais[la], lexema);
		verifica(ID);
		FPS();
		B();
	}
}

void FPS() {
	printf("FPS: (%s): %s\n", terminais[la], lexema);
	verifica(EPAREN);
	FPS_();
}

void FPS_() {
	if (la == DPAREN) {
		printf("FPS_: (%s): %s\n", terminais[la], lexema);
		verifica(DPAREN);
	} else {
		printf("FPS_:  (%s): %s\n", terminais[la], lexema);
		if(la == VAR || la == VAZIO || la == ID) {
			FP();
			FPK();
			verifica(DPAREN);
		}
	}
}

void FP() {		// <- dor de cabeça provavelmente aqui
	if (la == ID) {
		printf("FP: (%s): %s\n", terminais[la], lexema);
		verifica(ID);
		printf("FP: (%s): %s\n", terminais[la], lexema);

		if (la == VIRG || la == DPOINT) {
			EP();
		}

		if (la == ID) {
			FUP();
		}
	} else if (la == VAR) {
		printf("FP: (%s): %s\n", terminais[la], lexema);
		EP();
	} else	if (la == VAZIO) {
		printf("FP: (%s): %s\n", terminais[la], lexema);
		FUP();
	}
}

void FPK() {
	if (la == VIRG) {
		printf("FPK (%s): %s\n", terminais[la], lexema);
		verifica(VIRG);
		FP();
		FPK();
	}
}

void EP() {
	if (la == VAR) {
		printf("EP: (%s): %s\n", terminais[la], lexema);
		EPO();
		IL();
		verifica(DPOINT);
		verifica(ID);
	} else if (la == DPOINT) {
		printf("EP: (%s): %s\n", terminais[la], lexema);
		verifica(DPOINT);
		verifica(ID);
	} else if (la == VIRG) {
		printf("EP: (%s): %s\n", terminais[la], lexema);
		IL();
		verifica(DPOINT);
		verifica(ID);
	}
}

void EPO() {
	printf("EPO: (%s): %s\n", terminais[la], lexema);
	verifica(VAR);
}

void FUP() {
	if (la == VAZIO) {
		printf("FUP: (%s): %s\n", terminais[la], lexema);
		verifica(VAZIO);
		verifica(ID);
		FPS();
	} else if (la == ID) {
		printf("FUP: (%s): %s\n", terminais[la], lexema);
		verifica(ID);
		FPS();
	}
}

void B() {
	printf("B: (%s): %s\n", terminais[la], lexema);
	LO();
	TSO();
	VSO();
	FNO();
	BD();
}

void LO() {
	printf("LO: (%s): %s\n", terminais[la], lexema);
	if (la == LABELS) {
		L();
	}
}

void L() {
	printf("L: (%s): %s\n", terminais[la], lexema);
	verifica(LABELS);
	IL();
	printf("L: (%s): %s\n", terminais[la], lexema);
	verifica(PVIRG);
}

void IL() {
	if (la == ID) {				// Foi invocado por L() ou VSK ou por EP() com la == VAR
		printf("IL: (%s): %s\n", terminais[la], lexema);
		verifica(ID);
		ILK();
	} else if (la == VIRG) {	//Foi invocado por EP() onde la == ID
		printf("IL: (%s): %s\n", terminais[la], lexema);
		ILK();
	}
}

void ILK() {
	printf("ILK: (%s): %s\n", terminais[la], lexema);
	if (la == VIRG) {
		printf("ILK: (%s): %s\n", terminais[la], lexema);
		verifica(VIRG);
		printf("ILK: (%s): %s\n", terminais[la], lexema);
		verifica(ID);
		ILK();
	}
}

void TSO() {
	printf("TSO: (%s): %s\n", terminais[la], lexema);
	if (la == TYPES) {
		TS();
	}
}

void TS() {
	printf("TS: (%s): %s\n", terminais[la], lexema);
	verifica(TYPES);
	TSK();
} 

void TSK() {
	printf("TSK: (%s): %s\n", terminais[la], lexema);
	verifica(ID);
	printf("TSK: (%s): %s\n", terminais[la], lexema);
	verifica(OP_ATRB);
	T();
	printf("TSK: (%s): %s\n", terminais[la], lexema);
	verifica(PVIRG);
	TSK_();
}

void TSK_() {
	printf("TSK_: (%s): %s\n", terminais[la], lexema);
	if (la == ID) {
		TSK();
	}
}

void T() {
	printf("T: (%s): %s\n", terminais[la], lexema);
	verifica(ID);
	TK();
}

void TK() {
	if (la == EBRACK) {
		printf("TK: (%s): %s\n", terminais[la], lexema);
		verifica(EBRACK);
		printf("TK: (%s): %s\n", terminais[la], lexema);
		verifica(NUM);
		printf("TK: (%s): %s\n", terminais[la], lexema);
		verifica(DBRACK);
	}
}

void VSO() {
	printf("VSO: (%s): %s\n", terminais[la], lexema);
	if (la == VARS) {
		VS();
	}
}

void VS() {
	printf("VS: (%s): %s\n", terminais[la], lexema);
	verifica(VARS);
	VSK();
}

void VSK() {
	printf("VSK: (%s): %s\n", terminais[la], lexema);
	IL();
	printf("VSK: (%s): %s\n", terminais[la], lexema);
	verifica(DPOINT);
	T();
	printf("VSK: (%s): %s\n", terminais[la], lexema);
	verifica(PVIRG);
	VSK_();
}

void VSK_() {
	printf("VSK_: (%s): %s\n", terminais[la], lexema);
	if (la == ID) {
		VSK();
	}
}

void FNO() {
	printf("FNO: (%s): %s\n", terminais[la], lexema);
	if (la == FUNCTIONS) {
		FN();
	}
}

void FN() {
	printf("FN: (%s): %s\n", terminais[la], lexema);
	verifica(FUNCTIONS);
	FNK();
}

void FNK() {
	printf("FNK: (%s): %s\n", terminais[la], lexema);
	P();
	FNK_();
}

void FNK_() {
	printf("FNK_: (%s): %s\n", terminais[la], lexema);
	if (la == ID || la == VAZIO) {
		FNK();
	}
}

void BD() {
	printf("BD: (%s): %s\n", terminais[la], lexema);
	verifica(ECHAVE);
	BDK();
	printf("BD: (%s): %s\n", terminais[la], lexema);
	verifica(DCHAVE);
}

void BDK() {
	printf("BDK: (%s): %s\n", terminais[la], lexema);
	if (la == ID || la == EBRACK || la == GOTO || 
		la == RETURN ||	la == IF || la == WHILE || la == PVIRG) {
		ST();
		BDK();
	}
}

void ST() {
	printf("ST: (%s): %s\n", terminais[la], lexema);
	if (la == ID) {		// <- outra dor de cabeça
		verifica(ID);
		printf("ST: (%s): %s\n", terminais[la], lexema);

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
	printf("STO: (%s): %s\n", terminais[la], lexema);
	if (la == DPOINT) {
		verifica(DPOINT);
		printf("STO: (%s): %s\n", terminais[la], lexema);
	}
}

void US() {
	printf("US: (%s): %s\n", terminais[la], lexema);
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
	printf("CP: (%s): %s\n", terminais[la], lexema);
	verifica(ECHAVE);
	CPK();
	printf("CP: (%s): %s\n", terminais[la], lexema);
	verifica(DCHAVE);
}

void CPK() {
	printf("CPK: (%s): %s\n", terminais[la], lexema);
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
	printf("CPK_: (%s): %s\n", terminais[la], lexema);
	if (la == ID || la == GOTO || la == RETURN ||
		 la == IF || la == WHILE || la == PVIRG) {
		CPK();
	}
}

void AS() {
	printf("AS: (%s): %s\n", terminais[la], lexema);
	if (la == OP_ATRB) {
		verifica(OP_ATRB);
		EX();
		printf("AS: (%s): %s\n", terminais[la], lexema);
		verifica(PVIRG);
	} else {
		V();
		verifica(OP_ATRB);
		printf("AS: (%s): %s\n", terminais[la], lexema);
		EX();
		verifica(PVIRG);
	}
}

void V() {
	printf("V: (%s): %s\n", terminais[la], lexema);
	VK();
}

void VK() {
	printf("VK: (%s): %s\n", terminais[la], lexema);
	if (la == EBRACK) {
		verifica(EBRACK);
		EX();
		printf("VK: (%s): %s\n", terminais[la], lexema);
		verifica(DBRACK);
		VK();
	}
}

void EX() {
	printf("EX: (%s): %s\n", terminais[la], lexema);
	SE();
	EXO();
}

void EXO() {
	printf("EXO: (%s): %s\n", terminais[la], lexema);
	if (la == OP_EQUAL || la == OP_DIFF || la == OP_GREAT ||
		la == OP_GEQ || la == OP_LESS || OP_LEQ) {
		RO();
		SE();
	}
}

void SE() {
	printf("SE: (%s): %s\n", terminais[la], lexema);
	SEO();
	TR();
	SEK();
}

void SEO() {
	printf("SEO: (%s): %s\n", terminais[la], lexema);
	if (la == OP_PLUS) {
		verifica(OP_PLUS);
	} else if (la == OP_MINUS) {
		verifica(OP_MINUS);
	}
}

void SEK() {
	printf("SEK: (%s): %s\n", terminais[la], lexema);
	if (la == OP_PLUS || la == OP_MINUS || la == OP_AND) {
		AO();
		TR();
		SEK();
	}
}

void TR() {
	printf("TR: (%s): %s\n", terminais[la], lexema);
	FAC();
	TRK();
}

void FAC() {
	printf("FAC: (%s): %s\n", terminais[la], lexema);
	if (la == ID) {
		verifica(ID);
		printf("FAC: (%s): %s\n", terminais[la], lexema);

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
		printf("FAC: (%s): %s\n", terminais[la], lexema);
		verifica(DPAREN);
	} else if (la == OP_NOT) {
		verifica(OP_NOT);
		FAC();
	}
}

void TRK() {
	printf("TRK: (%s): %s\n", terminais[la], lexema);
	if (la == OP_MULT || la == OP_DIV || OP_AND) {
		MO();
		FC();
		TRK();
	}
}

void N() {
	printf("N: (%s): %s\n", terminais[la], lexema);
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
	printf("NPOS: (%s): %s\n", terminais[la], lexema);
	if (la == OP_PLUSPLUS) {
		verifica(OP_PLUSPLUS);
	} else if (la == OP_MINUSMINUS) {
		verifica(OP_MINUSMINUS);
	}
}

void FC() {
	printf("FC: (%s): %s\n", terminais[la], lexema);
	if (la == EPAREN) {
		verifica(EPAREN);
		EL();
		printf("FC: (%s): %s\n", terminais[la], lexema);
		verifica(DPAREN);
	} else {
		verifica(ID);
		verifica(EPAREN);
		EL();
		printf("FC: (%s): %s\n", terminais[la], lexema);
		verifica(DPAREN);
	}
}

void EL() {
	printf("EL: (%s): %s\n", terminais[la], lexema);
	if (la == OP_PLUS || la == OP_MINUS || la == EPAREN || la == OP_NOT ||
		la == ID || la == OP_PLUSPLUS || la == OP_MINUSMINUS || la == NUM) {
		EX();
		ELK();
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

void RO() {
	printf("RO: (%s): %s\n", terminais[la], lexema);
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
	printf("FCS: (%s): %s\n", terminais[la], lexema);
	if (la == EPAREN) {
		FC();
		printf("FCS: (%s): %s\n", terminais[la], lexema);
		verifica(PVIRG);
	}
}

void CD() {
	printf("CD: (%s): %s\n", terminais[la], lexema);
	verifica(IF);
	verifica(EPAREN);
	EX();
	printf("CD: (%s): %s\n", terminais[la], lexema);
	verifica(DPAREN);
	CP();
	CDO();
}

void CDO() {
	printf("CDO: (%s): %s\n", terminais[la], lexema);
	if (la == ELSE) {
		verifica(ELSE);
		CP();
	}
}

void RP() {
	printf("RP: (%s): %s\n", terminais[la], lexema);
	verifica(WHILE);
	verifica(EPAREN);
	EX();
	printf("RP: (%s): %s\n", terminais[la], lexema);
	verifica(DPAREN);
	CP();
}

void ES() {
	printf("ES: (%s): %s\n", terminais[la], lexema);
	verifica(PVIRG);
}

void GO() {
	printf("GO: (%s): %s\n", terminais[la], lexema);
	verifica(GOTO);
	verifica(ID);
	verifica(PVIRG);
}

void RE() {
	printf("RE: (%s): %s\n", terminais[la], lexema);
	verifica(RETURN);
	REO();
	printf("RE: (%s): %s\n", terminais[la], lexema);
	verifica(PVIRG);
}

void REO() {
	printf("REO: (%s): %s\n", terminais[la], lexema);
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