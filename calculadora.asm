; Universidade de Brasília
; Departamento de Ciência da Computação
; Software Básico - Turma B - 2018/1
; Professor Bruno L. Macchiavello

;    Trabalho 2 - Item 3 do Trabalho 2 - Calculadora Assembly IA-32

; GRUPO: LUIS FERNANDO ARRUDA MARQUES - 15/0016271
;        NASIRU ADAMU MARAFA          - 11/008055


section .data
	msg_user db 'Insira o seu nome: ',0
	msg_begin1 db 'Ola ',0
	msg_begin2 db '! Bem-vindo ao programa de CALC IA-32',0dh,0ah,0
	msg_menu0 db 'ESCOLHA UMA OPCAO:',0dh,0ah,0
	msg_menu1 db '1: SOMA',0dh,0ah,0
	msg_menu2 db '2: SUBTRACAO',0dh,0ah,0
	msg_menu3 db '3: MULTIPLICACAO',0dh,0ah,0
	msg_menu4 db '4: DIVISAO',0dh,0ah,0
	msg_menu5 db '5: MOD',0dh,0ah,0,0dh,0ah,0
	msg_menu6 db '6: SAIR',0dh,0ah,0
	msgnum1 db 'Insira o 1o. operando: ',0
	msgnum2 db 'Insira o 2o. operando: ',0
	msg_res db 'O resultado e: ',0
	msgenter db 0dh,0ah,'Tecle <ENTER> para continuar',0dh,0ah,0
section .bss
	user resb 50
	num1 resb 50
	intnum1 resd 1
	num2 resb 50
	intnum2 resd 1
	res  resb 100
	intres resd 1
	escolha resb 1
section .text
global _start
_start:
	push msg_user ;TOS (top of stack)
	call _printstr ;printa TOS
	mov ecx,user
	call _readstr
	mov byte [user+eax-1],0 ;remove o enter da string pra printar na mesma linha
	push msg_begin1
	call _printstr
	push user
	call _printstr
	push msg_begin2
	call _printstr

menu:
	push msg_menu0
	call _printstr
	push msg_menu1
	call _printstr
	push msg_menu2
	call _printstr
	push msg_menu3
	call _printstr
	push msg_menu4
	call _printstr
	push msg_menu5
	call _printstr
	push msg_menu6
	call _printstr
	mov ecx,escolha
	call _readstr


switch:
	cmp byte [escolha],'1'
	je _add
	cmp byte [escolha],'2'
	je _sub
	cmp byte [escolha],'3'
	je _mult
	cmp byte [escolha],'4'
	je _div
	cmp byte [escolha],'5'
	je _mod
	cmp byte [escolha],'6'
	je _fim ;else
	jmp menu


_add:
	push msgnum1
	call _printstr
	mov ecx,num1
	call _readstr
	push num1
	call _str2int
	mov dword [intnum1],eax ;retorno de _str2int

	push msgnum2
	call _printstr
	mov ecx,num2
	call _readstr
	push num2
	call _str2int
	mov dword [intnum2],eax ;retorno de _str2int

	mov eax,[intnum1]
	add eax,[intnum2]
	mov [intres],eax
	jmp _fimop

_sub:
	push msgnum1
	call _printstr
	mov ecx,num1
	call _readstr
	push num1
	call _str2int
	mov dword [intnum1],eax ;retorno de _str2int

	push msgnum2
	call _printstr
	mov ecx,num2
	call _readstr
	push num2
	call _str2int
	mov dword [intnum2],eax ;retorno de _str2int

	mov eax,[intnum1]
	sub eax,[intnum2]
	mov [intres],eax
	jmp _fimop

_mult:
	push msgnum1
	call _printstr
	mov ecx,num1
	call _readstr
	push num1
	call _str2int
	mov dword [intnum1],eax ;retorno de _str2int

	push msgnum2
	call _printstr
	mov ecx,num2
	call _readstr
	push num2
	call _str2int
	mov dword [intnum2],eax ;retorno de _str2int

	mov eax,[intnum1]
	mov ebx,[intnum2]
	imul ebx
	mov [intres],eax
	jmp _fimop

_div:
	push msgnum1
	call _printstr
	mov ecx,num1
	call _readstr
	push num1
	call _str2int
	mov dword [intnum1],eax ;retorno de _str2int

	push msgnum2
	call _printstr
	mov ecx,num2
	call _readstr
	push num2
	call _str2int
	mov dword [intnum2],eax ;retorno de _str2int

	mov eax,[intnum1]
	mov ebx,[intnum2]
	cdq ; extende em edx o sinal de eax
	idiv ebx
	mov [intres],eax
	jmp _fimop

_mod:
	push msgnum1
	call _printstr
	mov ecx,num1
	call _readstr
	push num1
	call _str2int
	mov dword [intnum1],eax ;retorno de _str2int

	push msgnum2
	call _printstr
	mov ecx,num2
	call _readstr
	push num2
	call _str2int
	mov dword [intnum2],eax ;retorno de _str2int

	mov eax,[intnum1]
	mov ebx,[intnum2]
	cdq ; extende em edx o sinal de eax
	idiv ebx
	mov [intres],edx ;pega o resto da divisao
	;jmp _fimop

_fimop:
	push msg_res
	call _printstr

	sub eax,eax
	mov [res],eax ;reseta o resultado
	
	call _int2str
	push res
	call _printstr

	push msgenter
	call _printstr
	mov ecx,escolha
	call _readstr
	jmp menu

	jmp _fim ; desnecessario

_str2int: ;funcao que converte string para inteiro.
;Precisa empilhar a string pois sera o parametro
	enter 0,0
	push dword 1 ;[ebp-4] <- 1. 
	pusha ; guarda os valores dos registradores... a funcao nao afetara eles
	
	;contagem da qtde de menos na string
	mov eax,dword [ebp+8]
	sub ecx,ecx
	mov ebx,eax
	laco1:
		mov al,byte [ebx]
		cmp al,0
		je laco1s
		inc ebx
		inc ecx
		cmp al,'-'
		je laco1
		dec ecx
		cmp al,'0' ;ignora '0'
		jz laco1
		cmp al,' ';ignora ' '
		jz laco1
		cmp al,9h ;ignora o TAB
		jz laco1
		dec ebx
	laco1s:
	mov eax,ecx
	test eax,1 ;verifica se a qtd de menos eh impar (negativo)
	je pula 
		mov dword [ebp-4],-1; eh impar, entao [ebp-4] <- -1
	pula: 
	;neste instante, ebx eh uma string com caracteres validos
	sub esi,esi ;reseta o inteiro
	laco2: 
		sub eax,eax
		mov al, byte [ebx]
		cmp al,'0'
		jl laco2s ;se nao for numero valido, quebra o loop (menor que 0)
		cmp al,'9'
		jg laco2s  ;se nao for numero valido, quebra o loop (maioe que 9)
		
		sub al,30h ;subtrai 48 (0x30) do ASCII (pra ficar o numero certo)
		add esi,eax ;soma o numero ao inteiro
		;multiplicar por 10:
		mov eax,esi
		add eax,eax ;multiplica por 2
		shl esi,3 ;multiplica por 8
		add esi,eax ;soma (num*2+num*8 = num*10)
		inc ebx; incrementa o ponteiro (pilha)
		jmp laco2
	laco2s: 
	; o laco anterior multiplicava por 10 uma vez a mais, logo precisa dividir por 10
	mov eax,esi
	sub edx,edx
	mov ecx,10
	div ecx
	mov esi,eax
	;uma vez que [ebp-4] guardou o sinal do numero, basta multiplicar por eles
	;se [ebp-4] eh -1, entao inverte o sinal
	mov ecx,dword [ebp-4]
	mov eax,esi
	imul ecx 
	mov dword [ebp-8],eax ;[ebp-8] eh a posicao da variavel local que sera o retorno
	popa ;restaura os valores dos registradores antes de entrar na funcao
	leave
ret 4


_printstr:
	enter 0,0
	mov eax,[ebp+8] ;primeiro e unico argumento (string a ser impressa)
	mov ecx,-1 ;começa com -1 pois o loop inicia incrementando
len: ;loop para calcular o tamanho (len) da string
	inc ecx
	cmp byte [eax+ecx],0 ;0 eh o EOF
	jne len
	mov eax,4
	mov ebx,1
	mov edx,ecx ;recebe len da msg
	mov ecx,[ebp+8]
	int 80h
	leave
	ret

_readstr: ;le string de tamanho ate 50 bytes. Passagem por registrador (ecx)
	mov eax,3
	mov ebx,0
	;ecx ja foi recebido.
	mov edx,50
	int 80h
	ret

_int2str:
	enter 0,0
	mov eax,[intres]
	sub ecx,ecx ;ecx <- 0
	sub edx,edx
	sub esi,esi
	mov ebx,10 ;utilizado na divisao
	cmp eax,0
	jnl percorre
	mov byte [res+esi],'-'
	inc esi
	neg eax ;muda o sinal
percorre: ;pula se nao negativo
	div ebx
	add edx,48 ;fator de conversao de inteiro pra ASCII
	push edx
	inc ecx
	sub edx,edx
	cmp eax,0
	jne percorre ;faz ate o quociente ser diferente de zero
final:
	pop edx ;ultimo resto
	mov byte [res+esi],dl
	dec ecx
	inc esi
	cmp ecx,0
	jne final
	mov byte [res+esi],0 ;poe null como EOF (necessario pro _printstr funcionar)
	leave
	ret

_fim:
	mov eax,1
	mov ebx,0
	int 80h