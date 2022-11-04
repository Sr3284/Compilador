# Trabalho de Compiladores

### Cronograma de entregas
  1. 25/08 - Início do trabalho;
  2. 20/09 - 1º Checkpoint: Entrevista sobre a parte 1;
  3. 09/11 - 2º Checkpoint: Entrevista sobre a parte 2; (atualizado)
  4. 24/11 - Entrega e entrevista do Trabalho.

### Critérios de avaliação
  - CHP1 e CHP2: Avaliação dos checkpoints;
  - IMP: Implementação;
  - REL: Relatório;
  - BON: Bônus.
  
  T = (CHP1 * 1.5 + CHP2 * 1.5 + IMP * 4 + REL * 3 + BON * 1)

## Analisador Léxico (Parte 1)

<ul>
    <li>- [x] Projetar um analisador léxico para a linguagem proposta, identificando seus átomos a partir da gramática apresentada pelo professor da disciplina.</li>
    <li>- [x] O método deve “ler” o contéudo de um arquivo, cujo nome é passado como parâmetro e retornar os átomos (tokens) encontrados.</li>
    <li>- [x] Nesta etapa, você deve implementar um mecanismo de controle de numeração das linhas e ignorar comentários. Para cada token reconhecido, deve ser possível exibir: <br><p align="center"><b>&lt;Numero da linha do token, Token, Atributo (quando possuir atributo)&gt;</b></p></br></li>
    <li>- [x] Observe que o seu analisador léxico deve retornar um token sempre que for chamado. Nesse momento, os tokens identificados devem ser colocados na saída padrão.</li>
    <li>- [x] As palavras-chave são consideradas palavras reservadas, i.e. não podem ser utilizadas como identificadores. A lista de palavras-reservadas para a linguagem na sua especificação.</li>
    <li>- [x] Seu Analisador Léxico deve criar e gerenciar uma tabela de palavras reservadas (que pode ser implementada como uma tabela hash ou vetor de strings) para as palavras reservadas da linguagem. Toda vez que um identificador for encontrado, deverá ser feita uma busca na tabela de palavras reservadas. No caso de sucesso, o Analisador Léxico deve retornar como token a palavra reservada encontrada. Caso contrário, um identificador foi reconhecido e o mesmo deverá ser inserido na tabela de símbolos, caso ainda não tenha sido inserido.</li>
    <li>- [x] Identifique e descreva as definições regulares para tokens sem atributos tais como, operadores aritméticos, relacionais, atribuição, entre outros.</li>
    <li>- [x] Identifique e descreva as definições regulares para tokens com atributos, tais como identificadores, números, decimais, comentários, entre outros.</li>
    <li>- [x] Observe que o reconhecimento de palavras reservadas deve ser implementado no reconhecimento de identificadores, pois possuem o mesmo padrão de formação.</li>
    <li>- [x] Por fim, não se esqueça de identificar e descrever definições regulares para tokens delimitadores: espaços em branco, tabulações e caracteres de nova linha.</li>
</ul>

## Analisador Sintático (Parte 2)
