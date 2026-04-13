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

void gerarResumosDoArquivo(const char *nomeArquivoConcatenado) {
    FILE *arquivo = fopen(nomeArquivoConcatenado, "r");
    if (!arquivo) {
        printf("\nERRO: Nao foi possivel abrir o arquivo %s para gerar resumos.\n", nomeArquivoConcatenado);
        return;
    }

    Calculos tribunais[30] = {0}; 
    int numTribunais = 0;
    char buffer[2048];
    int is_first_line = 1;

    while (fgets(buffer, sizeof(buffer), arquivo) != NULL) {
        if (is_first_line) {
            is_first_line = 0;
            continue; // Pula a linha de cabeçalho
        }

        char sigla_lida[20] = {0};
        Calculos linhaAtual = {0};
        
        // =========================================================================
        // PARSING EXATO DAS COLUNAS (Lê a Sigla e os 17 inteiros, ignora o resto)
        // =========================================================================
        int lidos = sscanf(buffer,
            "%[^,],%*[^,],%*[^,],%*[^,],%*[^,],%*[^,],%*[^,],%*[^,],%*[^,],%*[^,],"
            "%d,%d,%*[^,],%d,%d,%*[^,],"
            "%d,%d,%d,%*[^,],"
            "%d,%d,%d,%d,%*[^,],"
            "%d,%d,%d,%*[^,],"
            "%d,%d,%d",
            sigla_lida,                      // 1. sigla_tribunal
            &linhaAtual.casos_novos_2026,    // 11. casos_novos_2026
            &linhaAtual.julgados_2026,       // 12. julgados_2026
            // (ignora 13. prim_sent2026)
            &linhaAtual.suspensos_2026,      // 14. suspensos_2026
            &linhaAtual.dessobrestados_2026, // 15. dessobrestados_2026
            // (ignora 16. cumprimento_meta1)
            &linhaAtual.distm2_a,            // 17. distm2_a
            &linhaAtual.julgm2_a,            // 18. julgm2_a
            &linhaAtual.suspm2_a,            // 19. suspm2_a
            // (ignora 20. cumprimento_meta2a)
            &linhaAtual.distm2_ant,          // 21. distm2_ant
            &linhaAtual.julgm2_ant,          // 22. julgm2_ant
            &linhaAtual.suspm2_ant,          // 23. suspm2_ant
            &linhaAtual.desom2_ant,          // 24. desom2_ant
            // (ignora 25. cumprimento_meta2ant)
            &linhaAtual.distm4_a,            // 26. distm4_a
            &linhaAtual.julgm4_a,            // 27. julgm4_a
            &linhaAtual.suspm4_a,            // 28. suspm4_a
            // (ignora 29. cumprimento_meta4a)
            &linhaAtual.distm4_b,            // 30. distm4_b
            &linhaAtual.julgm4_b,            // 31. julgm4_b
            &linhaAtual.suspm4_b             // 32. suspm4_b
            // (a 33. cumprimento_meta4b é o fim da linha e fica ignorada)
        );
        
        // Se a leitura for mal sucedida (ex: linha em branco no fim do arquivo) ele pula
        if (lidos < 18) {
            continue; 
        }

        // --- ARMAZENAMENTO DOS DADOS NO ARRAY DO TRIBUNAL ---
        int index = -1;
        for (int i = 0; i < numTribunais; i++) {
            if (strcmp(tribunais[i].sigla_tribunal, sigla_lida) == 0) {
                index = i;
                break;
            }
        }

        if (index == -1) {
            index = numTribunais;
            strcpy(tribunais[index].sigla_tribunal, sigla_lida);
            numTribunais++;
        }

        tribunais[index].julgados_2026 += linhaAtual.julgados_2026;
        tribunais[index].casos_novos_2026 += linhaAtual.casos_novos_2026;
        tribunais[index].dessobrestados_2026 += linhaAtual.dessobrestados_2026;
        tribunais[index].suspensos_2026 += linhaAtual.suspensos_2026;
        tribunais[index].julgm2_a += linhaAtual.julgm2_a;
        tribunais[index].distm2_a += linhaAtual.distm2_a;
        tribunais[index].suspm2_a += linhaAtual.suspm2_a;
        tribunais[index].julgm2_ant += linhaAtual.julgm2_ant;
        tribunais[index].distm2_ant += linhaAtual.distm2_ant;
        tribunais[index].suspm2_ant += linhaAtual.suspm2_ant;
        tribunais[index].desom2_ant += linhaAtual.desom2_ant;
        tribunais[index].julgm4_a += linhaAtual.julgm4_a;
        tribunais[index].distm4_a += linhaAtual.distm4_a;
        tribunais[index].suspm4_a += linhaAtual.suspm4_a;
        tribunais[index].julgm4_b += linhaAtual.julgm4_b;
        tribunais[index].distm4_b += linhaAtual.distm4_b;
        tribunais[index].suspm4_b += linhaAtual.suspm4_b;
    }
    
    fclose(arquivo);

    // =========================================================================
    // ETAPA 2: CÁLCULOS E GERAÇÃO DOS ARQUIVOS NA PASTA "saida"
    // =========================================================================
    for (int i = 0; i < numTribunais; i++) {
        float M1 = 0, M2A = 0, M2Ant = 0, M4A = 0, M4B = 0;

        if ((tribunais[i].casos_novos_2026 + tribunais[i].dessobrestados_2026 - tribunais[i].suspensos_2026) != 0)
            M1 = (float)tribunais[i].julgados_2026 / (tribunais[i].casos_novos_2026 + tribunais[i].dessobrestados_2026 - tribunais[i].suspensos_2026) * 100;
        
        if ((tribunais[i].distm2_a - tribunais[i].suspm2_a) != 0)
            M2A = (float)tribunais[i].julgm2_a / (tribunais[i].distm2_a - tribunais[i].suspm2_a) * (1000.0 / 7.0);
        
        if ((tribunais[i].distm2_ant - tribunais[i].suspm2_ant - tribunais[i].desom2_ant) != 0)
            M2Ant = (float)tribunais[i].julgm2_ant / (tribunais[i].distm2_ant - tribunais[i].suspm2_ant - tribunais[i].desom2_ant) * 100;
        
        if ((tribunais[i].distm4_a - tribunais[i].suspm4_a) != 0)
            M4A = (float)tribunais[i].julgm4_a / (tribunais[i].distm4_a - tribunais[i].suspm4_a) * 100;
        
        if ((tribunais[i].distm4_b - tribunais[i].suspm4_b) != 0)
            M4B = (float)tribunais[i].julgm4_b / (tribunais[i].distm4_b - tribunais[i].suspm4_b) * 100;

        char caminhoResumo[100];
        sprintf(caminhoResumo, "saida/resumo_%s.csv", tribunais[i].sigla_tribunal);
        
        FILE *resumoLocal = fopen(caminhoResumo, "w");
        if (resumoLocal) {
            fprintf(resumoLocal, "Sigla,Total Julgados,Meta1,Meta2A,Meta2Ant,Meta4A,Meta4B\n");
            fprintf(resumoLocal, "%s,%d,%.2f,%.2f,%.2f,%.2f,%.2f\n", 
                    tribunais[i].sigla_tribunal, tribunais[i].julgados_2026, M1, M2A, M2Ant, M4A, M4B);
            fclose(resumoLocal);
        }
    }
}