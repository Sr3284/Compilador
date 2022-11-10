/* ---------------------------------------------------------------- */
/* parser.h                                                         */
/*                                                                  */
/*   As produções são descritas com mais detalhes na planilha       */
/*                                                                  */
/* ---------------------------------------------------------------- */

void verifica(int t);
char* parser();

void S();		// S
void P();		// Program + Function
void FPS();		// Formal_ParameterS
void FP();		// Formal_Parameter
void EP();		// Expression_Parameter
void FUP();		// FUnctin_Parameter
void B();		// Block
void L();		// Labels
void IL();		// Identifier_List
void TS();		// TypeS
void TSK();		// TypeS Kleene
void T();		// Type
void TK();		// Type Kleene
void VS();		// VariableS
void VSK();		// VariableS Kleene
void FN();		// FuNctions
void FNK();		// FuNctions Kleene
void BD();		// BoDy
void BDK();		// BoDy Kleene
void ST();		// STatement
void STO();		// Statement Optional
void CP();		// ComPound
void CPK();		// ComPound Kleene
void FCS();		// Function_Call_Statement
void FC();		// Function_Call
void AS();		// ASsignment
void VK();		// Variable Kleene
void US();		// Unlabeled_Statement
void REO();		// REturn Optional
void EL();		// Expression_List
void ELK();		// Expression_List Kleene
void EX();		// EXpression
void SE();		// Simple_Expression
void SEK();		// Simple_Expression Kleene
void TR();		// TeRm
void TRK();		// TeRm Kleene
void FAC();		// FACtor
void N();		// Criado na tentativa de aceitar números com ++ -- pré e pós fixados
void AO();		// Additive_Operator
void MO();		// Multiplicative_Operator
void RO();		// Relational_Operator