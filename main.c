#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tj.h"

int main() {
    int opcao;
    char municipio[100];
    char lixo[10];

    concatenarArqvs();
    gerarResumosDoArquivo("arqConcatenado.csv");

    do {
        printf("\n\n===== MENU =====");
        printf("\n1 - Pesquisar municipio");
        printf("\n0 - Encerrar programa");
        printf("\nOpcao: ");
        scanf("%d", &opcao);
 
        switch (opcao) {
            case 1:
                
                fgets(lixo, sizeof(lixo), stdin); 
                printf("Digite o nome do municipio: ");
                fgets(municipio, sizeof(municipio), stdin);
                municipio[strcspn(municipio, "\n")] = '\0'; 
                gerarCsvPorMunicipio(municipio);
                break;
            case 0:
                printf("\nEncerrando programa...\n");
                break;
            default:
                printf("\nOpcao invalida! Tente novamente.");
        }
    } while (opcao != 0);

    return 0;
}