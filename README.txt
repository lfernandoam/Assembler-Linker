Compilado com sucesso com seguinte comando:

gcc montador.c -o montador -Wall
gcc ligador.c -o ligador -Wall

Versão do gcc utilizado:
gcc (Debian 6.4.0-2) 6.4.0 20170724


Executa-se  o programa utilizando a seguinte sintaxe:

ATENCAO: NAO COLOCAR A EXTENSAO DO ARQUIVO!
Montador (NOTE QUE O MONTADOR NAO ESPECIFICA SAIDA: sera gerado um (nome_entrada).txt pra cada entrada)
./montador arq1 arq2
		ou
./montador arq1

Ligador (ESPECIFICAR SAIDA: sera gerado um (nome_saida).o)
./ligador arq1 arq2 saida
		ou
./ligador arq1 saida


CALCULADORA:

	MONTAGEM E LIGACAO:
nasm -f elf -o calculadora.o calculadora.asm
ld -m elf_i386 -o calculadora calculadora.o

	EXECUCAO:
./calculadora