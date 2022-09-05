import java.util.List;
import java.util.ArrayList;

//Prototipo simples de analisador léxico

public class Tokenizer {

	//Possui três tokens "(", ")" e "atomos"
	public static enum Ttipo {
		INTER, BOOL, VERD, FALS, READ, WRITE,						//Predefined identifiers
		EPAREN, DPAREN, PONTO_VIRG, VIRG, ECHAVES, DCHAVES,			//Pontuação
		OP_EQUALS, OP_DIFF, OP_LESS, OP_LEQ, OP_GEQ, OP_GREAT,
		OP_PLUS, OP_MINUS, OP_OR, OP_MULT, OP_DIV, OP_AND, OP_NOT,	//Simbolos de operações
		VOID, GOTO, RETURN, IF, WHILE,								//Reservadas?
		INVALIDO;													//Debug
	}

	//Classe dos Tokens
	public static class Token {
		public final Ttipo t;	//Tipo do token
		public final String c;	//Conteudo dos tokens
		public final int l; 	//Linha em que o token aparece

		public Token(Ttipo t, String c, int l) {
			this.t = t;
			this.c = c;
			this.l = l;
		}

		public String toString() {
			if (t == Ttipo.ATOMO) {
				return "ATOMO <" + c + "> na linha " + l;
			}
			return t.toString();
		}
	}

	//Recebe uma String e um indice, pega um atomo a partir do indice 
	public static String getAtomo(String s, int i) {
		int j = i;

		for ( ; j < s.length(); ) {
			if (Character.isLetter(s.charAt(j))) {
				j++;
			} else {
				return s.substring(i, j);
			}
		}

		return s.substring(i, j);
	}

	public static List<Token> lex(String in) {
		List<Token> resultado = new ArrayList<Token>();

		for (int i = 0; i < in.length(); ) {
			switch (in.charAt(i)) {
			case '(':
				resultado.add(new Token(Ttipo.EPAREN, "(", 1));
				i++;
				break;
			case ')':
				resultado.add(new Token(Ttipo.DPAREN, ")", 1));
				i++;
				break;
			default:
				if (Character.isWhitespace(in.charAt(i))) {
					i++;
				} else {
					String atomo = getAtomo(in, i);
					i += atomo.length();
					resultado.add(new Token(Ttipo.ATOMO, atomo, 1));
				}
				break;
			}
		}

		return resultado;
	}

	//versão in-line
	public static void main(String[] args) {
		if (args.length < 1) {
			System.out.println("Uso: java Tokenizer \"<Algo para o Lexer analisar>\"");
			return;
		}

		List<Token> tokens = lex(args[0]);
		for (Token t : tokens) {
			System.out.println(t);
		}
	}
}