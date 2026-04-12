#ifndef tj_h
#define tj_h

//Struct para variaveis dos calculos.
typedef struct {
    int suspm4_b = 0;
    int julgm4_b = 0;
    int distm4_b = 0;
    int suspm4_a = 0;
    int julgm4_a = 0;
    int distm4_a = 0;
    int suspm2_ant = 0;
    int julgm2_ant = 0;
    int distm2_ant = 0;
    int suspm2_a = 0;
    int julgm2_a = 0;
    int distm2_a = 0;
    int dessobrestados_2026 = 0;
    int suspensos_2026 = 0;
    int julgados_2026 = 0;
    int casos_novos_2026 = 0;

} Calculos;

// Realiza a leitura dos arquivos e a concatenação.
void concatenarArqvs();
#endif