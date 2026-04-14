#ifndef tj_h
#define tj_h

//Struct para variaveis dos calculos.
typedef struct {
    char sigla_tribunal[20]; // Adicionado para armazenar a sigla (ex: "TRE-AC")
    int suspm4_b;
    int julgm4_b;
    int distm4_b;
    int suspm4_a;
    int julgm4_a;
    int distm4_a;
    int suspm2_ant;
    int julgm2_ant;
    int distm2_ant;
    int desom2_ant;
    int suspm2_a;
    int julgm2_a;
    int distm2_a;
    int dessobrestados_2026;
    int suspensos_2026;
    int julgados_2026;
    int casos_novos_2026;
} Calculos;





// Realiza a leitura dos arquivos e a concatenação.
void concatenarArqvs();

// Funcao para gerar CSVs distinto com base no municiopio
void gerarCsvPorMunicipio(const char *nomeBusca);
int pegarCampo(const char *linha, int alvo, char *destino, int tamanho);

// Funcao para gerar os calculos e resumos do tribunal de cada estado
void gerarResumosDoArquivo(const char *nomeArquivoConcatenado);
#endif