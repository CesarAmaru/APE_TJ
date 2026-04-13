#include <stdio.h>
#include <stdlib.h>
#include "tj.h"

int main(void)
{
    printf("Carregando base de dados...\n");

    if (carregarCSV(ARQUIVO_CSV) <= 0) {
        fprintf(stderr,
            "Erro: nao foi possivel carregar '%s'.\n"
            "Certifique-se de que o arquivo existe em src/arqConcatenado.csv\n",
            ARQUIVO_CSV);
        return 1;
    }

    menuPrincipal();

    liberarBase();
    printf("\nPrograma encerrado.\n");
    return 0;
}
