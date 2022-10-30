#include <stdio.h>

enum simbAceitos {BOLEAN, FALSE, FUNCTIONS, GOTO, IF, INTEGER, LABELS, READ, RETURN, TRUE, TYPES,
VAR, VARS, VAZIO, WHILE, WRITE,EPAREN, DPAREN, ECHAVE, DCHAVE, EBRACK, DBRACK, PVIRG, VIRG, DPOINT,
OP_EQUAL, OP_DIFF, OP_GREAT, OP_GEQ, OP_LESS, OP_LEQ, OP_AND, OP_OR, OP_ATRB, OP_PLUS, OP_PLUSPLUS,
OP_MINUS, OP_MINUSMINUS, OP_MULT, OP_DIV, ID, NUM, COMENT, FIM, ERR};

extern int linhas, lastSimb, currLimit, posTabela;
extern char lexema[100], *terminais[], **tSimbolo;
extern FILE *f;

char ehChave (char *i);
int tokenizer();