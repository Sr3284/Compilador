import java.util.List;
import java.util.ArrayList;
import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;

//Prototipo simples de analisador léxico

public class Tokenizer {

	//Possui três tokens "(", ")" e "atomos"
	public enum Ttipo {
		INTER, BOOL, VERD, FALS, READ, WRITE,	//Predefined identifiers
		EPAREN, DPAREN, PONTO_VIRG, VIRG,
		ECHAVES, DCHAVES, EBRACK, DBRACK,    	//Pontuação
		OP_EQUALS, OP_DIFF, OP_LESS, OP_LEQ,
		OP_GEQ, OP_GREAT, OP_PLUS, OP_MINUS,
		OP_OR, OP_MULT, OP_DIV, OP_AND, OP_NOT,
		OP_ATRB,                             	//Simbolos de operações
		VOID, GOTO, RETURN, IF, WHILE,       	//Reservadas
		COMEN, INVALIDO, SPACE,                	//Debug
		NUM,                                   	//Numero
		IDENT;                                 	//Identificador
	}

	String[] keywords = {"interger", "boolean", "true", "false", "read", "write", "void", "goto", "return", "if", "while"};

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
			if ((t == Ttipo.IDENT) || (t == Ttipo.NUM)) {
				return "<linha: " + l + ", " + t.toString() + ", \"" + c + "\">";
			}

			return "<linha: " + l + ", " + t.toString() + ">";
		}
	}

	//Recebe uma String e um indice, pega um lexema a partir do indice 
	public static String getAtomo(String s, int i) {
		int j = i;

			for ( ; j < s.length(); ) {
				if (Character.isLetterOrDigit(s.charAt(j))) {
					j++;
				} else {
					return s.substring(i, j);
				}
		}

		return s.substring(i, j);	
	}

	public static List<Token> lex(String in, int l) {
		List<Token> resultado = new ArrayList<Token>();
		String ttemp = new String();

		for (int i = 0; i < in.length(); ) {
			switch (in.charAt(i)) {
			case '\r':
			case '\t':
			case ' ':
				resultado.add(new Token(Ttipo.SPACE, "", l));
				break;
			case '(':
				resultado.add(new Token(Ttipo.EPAREN, "(", l));
				i++;
				break;
			case ')':
				resultado.add(new Token(Ttipo.DPAREN, ")", l));
				i++;
				break;			
			case ';':
				resultado.add(new Token(Ttipo.PONTO_VIRG, ";", l));
				i++;
				break;
			case ',':
				resultado.add(new Token(Ttipo.VIRG, ",", l));
				i++;
				break;
			case '{':
				resultado.add(new Token(Ttipo.ECHAVES, "{", l));
				i++;
				break;
			case '}':
				resultado.add(new Token(Ttipo.DCHAVES, "}", l));
				i++;
				break;
			case '[':
				resultado.add(new Token(Ttipo.EBRACK, "[", l));
				i++;
				break;
			case ']':
				resultado.add(new Token(Ttipo.DBRACK, "]", l));
				i++;
				break;
			//OPERADORES
			case '+':
				resultado.add(new Token(Ttipo.OP_PLUS, "+", l));
				i++;
				break;
			case '-':
				resultado.add(new Token(Ttipo.OP_MINUS, "-", l));
				i++;
				break;
			case '*':
				resultado.add(new Token(Ttipo.OP_MULT, "*", l));
				i++;
				break;
			case '/':
				if (in.charAt(i + 1) == '/') {
					resultado.add(new Token(Ttipo.COMEN, "", l));
					i = in.length();
				} else {
					resultado.add(new Token(Ttipo.OP_DIV, "/", l));
					i++;
				}
				break;
			case '=':
				if (in.charAt(i + 1) == '=') {
					resultado.add(new Token(Ttipo.OP_EQUALS, "==", l));
					i+=2;
				} else {
					resultado.add(new Token(Ttipo.OP_ATRB, "=", l));
					i++;
				}
				break;
			case '<':
				if (in.charAt(i + 1) == '=') {
					resultado.add(new Token(Ttipo.OP_LEQ, "<=", l));
					i+=2;
				} else {
					resultado.add(new Token(Ttipo.OP_LESS, "<", l));
					i++;
				}
				break;
			case '>':
				if (in.charAt(i + 1) == '=') {
					resultado.add(new Token(Ttipo.OP_GEQ, ">=", l));
					i+=2;
				} else {
					resultado.add(new Token(Ttipo.OP_GREAT, ">", l));
					i++;
				}
				break;
			case '&':
				if (in.charAt(i + 1) == '&') {
					resultado.add(new Token(Ttipo.OP_AND, "&&", l));
					i+=2;
				} else {
					resultado.add(new Token(Ttipo.INVALIDO, "&", l));
				}
				break;
			case '|':
				if (in.charAt(i + 1) == '|') {
					resultado.add(new Token(Ttipo.OP_OR, "||", l));
					i+=2;
				} else {
					resultado.add(new Token(Ttipo.INVALIDO, "|", l));
				}
				break;

			case '!':
				if (in.charAt(i + 1) == '=') {
					resultado.add(new Token(Ttipo.OP_DIFF, "!=", l));
					i+=2;
				} else {
					resultado.add(new Token(Ttipo.OP_NOT, "!", l));
					i++;
				}
				break;
			default:
				String atomo = getAtomo(in, i);
				i += atomo.length();
				resultado.add(new Token(Ttipo.IDENT, atomo, l));
				break;
			}
		}

		return resultado;
	}

	//Main
	public static void main(String[] args) throws IOException {
		if (args.length < 1) {
			System.out.println("Uso: java Tokenizer \"<Arquivo de texto>\"");
			return;
		}

		BufferedReader leitor;

		try {
			leitor = new BufferedReader(new FileReader(args[0]));
			String in = leitor.readLine();

			int l = 1;
			while (in != null) {
				List<Token> tokens = lex(in, l);
				for (Token t : tokens) {
					System.out.println(t);
				}

				in = leitor.readLine();
				l++;
			}

			leitor.close();
		} catch (IOException ex) {
			ex.printStackTrace();
		}
	}
}
