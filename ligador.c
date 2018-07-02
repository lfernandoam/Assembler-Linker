/*Universidade de Brasília
Departamento de Ciência da Computação
Software Básico - Turma B - 2018/1
Professor Bruno L. Macchiavello

Sistema Operacional:         Ubuntu 16.4 LTS
Para Compilação:             gcc ligador.c -o ligador -wall
Um Arquivo de Entrada:      ./ligador arq_entrada arq_Saida
Dois Arquivos de Entrada:   ./ligador arq_entrada1 arq_entrada2 arq_Saida

   Trabalho 2 - Item 2 do Trabalho 2 - Ligador

GRUPO: LUIS FERNANDO ARRUDA MARQUES - 15/0016271
       NASIRU ADAMU MARAFA          - 11/008055    */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


int Factor[3];
typedef struct Table{    // Criar Tabelas, Uso e Definição
    char symbol[100];
    int address;
  }table;
  table *tDef1, *tDef2, *tDefGlobal;
  table *tUso1, *tUso2, *tUsoGlobal;
  int tDef1Size=0, tDef2Size=0, tDefGlobalSize=0;
  int tUso1Size=0, tUso2Size=0, tUsoGlobalSize=0;
typedef struct code{
    int endereco;
    int absoluto;
    int relativo;
    int relativo2;
  }code;
  code *tCode1, *tCode2;
  int tCode1Size=0, tCode2Size;
typedef struct relative{
    int end;
    int rel;
  }relative;
  relative *tRel;
  int tRelSize=0;


void Tabelas_Globais(){    //criar tabelas globais de Uso e Definição
      int i,j;
      tDefGlobalSize=tDef1Size+tDef2Size;
      tDefGlobal=(table*)malloc(tDefGlobalSize*sizeof(table));
      for(i=0;i<tDef1Size;i++){
          strcpy(tDefGlobal[i].symbol,tDef1[i].symbol);
          tDefGlobal[i].address=tDef1[i].address+Factor[1];
      }
      for(i=i,j=0;i<tDefGlobalSize;i++,j++){
          strcpy(tDefGlobal[i].symbol,tDef2[j].symbol);
          tDefGlobal[i].address=tDef2[j].address+Factor[2];
      }
      tUsoGlobalSize=tUso1Size+tUso2Size;
      tUsoGlobal=(table*)malloc(tUsoGlobalSize*sizeof(table));
      for(i=0;i<tUso1Size;i++){
          strcpy(tUsoGlobal[i].symbol,tUso1[i].symbol);
          tUsoGlobal[i].address=tUso1[i].address+Factor[1];
      }
      for(i=i,j=0;i<tUsoGlobalSize;i++,j++){
          strcpy(tUsoGlobal[i].symbol,tUso2[j].symbol);
          tUsoGlobal[i].address=tUso2[j].address+Factor[2];
      }
  }
void Referencias_Crusadas(){  //Resolver as Referencias Cruzadas
      int i,j;

      for(i=0;i<tUsoGlobalSize;i++){
          for(j=0;j<tDefGlobalSize;j++){
            if(strcmp(tUsoGlobal[i].symbol,tDefGlobal[j].symbol)==0){
            tRelSize++;
            tRel=realloc(tRel,(tRelSize)*sizeof(relative));
            tRel[tRelSize-1].end = tUsoGlobal[i].address-1;
            tRel[tRelSize-1].rel = tDefGlobal[j].address;
            }
            }
          }


          for (i=0;i<tCode1Size;i++) {
            for (j=0;j<tRelSize;j++) {
              if (tCode1[i].endereco==tRel[j].end){
                  if(tCode1[i].absoluto==9){
                    tCode1[i].relativo = tCode1[i].relativo + tRel[j].rel;
                    tCode1[i].relativo2 = tCode1[i].relativo2 + tRel[j].rel-1;
                  }else{
                    tCode1[i].relativo = tCode1[i].relativo + tRel[j].rel;
                  }

              }
            }
          }

          for (i=0;i<tCode2Size;i++) {
            for (j=0;j<tRelSize;j++) {
              if (tCode2[i].endereco==tRel[j].end){
                if(tCode2[i].absoluto==9){
                  tCode2[i].relativo = tCode2[i].relativo + tRel[j].rel;
                  tCode2[i].relativo2 = tCode2[i].relativo2 + tRel[j].rel-1;
                }else{
                  tCode2[i].relativo = tCode2[i].relativo + tRel[j].rel;
                }
              }
            }
          }
  }
void Aplica_FatorCorrecao(){    //Aplicar o Factor de Correção
      int i,j;
      for (i=0;i<tCode2Size;i++) {
            tCode2[i].relativo = tCode2[i].relativo + Factor[2];
            tCode2[i].relativo2 = tCode2[i].relativo2 + Factor[2];
        }
      for (i=0;i<tCode2Size;i++) {
         for (j=0;j<tRelSize;j++) {
           if(tCode2[i].endereco==tRel[j].end){
             tCode2[i].relativo = tCode2[i].relativo - Factor[2];
             tCode2[i].relativo2 = tCode2[i].relativo2 - Factor[2];
           }
         }
      }
   }

int main(int argc, char *argv[]){
    char file_in1[50];
    char file_in2[50];
    char file_out[50];
    FILE *fileIN1,*fileIN2,*fileOUT;
    char rot[100], buffer[512], op1[20], op2[20], op3[20];
    int i,  address1=0, address2=0;
    int flag_end1=0, flag_end2=0;


    if(argc<3){  // achar se os parâmetros passados são
        printf("Quantidade de arquivos invalida (ESPECIFIQUE ENTRADA E SAIDA)\n");
    }
    else{
        if(argc==4){
          strcpy(file_in1,argv[1]);
          strcpy(file_in2,argv[2]);
          strcpy(file_out,argv[3]);
        }
        if(argc==3){
          strcpy(file_in1,argv[1]);
          strcpy(file_out,argv[2]);
        }

    }
  if(argc==4){   /* FAZ A LIGAÇÃO DE ARQUIVO SE ARQUIVOS DA ENTRADA FOR DOIS*/
    if((fileIN1=fopen(strcat(file_in1,".txt"),"r"))==NULL){
        printf("\n\tERRO! Arquivo 1 nao encontrado.\n\n");
        printf("\n");exit(0);
        return 100;
    }
    if((fileIN2=fopen(strcat(file_in2,".txt"),"r"))==NULL){
        printf("\n\tERRO! Arquivo 2 nao encontrado.\n\n");
        printf("\n");exit(0);
        return 100;
    }
    if((fileOUT=fopen(strcat(file_out,".o"),"w"))==NULL){
        printf("\n\tERRO! Nao foi possivel criar arquivo.");
        printf("\n");exit(0);
        return 101;
    }

    // Para o arquivo 1 de entrada ler as tabelas de USO, DEFINÇÃO E O CÓDIGO MAQUINA
    while(!feof(fileIN1)){
        strcpy(buffer,"");strcpy(rot,"");strcpy(op1,"");strcpy(op2,"");strcpy(op3,"");
        if (fgets(buffer, 511, fileIN1)!=NULL){
          sscanf(buffer,"%s %s %s %s",rot,op1,op2, op3);
            if (strcmp(rot,"U:")==0) {
              tUso1Size++;
              tUso1=realloc(tUso1,(tUso1Size)*sizeof(table));
              strcpy(tUso1[tUso1Size-1].symbol,op1);
              tUso1[tUso1Size-1].address = atoi(op2);
              }

           if (strcmp(rot,"D:")==0) {
             tDef1Size++;
             tDef1=realloc(tDef1,(tDef1Size)*sizeof(table));
             strcpy(tDef1[tDef1Size-1].symbol,op1);
             tDef1[tDef1Size-1].address = atoi(op2);
             }

          if (strcmp(rot,"T:")==0) {
              tCode1Size++;
              tCode1=realloc(tCode1,(tCode1Size)*sizeof(code));
              tCode1[tCode1Size-1].endereco = address1;
              tCode1[tCode1Size-1].absoluto = atoi(op1);
              tCode1[tCode1Size-1].relativo = atoi(op2);
              tCode1[tCode1Size-1].relativo2 = atoi(op3);
              if (tCode1[tCode1Size-1].absoluto==9) {
                address1=address1+3;
              }else if (tCode1[tCode1Size-1].absoluto==0) {
                address1=address1+1;
              }else if (tCode1[tCode1Size-1].absoluto==14) {
                address1=address1+1;
              }
              else{
                if (strcmp(op2,"")==0) {
                  address1=address1+1;
                }else{
                address1=address1+2;
                }
              }
            }
          }
    }

    Factor[1]=0;
    Factor[2]=address1;
    address2=address1;
    fclose(fileIN1);


    // Para o arquivo 2 de entrada ler as tabelas de USO, DEFINÇÃO E O CÓDIGO MAQUINA
    while(!feof(fileIN2)){
          strcpy(buffer,"");strcpy(rot,"");strcpy(op1,"");strcpy(op2,"");strcpy(op3,"");
            if (fgets(buffer, 511, fileIN2)!=NULL){
              sscanf(buffer,"%s %s %s %s",rot,op1,op2,op3);
                  if (strcmp(rot,"U:")==0) {
                    tUso2Size++;
                    tUso2=realloc(tUso2,(tUso2Size)*sizeof(table));
                    strcpy(tUso2[tUso2Size-1].symbol,op1);
                    tUso2[tUso2Size-1].address = atoi(op2);
                    }

                 if (strcmp(rot,"D:")==0) {
                   tDef2Size++;
                   tDef2=realloc(tDef2,(tDef2Size)*sizeof(table));
                   strcpy(tDef2[tDef2Size-1].symbol,op1);
                   tDef2[tDef2Size-1].address = atoi(op2);
                   }

                   if (strcmp(rot,"T:")==0) {
                       tCode2Size++;
                       tCode2=realloc(tCode2,(tCode2Size)*sizeof(code));
                       tCode2[tCode2Size-1].endereco = address2;
                       tCode2[tCode2Size-1].absoluto = atoi(op1);
                       tCode2[tCode2Size-1].relativo = atoi(op2);
                       tCode2[tCode2Size-1].relativo2 = atoi(op3);
                       if (tCode2[tCode2Size-1].absoluto==9) {
                         address2=address2+3;
                       }else if (tCode2[tCode2Size-1].absoluto==0) {
                         address2=address2+1;
                       }else if (tCode2[tCode2Size-1].absoluto==14) {
                         address2=address2+1;
                       }
                       else{
                         if (strcmp(op2,"")==0) {
                           address2=address2+1;
                         }else{
                         address2=address2+2;
                        }
                       }
                     }
                   }
            }
            fclose(fileIN2);   // depois de ler o arquivo e suas respectivas tabelas, comecar a ligacao seguindo receita de bolo

            Tabelas_Globais();
            Referencias_Crusadas();
            Aplica_FatorCorrecao();

         printf("\nArquivo %s sera encontrado com o codigo ligado.\n\n", file_out);
         for(i=0;i<tCode1Size;i++){
              flag_end1++;
              if ((tCode1[i].absoluto == 14)||(tCode1[i].absoluto == 0)){
                  printf("%d ",tCode1[i].absoluto);
                  fprintf(fileOUT,"%d ",tCode1[i].absoluto);
                  break;
                }else{
                  if(tCode1[i].absoluto==9){
                    printf("%d %d %d ", tCode1[i].absoluto,tCode1[i].relativo,tCode1[i].relativo2);
                    fprintf(fileOUT,"%d %d %d ",tCode1[i].absoluto,tCode1[i].relativo,tCode1[i].relativo2);
                  }else{
                    printf("%d %d ", tCode1[i].absoluto,tCode1[i].relativo);
                    fprintf(fileOUT,"%d %d ",tCode1[i].absoluto,tCode1[i].relativo);
                  }
              }
            }
              for (i=flag_end1;i<tCode1Size;i++) {
                printf("%d ",tCode1[i].absoluto);
                fprintf(fileOUT,"%d ",tCode1[i].absoluto);
              }



             for(i=0;i<tCode2Size;i++){
               flag_end2++;
                  if ((tCode2[i].absoluto == 14)||(tCode2[i].absoluto == 0)){
                      printf("%d ",tCode2[i].absoluto);
                      fprintf(fileOUT,"%d ",tCode2[i].absoluto);
                      break;
                    }else{
                      if(tCode1[i].absoluto==9){
                        printf("%d %d %d ",tCode2[i].absoluto,tCode2[i].relativo,tCode2[i].relativo2);
                        fprintf(fileOUT,"%d %d %d ",tCode2[i].absoluto,tCode2[i].relativo,tCode2[i].relativo2);
                      }else{
                        printf("%d %d ",tCode2[i].absoluto,tCode2[i].relativo);
                        fprintf(fileOUT,"%d %d ",tCode2[i].absoluto,tCode2[i].relativo);
                      }

                  }
                }

                for (i=flag_end2;i<tCode2Size;i++) {
                  printf("%d ",tCode2[i].absoluto);
                  fprintf(fileOUT,"%d ",tCode2[i].absoluto);
                }
                printf("\n\n");

           fclose(fileOUT);
         }
  if(argc==3){     /* FAZ A LIGAÇÃO DE ARQUIVO SE ARQUIVO DA ENTRADA FOR ÚNICO*/
      if((fileIN1=fopen(strcat(file_in1,".txt"),"r"))==NULL){
        printf("\n\tERRO! Arquivo 1 nao encontrado\n\n");
        printf("\n");exit(0);
        return 100;
      }
      if((fileOUT=fopen(strcat(file_out,".o"),"w"))==NULL){
        printf("\n\tERRO! Nao foi possivel criar arquivo");
        printf("\n");exit(0);
        return 101;
      }

    // Para o arquivo 1 de entrada ler as tabelas de USO, DEFINÇÃO E O CÓDIGO MAQUINA
    while(!feof(fileIN1)){
            strcpy(buffer,"");strcpy(rot,"");strcpy(op1,"");strcpy(op2,"");strcpy(op3,"");
            if (fgets(buffer, 511, fileIN1)!=NULL){
              sscanf(buffer,"%s %s %s %s",rot,op1,op2, op3);
              if (strcmp(rot,"T:")==0) {
                  tCode1Size++;
                  tCode1=realloc(tCode1,(tCode1Size)*sizeof(code));
                  tCode1[tCode1Size-1].endereco = address1;
                  tCode1[tCode1Size-1].absoluto = atoi(op1);
                  tCode1[tCode1Size-1].relativo = atoi(op2);
                  tCode1[tCode1Size-1].relativo2 = atoi(op3);
                }
              }
            }
            fclose(fileIN1);
        printf("\nArquivo %s sera encontrado com o codigo ligado.\n\n", file_out);
        for(i=0;i<tCode1Size;i++){
             flag_end1++;
             if ((tCode1[i].absoluto == 14)||(tCode1[i].absoluto == 0)){
                 printf("%d ",tCode1[i].absoluto);
                 fprintf(fileOUT,"%d ",tCode1[i].absoluto);
                 break;
               }else{
                 if(tCode1[i].absoluto==9){
                   printf("%d %d %d ", tCode1[i].absoluto,tCode1[i].relativo,tCode1[i].relativo2);
                   fprintf(fileOUT,"%d %d %d ",tCode1[i].absoluto,tCode1[i].relativo,tCode1[i].relativo2);
                 }else{
                   printf("%d %d ", tCode1[i].absoluto,tCode1[i].relativo);
                   fprintf(fileOUT,"%d %d ",tCode1[i].absoluto,tCode1[i].relativo);
                 }
             }
           }
             for (i=flag_end1;i<tCode1Size;i++) {
               printf("%d ",tCode1[i].absoluto);
               fprintf(fileOUT,"%d ",tCode1[i].absoluto);
             }
             printf("\n\n");

             fclose(fileOUT);
           }
    return 0;
  }
