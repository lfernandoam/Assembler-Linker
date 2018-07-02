/*Universidade de Brasília
Departamento de Ciência da Computação
Software Básico - Turma B - 2018/1
Professor Bruno L. Macchiavello

   Trabalho 2 - Item 1 - Montador de Assembly didático
GRUPO: LUIS FERNANDO ARRUDA MARQUES - 15/0016271
       NASIRU ADAMU MARAFA          - 11/008055 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int qtd=0; //conta o tamanho do arquivo
int offset=0;
int linham=1; // contador de linhas utilizada na funcao de montagem
int noval=0;

typedef struct SymbTable { // Tabela de Simbolos
	char symbol[20]; // nome do simbolo
	char end; // endereco
	long int const_value; //valor de sua constante (-99999 se nao eh const)
	int externo;
} SymbTable;


int getToken(char* fonte,int *pos, char* token);
int monta(char* fonte, FILE *fileOUT, int numprog);
long int getConstValue(char* fonte,int j);
int desloc(char* token);
int passar_pra_string(FILE *fileIN, char* fonte, long tam_fonte);
void insertTable(SymbTable* tSymb, int *tamSymb, char* token, int endereco, int const_value, int externo);
void exibe(SymbTable *tSymb, int tamSymb);
int checkEnd(SymbTable* tSymb, int tamSymb, char* token);
void segPassagem(char* fonte, SymbTable* tSymb, int tamSymb, char* token, int endf, FILE *fileOUT);


void exibe(SymbTable *tSymb, int tamSymb) { // Função utilizada apenas para Debug.
	int i;
	printf("\n");
	for(i=0; i<tamSymb; i++) {
		printf("tSymb[i].symbol: %s | ",tSymb[i].symbol);
		printf("tSymb[i].end: %d | ",tSymb[i].end);
		printf("tSymb[i].const_value: %ld | ",tSymb[i].const_value);
		printf("tSymb[i].externo: %d | ",tSymb[i].externo);
	}
}

void insertTable(SymbTable* tSymb, int *tamSymb, char* token, int endereco, int const_value, int externo) {
	int i, existe=0;
	int tam_token=strlen(token);
	token[tam_token-1]='\0';
	//printf("intertTable.token: %s\n",token);
	for(i=0; i<*tamSymb; i++) {
		if(strcmp(token,tSymb[i].symbol)==0) {
			existe=1;
			printf("ERRO SEMANTICO: linha %d, com Rotulo repetido.\n",linham);
			//erro=1;
		}
	}
	if (!existe) {
		//printf("<DEBUG> INSERÇÃO: %s\n", token);
		++*tamSymb;
		tSymb=realloc(tSymb,(*tamSymb)*sizeof(SymbTable));
		strcpy(tSymb[*tamSymb-1].symbol,token); //tSymb[tamSymb-1] eh a linha
		tSymb[*tamSymb-1].end=endereco;
		tSymb[*tamSymb-1].const_value=const_value;
		tSymb[*tamSymb-1].externo=externo; // 1 se sim, 0 se nao
	}
	//exibe(tSymb,*tamSymb);
}


int checkEnd(SymbTable* tSymb, int tamSymb, char* token) {
	int i;
	for(i=0; i<tamSymb; i++) {
		if(strcmp(token,tSymb[i].symbol)==0) {
			if(tSymb[i].externo==1){
				return 0;
			}
			return tSymb[i].end;
		}
	}
	return -1; //mudar para zero (caso der problema, ele vai assumir que eh externo... menos chance de errar)
}


int desloc(char* token) {
	if(offset) {
		return atoi(token);
	}
	return 0;
}

int getToken(char* fonte,int *pos, char* token) {
	int i = 0, j = *pos;

	while(fonte[j] == '\t' || fonte[j] == ' ' || fonte[j] == '\r' || fonte[j]=='+' || fonte[j]==',') {
		j++;
		++*pos;
	}
	while(fonte[j] == '\n'){
        noval=1; //usado para evitar dar erro sintatico e semantico na mesma linha caso nao tenha operando
		j++;
		++*pos;
		linham++;
	}
	while(fonte[j] != '\n' && fonte[j] != '\t' && fonte[j] != ' ' && fonte[j] != '\r' && fonte[j] != '\0') {
		token[i] = fonte[j];
		i++;
		j++;
		if((fonte[j]=='+')) {
			offset=1;
			break;
		}
		if((fonte[j]==',')||(fonte[j-1]==',')) {
			break;
		}

	}
	token[i] = '\0';
	int tam=strlen(token);
	//printf("getToken.token: %s\n",token);
	return tam;
}

int isExtern(char* fonte,int j) {
	int i=0;
	char valor[21];

	while(fonte[j] == '\n' || fonte[j] == '\t' || fonte[j] == ' ' || fonte[j] == '\r' || fonte[j]=='+' || fonte[j]==',') {
		j++;
	}
	while(fonte[j] != '\n' && fonte[j] != '\t' && fonte[j] != ' ' && fonte[j] != '\r' && fonte[j] != '\0') {
		valor[i] = fonte[j];
		i++;
		j++;
	}
	valor[i] = '\0';
	if(strcmp(valor,"EXTERN")==0) {
		return 1;
	}
	return 0;
}

long int getConstValue(char* fonte,int j) {
	int i=0;
	char valor[21];
	int ehconst=0;

	//primeiro token
	while(fonte[j] == '\n' || fonte[j] == '\t' || fonte[j] == ' ' || fonte[j] == '\r' || fonte[j]=='+' || fonte[j]==',') {
		j++;
	}
	while(fonte[j] != '\n' && fonte[j] != '\t' && fonte[j] != ' ' && fonte[j] != '\r' && fonte[j] != '\0') {
		valor[i] = fonte[j];
		i++;
		j++;
	}
	valor[i] = '\0';
	if(strcmp(valor,"CONST")==0) {
		ehconst=1;
	}
	i=0;

	//segundo token (onde esta o numero)
	while(fonte[j] == '\n' || fonte[j] == '\t' || fonte[j] == ' ' || fonte[j] == '\r' || fonte[j] == '\0' || fonte[j]=='+' || fonte[j]==',') {
		j++;
	}
	while(fonte[j] != '\n' && fonte[j] != '\t' && fonte[j] != ' ' && fonte[j] != '\r' && fonte[j] != '\0') {
		valor[i] = fonte[j];
		i++;
		j++;
	}
	valor[i] = '\0';
	if (ehconst) {
		if((valor[1]=='X')||(valor[2]=='X')||(valor[1]=='x')||(valor[2]=='x')) {
			return strtol(valor,NULL,0);
		} else {
			return atoi(valor);
		}
	}
	return -99999;
}



void segPassagem(char* fonte, SymbTable* tSymb, int tamSymb, char* token, int endf, FILE *fileOUT) {
	int i=0;
	long int j=0;
	int pos=0;
	char auxtoken[20];
	int tam_fonte=strlen(fonte);
	char buffer[150]="";
	char bitsmap[150]="";
	char buffertexto[500]="";
	char texto[500]="";
	char bufferdef[100]="";
	char def[100]="";
	sprintf(bitsmap,"\nR: ");
	pos+=getToken(fonte, &pos, token);
	//fprintf(fileOUT,"\nR:");
	while(pos<tam_fonte) { //NAO PODE SER tam_fonte -1 pois precisa executar a funcao pro ultimo token
		noval=0;
		offset=0;
		if(strcmp(token,"PUBLIC")==0){
			pos+=getToken(fonte, &pos, token);
			for(i=0; i<tamSymb; i++) {
				if(strcmp(token,tSymb[i].symbol)==0) {
					sprintf(bufferdef,"\nD: %s %d",tSymb[i].symbol,tSymb[i].end);
					strcat(def,bufferdef);
				}
			}
		}
		if(strcmp(token,"ADD")==0) {
			pos+=getToken(fonte, &pos, token);
			strcpy(auxtoken,token);
			if (offset) {
				pos+=getToken(fonte, &pos, token);
			}
			if ( (checkEnd(tSymb,tamSymb,auxtoken) + desloc(token) )!=0){ //externo com desloc ou public ou comum
				sprintf(buffer,"01");strcat(bitsmap,buffer);

			}else{
				sprintf(buffer,"00");strcat(bitsmap,buffer);
			}
			sprintf(buffertexto,"\nT: 1 %d",checkEnd(tSymb,tamSymb,auxtoken) + desloc(token));strcat(texto,buffertexto);
		} else if(strcmp(token,"SUB")==0) {
			pos+=getToken(fonte, &pos, token);
			strcpy(auxtoken,token);
			if (offset) {
				pos+=getToken(fonte, &pos, token);
			}
			if ( (checkEnd(tSymb,tamSymb,auxtoken) + desloc(token) )!=0){ //externo com desloc ou public ou comum
				sprintf(buffer,"01");strcat(bitsmap,buffer);
			}else{
				sprintf(buffer,"00");strcat(bitsmap,buffer);
			}
			sprintf(buffertexto,"\nT: 2 %d",checkEnd(tSymb,tamSymb,auxtoken) + desloc(token));strcat(texto,buffertexto);
		} else if(strcmp(token,"MULT")==0) {
			pos+=getToken(fonte, &pos, token);
			strcpy(auxtoken,token);
			if (offset) {
				pos+=getToken(fonte, &pos, token);
			}
			if ( (checkEnd(tSymb,tamSymb,auxtoken) + desloc(token) )!=0){ //externo com desloc ou public ou comum
				sprintf(buffer,"01");strcat(bitsmap,buffer);
			}else{
				sprintf(buffer,"00");strcat(bitsmap,buffer);
			}
			sprintf(buffertexto,"\nT: 3 %d",checkEnd(tSymb,tamSymb,auxtoken) + desloc(token));strcat(texto,buffertexto);
		} else if(strcmp(token,"DIV")==0) {
			pos+=getToken(fonte, &pos, token);
			strcpy(auxtoken,token);
			if (offset) {
				pos+=getToken(fonte, &pos, token);
			}
			if ( (checkEnd(tSymb,tamSymb,auxtoken) + desloc(token) )!=0){ //externo com desloc ou public ou comum
				sprintf(buffer,"01");strcat(bitsmap,buffer);
			}else{
				sprintf(buffer,"00");strcat(bitsmap,buffer);
			}
			sprintf(buffertexto,"\nT: 4 %d",checkEnd(tSymb,tamSymb,auxtoken) + desloc(token));strcat(texto,buffertexto);
		} else if(strcmp(token,"JMP")==0) {
			pos+=getToken(fonte, &pos, token);
			strcpy(auxtoken,token);
			if (offset) {
				pos+=getToken(fonte, &pos, token);
			}
			if ( (checkEnd(tSymb,tamSymb,auxtoken) + desloc(token) )!=0){ //externo com desloc ou public ou comum
				sprintf(buffer,"01");strcat(bitsmap,buffer);
			}else{
				sprintf(buffer,"00");strcat(bitsmap,buffer);
			}
			sprintf(buffertexto,"\nT: 5 %d",checkEnd(tSymb,tamSymb,auxtoken) + desloc(token));strcat(texto,buffertexto);
		} else if(strcmp(token,"JMPN")==0) {
			pos+=getToken(fonte, &pos, token);
			strcpy(auxtoken,token);
			if (offset) {
				pos+=getToken(fonte, &pos, token);
			}
			if ( (checkEnd(tSymb,tamSymb,auxtoken) + desloc(token) )!=0){ //externo com desloc ou public ou comum
				sprintf(buffer,"01");strcat(bitsmap,buffer);
			}else{
				sprintf(buffer,"00");strcat(bitsmap,buffer);
			}
			sprintf(buffertexto,"\nT: 6 %d",checkEnd(tSymb,tamSymb,auxtoken) + desloc(token));strcat(texto,buffertexto);
		} else if(strcmp(token,"JMPP")==0) {
			pos+=getToken(fonte, &pos, token);
			strcpy(auxtoken,token);
			if (offset) {
				pos+=getToken(fonte, &pos, token);
			}
			if ( (checkEnd(tSymb,tamSymb,auxtoken) + desloc(token) )!=0){ //externo com desloc ou public ou comum
				sprintf(buffer,"01");strcat(bitsmap,buffer);
			}else{
				sprintf(buffer,"00");strcat(bitsmap,buffer);
			}
			sprintf(buffertexto,"\nT: 7 %d",checkEnd(tSymb,tamSymb,auxtoken) + desloc(token));strcat(texto,buffertexto);
		} else if(strcmp(token,"JMPZ")==0) {
			pos+=getToken(fonte, &pos, token);
			strcpy(auxtoken,token);
			if (offset) {
				pos+=getToken(fonte, &pos, token);
			}
			if ( (checkEnd(tSymb,tamSymb,auxtoken) + desloc(token) )!=0){ //externo com desloc ou public ou comum
				sprintf(buffer,"01");strcat(bitsmap,buffer);
			}else{
				sprintf(buffer,"00");strcat(bitsmap,buffer);
			}
			sprintf(buffertexto,"\nT: 8 %d",checkEnd(tSymb,tamSymb,auxtoken) + desloc(token));strcat(texto,buffertexto);
		} else if(strcmp(token,"COPY")==0) {
			sprintf(buffertexto,"\nT: 9");strcat(texto,buffertexto);
			pos+=getToken(fonte, &pos, token);
			strcpy(auxtoken,token);
			if (offset) {
				pos+=getToken(fonte, &pos, token);
			}
			if ( (checkEnd(tSymb,tamSymb,auxtoken) + desloc(token) )!=0){ //externo com desloc ou public ou comum
				sprintf(buffer,"01");strcat(bitsmap,buffer);
			}else{
				sprintf(buffer,"00");strcat(bitsmap,buffer);
			}
			sprintf(buffertexto," %d",checkEnd(tSymb,tamSymb,auxtoken) + desloc(token));strcat(texto,buffertexto);
			pos+=getToken(fonte, &pos, token);
			strcpy(auxtoken,token);
			if (offset) {
				pos+=getToken(fonte, &pos, token);
			}
			if ( (checkEnd(tSymb,tamSymb,auxtoken) + desloc(token) )!=0){ //externo com desloc ou public ou comum
				sprintf(buffer,"1");strcat(bitsmap,buffer);
			}else{
				sprintf(buffer,"0");strcat(bitsmap,buffer);
			}
			sprintf(buffertexto," %d",checkEnd(tSymb,tamSymb,auxtoken) + desloc(token));strcat(texto,buffertexto);
		} else if(strcmp(token,"LOAD")==0) {
			pos+=getToken(fonte, &pos, token);
			strcpy(auxtoken,token);
			if (offset) {
				pos+=getToken(fonte, &pos, token);
			}
			if ( (checkEnd(tSymb,tamSymb,auxtoken) + desloc(token) )!=0){ //externo com desloc ou public ou comum
				sprintf(buffer,"01");strcat(bitsmap,buffer);
			}else{
				sprintf(buffer,"00");strcat(bitsmap,buffer);
			}
			sprintf(buffertexto,"\nT: 10 %d",checkEnd(tSymb,tamSymb,auxtoken) + desloc(token));strcat(texto,buffertexto);
		} else if(strcmp(token,"STORE")==0) {
			pos+=getToken(fonte, &pos, token);
			strcpy(auxtoken,token);
			if (offset) {
				pos+=getToken(fonte, &pos, token);
			}
			if ( (checkEnd(tSymb,tamSymb,auxtoken) + desloc(token) )!=0){ //externo com desloc ou public ou comum
				sprintf(buffer,"01");strcat(bitsmap,buffer);
			}else{
				sprintf(buffer,"00");strcat(bitsmap,buffer);
			}
			sprintf(buffertexto,"\nT: 11 %d",checkEnd(tSymb,tamSymb,auxtoken) + desloc(token));strcat(texto,buffertexto);
		} else if(strcmp(token,"INPUT")==0) {
			pos+=getToken(fonte, &pos, token);
			strcpy(auxtoken,token);
			if (offset) {
				pos+=getToken(fonte, &pos, token);
			}
			if ( (checkEnd(tSymb,tamSymb,auxtoken) + desloc(token) )!=0){ //externo com desloc ou public ou comum
				sprintf(buffer,"01");strcat(bitsmap,buffer);
			}else{
				sprintf(buffer,"00");strcat(bitsmap,buffer);
			}
			sprintf(buffertexto,"\nT: 12 %d",checkEnd(tSymb,tamSymb,auxtoken) + desloc(token));strcat(texto,buffertexto);
		} else if(strcmp(token,"OUTPUT")==0) {
			pos+=getToken(fonte, &pos, token);
			strcpy(auxtoken,token);
			if (offset) {
				pos+=getToken(fonte, &pos, token);
			}
			if ( (checkEnd(tSymb,tamSymb,auxtoken) + desloc(token) )!=0){ //externo com desloc ou public ou comum
				sprintf(buffer,"01");strcat(bitsmap,buffer);
			}else{
				sprintf(buffer,"00");strcat(bitsmap,buffer);
			}
			sprintf(buffertexto,"\nT: 13 %d",checkEnd(tSymb,tamSymb,auxtoken) + desloc(token));strcat(texto,buffertexto);
		} else if(strcmp(token,"STOP")==0) {
			sprintf(buffertexto,"\nT: 14");strcat(texto,buffertexto);
			sprintf(buffer,"0");strcat(bitsmap,buffer);
			pos+=getToken(fonte, &pos, token);
		} else if(strcmp(token,"SPACE")==0) {
			pos+=getToken(fonte, &pos, token);
			j=atoi(token);
			if (j==0) {
				j=1;
			}
			for(i=1; i<=j; i++) {
				sprintf(buffertexto,"\nT: 0");strcat(texto,buffertexto);
				sprintf(buffer,"0");strcat(bitsmap,buffer);
			}
		} else if(strcmp(token,"CONST")==0) {
			sprintf(buffer,"0");strcat(bitsmap,buffer);
			pos+=getToken(fonte, &pos, token);
			if((token[1]=='X')||(token[2]=='X')||(token[1]=='x')||(token[2]=='x')) { // se for negativo, a posicao 2 conterá o X
				j=strtol(token,NULL,0);
			} else {
				j=atoi(token);
			}
			sprintf(buffertexto,"\nT: %ld",j);strcat(texto,buffertexto);
		} else {
			pos+=getToken(fonte, &pos, token);

		}
	}
	fprintf(fileOUT,"%s",def);
	fprintf(fileOUT,"%s",bitsmap);
	fprintf(fileOUT,"%s",texto);
}

int monta(char* fonte, FILE *fileOUT, int numprog) {
	int tamSymb=0;
	SymbTable *tSymb = (SymbTable*) malloc(sizeof(SymbTable));
	char token[21];
	char auxtoken[21];
	int endereco=0;
	long int const_value=-99999; //j eh diferente de -99999 sempre que eh const
	int space_size=0;
	int pos=0;
	int tam_fonte=strlen(fonte);
	int tam_token;
	int i;
	char bufferuso[100]="";
	char uso[100]="";
	int begin=0,end=0;

	while(pos<tam_fonte) { //se der ruim, mudar para pos<tam_fonte-1
		pos+=getToken(fonte, &pos, token);
		if(strcmp(token,"ADD")==0) {
			qtd+=2;
			endereco+=1;
			pos+=getToken(fonte, &pos, token);
			for(i=0; i<tamSymb; i++) {
				if(strcmp(token,tSymb[i].symbol)==0) {
					if(tSymb[i].externo==1){
						sprintf(bufferuso,"\nU: %s %d",token,endereco);strcat(uso,bufferuso);
					}
				}
			}
			endereco+=1;
		} else if(strcmp(token,"SUB")==0) {
			qtd+=2;
			endereco+=1;
			pos+=getToken(fonte, &pos, token);
			for(i=0; i<tamSymb; i++) {
				if(strcmp(token,tSymb[i].symbol)==0) {
					if(tSymb[i].externo==1){
						sprintf(bufferuso,"\nU: %s %d",token,endereco);strcat(uso,bufferuso);
					}
				}
			}
			endereco+=1;
		} else if(strcmp(token,"MULT")==0) {
			qtd+=2;
			endereco+=1;
			pos+=getToken(fonte, &pos, token);
			for(i=0; i<tamSymb; i++) {
				if(strcmp(token,tSymb[i].symbol)==0) {
					if(tSymb[i].externo==1){
						sprintf(bufferuso,"\nU: %s %d",token,endereco);strcat(uso,bufferuso);
					}
				}
			}
			endereco+=1;
		} else if(strcmp(token,"DIV")==0) {
			qtd+=2;
			endereco+=1;
			pos+=getToken(fonte, &pos, token);
			for(i=0; i<tamSymb; i++) {
				if(strcmp(token,tSymb[i].symbol)==0) {
					if(tSymb[i].externo==1){
						sprintf(bufferuso,"\nU: %s %d",token,endereco);strcat(uso,bufferuso);
					}
				}
			}
			endereco+=1;
		} else if(strcmp(token,"JMP")==0) {
			qtd+=2;
			endereco+=1;
			pos+=getToken(fonte, &pos, token);
			for(i=0; i<tamSymb; i++) {
				if(strcmp(token,tSymb[i].symbol)==0) {
					if(tSymb[i].externo==1){
						sprintf(bufferuso,"\nU: %s %d",token,endereco);strcat(uso,bufferuso);
					}
				}
			}
			endereco+=1;
		} else if(strcmp(token,"JMPN")==0) {
			qtd+=2;
			endereco+=1;
			pos+=getToken(fonte, &pos, token);
			for(i=0; i<tamSymb; i++) {
				if(strcmp(token,tSymb[i].symbol)==0) {
					if(tSymb[i].externo==1){
						sprintf(bufferuso,"\nU: %s %d",token,endereco);strcat(uso,bufferuso);
					}
				}
			}
			endereco+=1;
		} else if(strcmp(token,"JMPP")==0) {
			qtd+=2;
			endereco+=1;
			pos+=getToken(fonte, &pos, token);
			for(i=0; i<tamSymb; i++) {
				if(strcmp(token,tSymb[i].symbol)==0) {
					if(tSymb[i].externo==1){
						sprintf(bufferuso,"\nU: %s %d",token,endereco);strcat(uso,bufferuso);
					}
				}
			}
			endereco+=1;
		} else if(strcmp(token,"JMPZ")==0) {
			qtd+=2;
			endereco+=1;
			pos+=getToken(fonte, &pos, token);
			for(i=0; i<tamSymb; i++) {
				if(strcmp(token,tSymb[i].symbol)==0) {
					if(tSymb[i].externo==1){
						sprintf(bufferuso,"\nU: %s %d",token,endereco);strcat(uso,bufferuso);
					}
				}
			}
			endereco+=1;
		} else if(strcmp(token,"COPY")==0) {
			qtd+=3;
			endereco+=1;
			pos+=getToken(fonte, &pos, token);
			for(i=0; i<tamSymb; i++) {
				if(strcmp(token,tSymb[i].symbol)==0) {
					if(tSymb[i].externo==1){
						sprintf(bufferuso,"\nU: %s %d",token,endereco);strcat(uso,bufferuso);
					}
				}
			}
			endereco+=1;
			pos+=getToken(fonte, &pos, token);
			for(i=0; i<tamSymb; i++) {
				if(strcmp(token,tSymb[i].symbol)==0) {
					if(tSymb[i].externo==1){
						sprintf(bufferuso,"\nU: %s %d",token,endereco);strcat(uso,bufferuso);
					}
				}
			}
			endereco+=1;
		} else if(strcmp(token,"LOAD")==0) {
			qtd+=2;
			endereco+=1;
			pos+=getToken(fonte, &pos, token);
			for(i=0; i<tamSymb; i++) {
				if(strcmp(token,tSymb[i].symbol)==0) {
					if(tSymb[i].externo==1){
						sprintf(bufferuso,"\nU: %s %d",token,endereco);strcat(uso,bufferuso);
					}
				}
			}
			endereco+=1;
		} else if(strcmp(token,"STORE")==0) {
			qtd+=2;
			endereco+=1;
			pos+=getToken(fonte, &pos, token);
			for(i=0; i<tamSymb; i++) {
				if(strcmp(token,tSymb[i].symbol)==0) {
					if(tSymb[i].externo==1){
						sprintf(bufferuso,"\nU: %s %d",token,endereco);strcat(uso,bufferuso);
					}
				}
			}
			endereco+=1;
		} else if(strcmp(token,"INPUT")==0) {
			qtd+=2;
			endereco+=1;
			pos+=getToken(fonte, &pos, token);
			for(i=0; i<tamSymb; i++) {
				if(strcmp(token,tSymb[i].symbol)==0) {
					if(tSymb[i].externo==1){
						sprintf(bufferuso,"\nU: %s %d",token,endereco);strcat(uso,bufferuso);
					}
				}
			}
			endereco+=1;
		} else if(strcmp(token,"OUTPUT")==0) {
			qtd+=2;
			endereco+=1;
			pos+=getToken(fonte, &pos, token);
			for(i=0; i<tamSymb; i++) {
				if(strcmp(token,tSymb[i].symbol)==0) {
					if(tSymb[i].externo==1){
						sprintf(bufferuso,"\nU: %s %d",token,endereco);strcat(uso,bufferuso);
					}
				}
			}
			endereco+=1;
		} else if(strcmp(token,"STOP")==0) {
			endereco+=1;
			qtd+=1;
		} else if(strcmp(token,"SPACE")==0) {
			pos+=getToken(fonte, &pos, token);
			space_size=atoi(token);
			if (space_size==0) { //caso nao tenha nada depois de SPACE
				space_size=1;
			}
			endereco+=space_size;
			qtd+=space_size;
		} else if(strcmp(token,"CONST")==0) {
			qtd+=1;
			endereco+=1;
			pos+=getToken(fonte, &pos, token);
		}
		tam_token=strlen(token);
		if (token[tam_token-1]==':') { // se eh rotulo
			const_value=getConstValue(fonte,pos);
			insertTable(tSymb, &tamSymb, token, endereco, const_value, isExtern(fonte,pos));
		}

		const_value=-99999; //const_value eh -99999 sempre que nao eh const
		space_size=0;

		strcpy(auxtoken,token);
		strcat(auxtoken," ");

		if( strcmp(token,"BEGIN")==0 ) {
			begin++;
		}

		if( strcmp(token,"END")==0 ) {
			end++;
		}
	}
	if ( (begin==0) && (numprog==2)){
		printf("ATENCAO: O programa NAO POSSUI a diretiva BEGIN! Montando mesmo assim...\n");
		//return 0;
	}
	if ( (end==0) && (numprog==2)){
		printf("ATENCAO: O programa NAO POSSUI a diretiva END! Montando mesmo assim...\n");
		//return 0;
	}

	if ( (begin!=0) && (numprog==1)){
		printf("ATENCAO: O programa NAO deveria possuir diretivas BEGIN! Montando mesmo assim...\n");
		//return 0;
	}
	if ( (end!=0) && (numprog==1)){
		printf("ATENCAO: O programa NAO deveria possuir diretivas END! Montando mesmo assim...\n");
		//return 0;
	}

	linham=1;
	fprintf(fileOUT,"H: %d",qtd);
	fprintf(fileOUT,"%s",uso);
	segPassagem(fonte, tSymb, tamSymb, token, endereco, fileOUT);
	free(tSymb);
	return 1;
}

int passar_pra_string(FILE *fileIN, char* fonte, long tam_fonte) { // passa todo o codigo para uma string
	if (!fonte) return 0; //se deu erro de memoria
	fread(fonte, sizeof(char), tam_fonte, fileIN);
	int i;
	for(i=0;i<=tam_fonte;i++){ // converter pra maiuscula
		fonte[i]=toupper(fonte[i]);
	}
	return 1;
	}

int main (int argc, char* argv[]) {
	char file_in[50];
	char file_in2[50];
	char file_out[50];
	char file_out2[50];
	FILE *fileIN, *fileOUT;
	strcpy(file_in,argv[1]);
	strcpy(file_out,argv[1]);
	if((fileIN=fopen(strcat(file_in,".asm"),"r"))==NULL) {
		printf("\n\nERRO! Arquivo 1 não encontrado.\n");
		return 0;
	}
	if((fileOUT=fopen(strcat(file_out,".txt"),"w"))==NULL) {   /* abre o arquivo de saida */
		printf("\n\nERRO! não foi possivel criar arquivo.\n");
		return 0;
	} else {
		char* fonte="";
        fseek(fileIN, 0L, SEEK_END); //posiona o fileIN no final do arquivo
        long tam_fonte = ftell(fileIN); //pega o tamanho
        fseek(fileIN, 0L, SEEK_SET); //posiona o fileIN no inicio do arquivo (mesmo que rewind(fileIN), mas com retorno)
        fonte = (char*) calloc(tam_fonte,sizeof(char)); //alocacao dinamica
        if(!passar_pra_string(fileIN, fonte, tam_fonte)) { //problema na alocacao dinamica (memoria)
            printf("Erro ao ler o fonte 1.\n");
            return 0;
        }
        fclose(fileIN);
        fprintf(fileOUT,"H: %s\n",argv[1]);
        if(!monta(fonte,fileOUT,argc-1)){
        	return 0;
        }
        printf("\nArquivo fonte 1:\n\n%s\n", fonte);
        printf("\nArquivo 1 montado com sucesso!\n\n");
        fclose(fileOUT);
	}
	if (argc>2){
		qtd=0;
		strcpy(file_in2,argv[2]);
		strcpy(file_out2,argv[2]);
		if((fileIN=fopen(strcat(file_in2,".asm"),"r"))==NULL) {
			printf("\n\nERRO! Arquivo 1 não encontrado.\n");
			return 0;
		}
		if((fileOUT=fopen(strcat(file_out2,".txt"),"w"))==NULL) {   /* abre o arquivo de saida */
			printf("\n\nERRO! não foi possivel criar arquivo.\n");
			return 0;
		} else {
			char* fonte="";
	        fseek(fileIN, 0L, SEEK_END); //posiona o fileIN no final do arquivo
	        long tam_fonte = ftell(fileIN); //pega o tamanho
	        fseek(fileIN, 0L, SEEK_SET); //posiona o fileIN no inicio do arquivo (mesmo que rewind(fileIN), mas com retorno)
	        fonte = (char*) calloc(tam_fonte,sizeof(char)); //alocacao dinamica
	        if(!passar_pra_string(fileIN, fonte, tam_fonte)) { //problema na alocacao dinamica (memoria)
	            printf("Erro ao ler o fonte.\n");
	            return 0;
	        }
			fclose(fileIN);
	        fprintf(fileOUT,"H: %s\n",argv[2]);
	        if(!monta(fonte,fileOUT,argc-1)){
				return 0;
			}
	        printf("\nArquivo fonte 2:\n\n%s\n", fonte);
	        printf("\nArquivo 2 montado com sucesso!\n");
	        fclose(fileOUT);
		}
	}

	// fclose(fileIN);
	// fclose(fileOUT);
	return 1;
}
