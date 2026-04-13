#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tj.h"

// Funcao para concatenar os arquivos CSV em um unico arquivo de saida.
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