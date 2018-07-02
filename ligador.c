#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int Factor[3];

typedef struct Table{
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
  }code;
  code *tCode1, *tCode2;
  int tCode1Size=0, tCode2Size;

typedef struct relative{
    int end;
    int rel;
  }relative;
  relative *tRel;
  int tRelSize=0;


void Tabelas_Globais(){                      //criar tabelas globais e mapa de bits globais
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

void Referencias_Crusadas(){
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
                tCode1[i].relativo = tCode1[i].relativo + tRel[j].rel;
              }
            }
          }

          for (i=0;i<tCode2Size;i++) {
            for (j=0;j<tRelSize;j++) {
              if (tCode2[i].endereco==tRel[j].end){
                tCode2[i].relativo = tCode2[i].relativo + tRel[j].rel;
              }
            }
          }
  }

void Aplica_FatorCorrecao(){
      int i,j;
      for (i=0;i<tCode2Size;i++) {
            tCode2[i].relativo = tCode2[i].relativo + Factor[2];
        }
      for (i=0;i<tCode2Size;i++) {
         for (j=0;j<tRelSize;j++) {
           if(tCode2[i].endereco==tRel[j].end){
             tCode2[i].relativo = tCode2[i].relativo - Factor[2];
           }
         }
      }
   }

int main(int argc, char *argv[]){
    char *file_in1=(char*)malloc(256*sizeof(char));   //Primeiro programa
    char *file_in2=(char*)malloc(256*sizeof(char));   //segundo programa
    char *file_out=(char*)malloc(256*sizeof(char));   //Saida do ligador
    FILE *fileIN1,*fileIN2,*fileOUT;
    char rot[100], buffer[512], op1[20], op2[20];
    int i, address1=0, address2=0;
    int flag_end1=0, flag_end2=0;

    if(argc<4){  // achar se os parâmetros passados são
        printf("\nDigite o nome do Primeiro arquivo de entrada: ");
        scanf("%s",file_in1); getchar();
        printf("\nDigite o nome do segundo arquivo de entrada: ");
        scanf("%s",file_in2); getchar();
        printf("\nDigite o nome do arquivo de saida: ");
        scanf("%s",file_out); getchar();
    }
    else{
        file_in1=argv[1];
        file_in2=argv[2];
        file_out=argv[3];
    }

    if((fileIN1=fopen(file_in1,"r"))==NULL){
        printf("\n\tERRO! Arquivo 1 nao encontrado\n\n");
        printf("\n");exit(0);
        return 100;
    }
    if((fileIN2=fopen(file_in2,"r"))==NULL){
        printf("\n\tERRO! Arquivo 2 nao encontrado\n\n");
        printf("\n");exit(0);
        return 100;
    }
    if((fileOUT=fopen(strcat(file_out,".txt"),"w"))==NULL){
        printf("\n\tERRO! Nao foi possivel criar arquivo");
        printf("\n");exit(0);
        return 101;
    }

// Para o arquivo 1 de entrada ler as tabelas de USO, DEFINÇÃO E O CÓDIGO MAQUINA
while(!feof(fileIN1)){
    strcpy(buffer,"");strcpy(rot,"");strcpy(op1,"");strcpy(op2,"");
    if (fgets(buffer, 511, fileIN1)!=NULL){
      sscanf(buffer,"%s %s %s",rot,op1,op2);
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
    //printf("%d\n", Factor[2]);
    fclose(fileIN1);


// Para o arquivo 2 de entrada ler as tabelas de USO, DEFINÇÃO E O CÓDIGO MAQUINA
while(!feof(fileIN2)){
      strcpy(buffer,"");strcpy(rot,"");strcpy(op1,"");strcpy(op2,"");
        if (fgets(buffer, 511, fileIN2)!=NULL){
          sscanf(buffer,"%s %s %s",rot,op1,op2);
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


           for (i=0;i<tRelSize;i++) {
             printf("%d %d\n",tRel[i].end, tRel[i].rel );
           }
         printf("TABELA DE USO GLOBAL");
          for(i=0;i<tUsoGlobalSize;i++)
           printf("\n%s %d",tUsoGlobal[i].symbol,tUsoGlobal[i].address);
            printf("\n\n");

            printf("TABELA DE DEFINÇÃO GLOBAL");
          for(i=0;i<tDefGlobalSize;i++)
              printf("\n%s %d",tDefGlobal[i].symbol,tDefGlobal[i].address);
               printf("\n\n");

              printf("CODE PROG1");
         for(i=0;i<tCode1Size;i++){
              flag_end1++;
              if ((tCode1[i].absoluto == 14)||(tCode1[i].absoluto == 0)){
                  printf("\n%d %d",tCode1[i].endereco,tCode1[i].absoluto);
                  fprintf(fileOUT,"%d ",tCode1[i].absoluto);
                  break;
                }else{
                printf("\n%d %d %d",tCode1[i].endereco, tCode1[i].absoluto, tCode1[i].relativo);
                fprintf(fileOUT,"%d %d ",tCode1[i].absoluto, tCode1[i].relativo);
              }
            }
              for (i=flag_end1;i<tCode1Size;i++) {
                printf("\n%d %d",tCode1[i].endereco,tCode1[i].absoluto);
                fprintf(fileOUT,"%d ",tCode1[i].absoluto);
              }
              printf("\n\n");


            printf("CODE PROG2");
             for(i=0;i<tCode2Size;i++){
               flag_end2++;
                  if ((tCode2[i].absoluto == 14)||(tCode2[i].absoluto == 0)){
                      printf("\n%d %d",tCode2[i].endereco,tCode2[i].absoluto);
                      fprintf(fileOUT,"%d ",tCode2[i].absoluto);
                      break;
                    }else{
                    printf("\n%d %d %d",tCode2[i].endereco, tCode2[i].absoluto, tCode2[i].relativo);
                    fprintf(fileOUT,"%d %d ",tCode2[i].absoluto, tCode2[i].relativo);
                  }
                }

                for (i=flag_end2;i<tCode2Size;i++) {
                  printf("\n%d %d",tCode2[i].endereco,tCode2[i].absoluto);
                  fprintf(fileOUT,"%d ",tCode2[i].absoluto);
                }

           printf("\n");
           fclose(fileOUT);


 return 0;
 }
