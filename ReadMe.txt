Compressor Huffman C++
Descrição
Este projeto é uma implementação em C++ do algoritmo de compressão de Huffman, desenvolvido como parte da disciplina de Estrutura de Dados 2. O objetivo é criar uma ferramenta de linha de comando capaz de comprimir e descomprimir arquivos, com uma otimização especial para códigos-fonte da linguagem C++.

O projeto é dividido em dois programas principais:

Contador de Frequência (contador): Uma ferramenta para analisar um conjunto de arquivos e gerar uma tabela de frequência de símbolos (caracteres e palavras-chave).

Compressor/Descompressor (compressor): O programa principal que utiliza uma tabela de frequência fixa para comprimir arquivos para o formato .huff e descomprimi-los de volta ao seu estado original.

Funcionalidades
Contagem de Frequência: Analisa múltiplos arquivos para gerar uma tabela de frequência otimizada.

Compressão: Comprime qualquer arquivo de texto, utilizando uma tabela de frequência pré-definida.

Descompressão: Restaura arquivos comprimidos (.huff) para sua forma original, garantindo a integridade dos dados.

Otimização para C++: A lógica de análise e a tabela de frequência são projetadas para alcançar uma melhor taxa de compressão em arquivos .cpp.

Estrutura do Projeto
compressor.cpp: Contém a função main para o programa de compressão e descompressão.

contador_main.cpp: Contém a função main para o programa de análise de frequência.

huffman_logic.hpp: Biblioteca principal com toda a lógica de Huffman (construção da árvore, geração de códigos, funções de compressão e descompressão).

huffman_tree.hpp: Definição da estrutura Node que representa um nó na árvore de Huffman.

Como Compilar
É necessário um compilador C++ com suporte para C++11 ou superior (ex: g++).

Compilar o Contador de Frequência:

Bash

g++ -std=c++11 -o contador contador_main.cpp
Compilar o Compressor/Descompressor:

Bash

g++ -std=c++11 -o compressor compressor.cpp
Como Usar
O fluxo de trabalho completo envolve primeiro gerar uma tabela de frequência otimizada e depois usá-la no programa principal.

Passo 1: Gerar a Tabela de Frequência
Use o programa contador para analisar seus arquivos de código-fonte (.cpp, .hpp) e gerar um mapa de frequência.

Bash

# O contador criará um arquivo 'meu_mapa.txt' automaticamente.
# Liste todos os arquivos que você deseja usar como base para a análise.
./contador arquivo1.cpp arquivo2.cpp seu_projeto.cpp
Este comando criará um arquivo meu_mapa.txt com a tabela de frequência já formatada como código C++.

Passo 2: Atualizar o Compressor com a Tabela
Abra o arquivo meu_mapa.txt gerado.

Copie todo o conteúdo que está dentro das chaves {...}.

Abra o arquivo compressor.cpp.

Encontre a variável FIXED_FREQUENCY_MAP e substitua seu conteúdo pelo mapa que você acabou de copiar.

Recompile o compressor com o comando do passo de compilação para que ele use a nova tabela otimizada.

Passo 3: Comprimir um Arquivo
Para comprimir um arquivo, use o programa compressor com a flag -c.

Sintaxe:
./compressor -c <arquivo_de_entrada> <arquivo_de_saida.huff>

Exemplo:

Bash

./compressor -c main.cpp main.huff
Passo 4: Descomprimir um Arquivo
Para descomprimir um arquivo .huff, use a flag -d.

Sintaxe:
./compressor -d <arquivo_comprimido.huff> <arquivo_de_saida>

Exemplo:

Bash

./compressor -d main.huff main_restaurado.cpp
Após a descompressão, o arquivo main_restaurado.cpp será idêntico ao arquivo main.cpp original.