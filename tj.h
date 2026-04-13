#ifndef TJ_H
#define TJ_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINHAS     210000
#define MAX_STR        128
#define MAX_RESULTADOS 10000
#define ARQUIVO_CSV    "src/arqConcatenado.csv"

/* ── Registro de um processo ─────────────────────────────────── */
typedef struct {
    char  sigla_tribunal[MAX_STR];
    char  procedimento[MAX_STR];
    char  ramo_justica[MAX_STR];
    char  sigla_grau[MAX_STR];
    char  uf_oj[MAX_STR];
    char  municipio_oj[MAX_STR];
    int   id_ultimo_oj;
    char  nome[MAX_STR];
    char  mesano_cnm1[MAX_STR];
    char  mesano_sent[MAX_STR];

    int casos_novos_2026;
    int julgados_2026;
    int prim_sent2026;
    int suspensos_2026;
    int dessobrestados_2026;
    int cumprimento_meta1;

    int distm2_a;
    int julgm2_a;
    int suspm2_a;
    int cumprimento_meta2a;
    int distm2_ant;
    int julgm2_ant;
    int suspm2_ant;
    int desom2_ant;
    int cumprimento_meta2ant;

    int distm4_a;
    int julgm4_a;
    int suspm4_a;
    int cumprimento_meta4a;
    int distm4_b;
    int julgm4_b;
    int suspm4_b;
    int cumprimento_meta4b;
} Registro;

/* ── Calculos agrupados (soma de um conjunto de registros) ───── */
typedef struct {
    int suspm4_b;
    int julgm4_b;
    int distm4_b;
    int suspm4_a;
    int julgm4_a;
    int distm4_a;
    int suspm2_ant;
    int julgm2_ant;
    int distm2_ant;
    int suspm2_a;
    int julgm2_a;
    int distm2_a;
    int dessobrestados_2026;
    int suspensos_2026;
    int julgados_2026;
    int casos_novos_2026;
    int prim_sent2026;
    int cumprimento_meta1;
    int cumprimento_meta2a;
    int cumprimento_meta2ant;
    int cumprimento_meta4a;
    int cumprimento_meta4b;
} Calculos;

/* ── Base de dados em memória ────────────────────────────────── */
extern Registro *base;
extern int       total_registros;

/* ── Protótipos ──────────────────────────────────────────────── */
int  carregarCSV(const char *caminho);
void liberarBase(void);

/* Pesquisa: retorna índices dos registros que batem com o filtro.
   Qualquer campo NULL ou "" é ignorado (wildcard).             */
int pesquisar(
    const char *tribunal,
    const char *procedimento,
    const char *grau,
    const char *uf,
    const char *municipio,
    const char *nome,
    int *resultados,        /* array de índices (saída)          */
    int  max_res            /* tamanho máximo do array           */
);

/* Agrega os cálculos de um conjunto de registros */
Calculos calcularTotais(const int *indices, int n);

/* Exibe um registro em detalhe */
void exibirRegistro(int idx);

/* Exibe o resumo de cálculos */
void exibirCalculos(const Calculos *c, int n_registros);

/* Menu interativo principal */
void menuPrincipal(void);

#endif /* TJ_H */
