#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tj.h"


void concatenarArqvs(){
    char *caminhos[] = {
    "src/teste_TRE-AC.csv", "src/teste_TRE-AL.csv", "src/teste_TRE-AM.csv",
    "src/teste_TRE-AP.csv", "src/teste_TRE-BA.csv", "src/teste_TRE-CE.csv",
    "src/teste_TRE-DF.csv", "src/teste_TRE-ES.csv", "src/teste_TRE-GO.csv",
    "src/teste_TRE-MA.csv", "src/teste_TRE-MG.csv", "src/teste_TRE-MS.csv",
    "src/teste_TRE-MT.csv", "src/teste_TRE-PA.csv", "src/teste_TRE-PB.csv",
    "src/teste_TRE-PE.csv", "src/teste_TRE-PI.csv", "src/teste_TRE-PR.csv",
    "src/teste_TRE-RJ.csv", "src/teste_TRE-RN.csv", "src/teste_TRE-RO.csv",
    "src/teste_TRE-RR.csv", "src/teste_TRE-RS.csv", "src/teste_TRE-SC.csv",
    "src/teste_TRE-SE.csv", "src/teste_TRE-SP.csv", "src/teste_TRE-TO.csv"
    };
    FILE *saida = fopen("arqConcatenado.csv", "w");
    if (!saida)
    {
        printf("\nERRO AO CRIAR ARQUIVO DE SAIDA!");
        return;
    }
    

    char buffer[2048];
    int tamCam = sizeof(caminhos)/sizeof(caminhos[0]);

    for (int i = 0; i < tamCam; i++)
    {
        FILE *entrada = fopen(caminhos[i], "r");
        if (!entrada)
        {
            printf("\nERRO AO CRIAR ARQUIVO DE SAIDA!");
        return;
        }

        if (i>0)
        {
            fgets(buffer, sizeof(buffer), entrada);
        }

        while (fgets(buffer, sizeof(buffer), entrada) != NULL)
        {
            fputs(buffer, saida);
        }

        fclose(entrada);
        
    }
    fclose(saida);
    
}


void calculo(char *arqConq){
    FILE *arqConcatenado = fopen(arqConq, "r");
    if (!arqConcatenado)
    {
        printf("FALHA AO ABRIR ARQUIVO CONCATERNADO!"); exit(1);
    }

    char linha[256];
    int suspm4_b = 0, julgm4_b = 0, distm4_b = 0, suspm4_a = 0, julgm4_a = 0, distm4_a = 0, suspm2_ant = 0, julgm2_ant = 0, distm2_ant = 0, suspm2_a = 0, julgm2_a = 0, distm2_a = 0, dessobrestados_2026 = 0, suspensos_2026 = 0, julgados_2026 = 0, casos_novos_2026 = 0;

    fgets(linha, sizeof(linha), arqConcatenado);
    
    
}