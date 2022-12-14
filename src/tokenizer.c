#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "tokenizer.h"

//globais
char c;          	//Armazena o char lido na entrada
char lexema[100];	//Armazena a palavra processada
int ilexema;     	//Indice do lexema
int linhas = 1;  	//Contadores de linhas

char *pChaves[] = {"boolean", "else", "false", "functions", "goto", "if", "integer", "labels", "read", 
					"return", "true", "types", "var", "vars", "void", "while", "write"};	//palavras reservadas

char *terminais[] = {"boolean", "else", "false", "functions", "goto", "if", "integer", "labels", "read",
					 "return", "true", "types", "var", "vars", "void", "while", "write", "EPAREN",
					 "DPAREN", "ECHAVE", "DCHAVE", "EBRACK", "DBRACK", 
					 "PVIRG", "VIRG", "DPOINT", "OP_EQUAL", "OP_DIFF", "OP_GREAT", 
					 "OP_GEQ", "OP_LESS", "OP_LEQ", "OP_AND", "OP_OR","OP_ATRB", 
					 "OP_PLUS", "OP_PLUSPLUS", "OP_MINUS", "OP_MINUSMINUS", 
					 "OP_MULT", "OP_DIV", "OP_NOT", "ID", "NUM", "COMENT", "FIM"};	//terminais aceitos

//verifica se é uma palavra reservada, retorna cod. da palavra chave ou ID 
char ehChave(char *i) {

	char *temp = strdup(i);
	unsigned char *tpointer = (unsigned char *)temp;

	int j, k, l, m, n;
//transforma todos caracteres para minusculo
	while (*tpointer) {
		*tpointer = tolower(*tpointer);
		tpointer++;
	}

	j = 0;
	k = (sizeof(pChaves)/sizeof(pChaves[0])) - 1;

	while (j <= k) {

		l = (j + k)/2;
		//compara caractere a caractere das palavras reservadas com a entrada armazenada em temp 
		if ((m = strcmp(temp,pChaves[l])) == 0) {
			free(temp);
			return l;
		} else if (m < 0) {
			k = l - 1;
		} else {
			j = l + 1;
		}
	}
//cria a tabela de simbolos sem repetição
	for (j = 0; j <= lastSimb; j++) {
		if ((m = strcmp(temp,tSimbolo[j])) == 0) {
			posTabela = j;
			j = lastSimb + 1;
		}
	}

	if (m != 0) {
		strcpy(tSimbolo[lastSimb], i);
		posTabela = lastSimb;
		lastSimb+=1;
	}

	free(temp);
	return ID;
}

//processa o arquivo e devolve o código da palavra ou err
int tokenizer() {

	int est;

	ilexema = 0;
	est = 0;

	do {

		c = getc(f);

		lexema[ilexema++] = c;
		lexema[ilexema] = '\0';

		switch (est) {
		case 0:
			switch (c) {
			case '\t':
			case ' ':
				ilexema--;
				break;
			case '\n':
				ilexema--;
				linhas++;
				break;
			case '+':
				est = 1;
				break;
			case '-':
				est = 2;
				break;
			case '*':
				return OP_MULT;
				break;
			case '/':
				est = 3;
				break;
			case '=':
				est = 4;
				break;
			case '!':
				est = 14;
				break;
			case '>':
				est = 5;
				break;
			case '<':
				est = 6;
				break;
			case '(':
				return EPAREN;
			case ')':
				return DPAREN;
			case '{':
				return ECHAVE;
			case '}':
				return DCHAVE;
			case '[':
				return EBRACK;
			case ']':
				return DBRACK;
			case ';':
				return PVIRG;
			case ':':
				return DPOINT;
			case ',':
				return VIRG;
			case '&':
				est = 7;
				break;
			case '|':
				est = 8;
				break;
			default:
				if (isdigit(c)) {
					est = 9;
				} else if (isalpha(c)) {
					est = 10;
				} else if (!feof(f)) {
					return ERR;
				}
				break;
			}
			break;
		case 1:
			if (c == '+') {
				return OP_PLUSPLUS;
			} else {
				ungetc(c, f);
				lexema[--ilexema] = '\0';
				return OP_PLUS;
			}
			break;
		case 2:
			if (c == '-') {
				return OP_MINUSMINUS;
			} else {
				ungetc(c, f);
				lexema[--ilexema] = '\0';
				return OP_MINUS;
			}
			break;
		case 3:
			if (c == '/') {
				est = 11;
			} else if (c == '*') {
				est = 12;
			} else {
				ungetc(c, f);
				lexema[--ilexema] = '\0';
				return OP_DIV;
			}
			break;
		case 4:
			if (c == '=') {
				return OP_EQUAL;
			} else {
				ungetc(c, f);
				lexema[--ilexema] = '\0';
				return OP_ATRB;
			}
			break;
		case 5:
			if (c == '=') {
				return OP_GEQ;
			} else {
				ungetc(c, f);
				lexema[--ilexema] = '\0';
				return OP_GREAT;
			}
			break;
		case 6:
			if (c == '=') {
				return OP_LEQ;
			} else {
				ungetc(c, f);
				lexema[--ilexema] = '\0';
				return OP_LESS;
			}
			break;
		case 7:
			if (c == '&') {
				return OP_AND;
			} else {
				ungetc(c, f);
				lexema[--ilexema] = '\0';
				return ERR;
			}
			break;
		case 8:
			if (c == '|') {
				return OP_OR;
			} else {
				ungetc(c, f);
				lexema[--ilexema] = '\0';
				return ERR;
			}
			break;
		case 9:
			if (!isdigit(c)) {
				ungetc(c, f);
				lexema[--ilexema] = '\0';
				return NUM;
			}
			break;
		case 10:
			if (!isalpha(c) && !isdigit(c)) {
				ungetc(c, f);
				lexema[--ilexema] = '\0';
				return (ehChave(lexema));
			}
			break;
		case 11:
			if (c == '\n') {
				ungetc(c, f);
				lexema[--ilexema] = '\0';
				return COMENT;
			}
			break;
		case 12:
			if (c == '*') {
				est = 13;
			}
			break;
		case 13:
			if (c == '/') {
				return COMENT;
			} else {
				est = 12;
			}
			break;
		case 14:
			if (c == '=') {
				return OP_DIFF;
			} else {
				ungetc(c, f);
				lexema[--ilexema] = '\0';
				return OP_NOT;
			}
		}
	}while (!feof(f));

	if (est == 0) {      	//sinaliza final do arquivo
		lexema[0] = '\0';
		return FIM;
	}

	return ERR;
}		 
