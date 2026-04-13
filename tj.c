#include "tj.h"

/* ── Base em memória ─────────────────────────────────────────── */
Registro *base           = NULL;
int       total_registros = 0;

/* ─────────────────────────────────────────────────────────────
   Utilitário: trim in-place de '\r', '\n' e espaços nas bordas
   ───────────────────────────────────────────────────────────── */
static void trim(char *s)
{
    if (!s) return;
    /* direita */
    int len = (int)strlen(s);
    while (len > 0 && (s[len-1]=='\r'||s[len-1]=='\n'||s[len-1]==' '||s[len-1]=='"'))
        s[--len] = '\0';
    /* esquerda */
    int ini = 0;
    while (s[ini] == '"' || s[ini] == ' ') ini++;
    if (ini) memmove(s, s+ini, len-ini+1);
}

/* ─────────────────────────────────────────────────────────────
   Utilitário: strcmp case-insensitive portável
   ───────────────────────────────────────────────────────────── */
static int strieq(const char *a, const char *b)
{
    while (*a && *b) {
        char ca = (*a>='A'&&*a<='Z') ? *a+32 : *a;
        char cb = (*b>='A'&&*b<='Z') ? *b+32 : *b;
        if (ca != cb) return 0;
        a++; b++;
    }
    return *a == *b;
}

/* ─────────────────────────────────────────────────────────────
   Utilitário: verifica se 'needle' aparece em 'haystack'
   (comparação case-insensitive)
   ───────────────────────────────────────────────────────────── */
static int stricontains(const char *haystack, const char *needle)
{
    if (!needle || needle[0]=='\0') return 1;   /* vazio = wildcard */
    int hn = (int)strlen(haystack);
    int nn = (int)strlen(needle);
    for (int i = 0; i <= hn - nn; i++) {
        int ok = 1;
        for (int j = 0; j < nn && ok; j++) {
            char ch = haystack[i+j];
            char cn = needle[j];
            char chl = (ch>='A'&&ch<='Z') ? ch+32 : ch;
            char cnl = (cn>='A'&&cn<='Z') ? cn+32 : cn;
            if (chl != cnl) ok = 0;
        }
        if (ok) return 1;
    }
    return 0;
}

/* ─────────────────────────────────────────────────────────────
   Parseia um campo CSV (lida com aspas simples)
   ───────────────────────────────────────────────────────────── */
static char *nextField(char **p, char *buf, int bufsz)
{
    buf[0] = '\0';
    if (!*p) return NULL;
    int i = 0;
    char *s = *p;
    int quoted = (*s == '"');
    if (quoted) s++;
    while (*s && i < bufsz-1) {
        if (quoted) {
            if (*s == '"') { s++; if (*s == '"') buf[i++] = *s++; else break; }
            else buf[i++] = *s++;
        } else {
            if (*s == ',') break;
            buf[i++] = *s++;
        }
    }
    buf[i] = '\0';
    if (*s == ',') s++;
    *p = s;
    trim(buf);
    return buf;
}

/* ─────────────────────────────────────────────────────────────
   Carrega o CSV concatenado na memória
   ───────────────────────────────────────────────────────────── */
int carregarCSV(const char *caminho)
{
    FILE *f = fopen(caminho, "r");
    if (!f) { fprintf(stderr, "Erro ao abrir %s\n", caminho); return -1; }

    base = malloc(sizeof(Registro) * MAX_LINHAS);
    if (!base) { fclose(f); return -1; }

    char linha[4096];
    /* pula cabeçalho */
    if (!fgets(linha, sizeof(linha), f)) { fclose(f); return 0; }

    total_registros = 0;
    while (fgets(linha, sizeof(linha), f) && total_registros < MAX_LINHAS)
    {
        char *p = linha;
        char buf[MAX_STR];
        Registro *r = &base[total_registros];

        #define FIELD(dst)  nextField(&p, buf, sizeof(buf)); strncpy(dst, buf, MAX_STR-1); dst[MAX_STR-1]='\0'
        #define IFIELD(dst) nextField(&p, buf, sizeof(buf)); dst = atoi(buf)

        FIELD(r->sigla_tribunal);
        FIELD(r->procedimento);
        FIELD(r->ramo_justica);
        FIELD(r->sigla_grau);
        FIELD(r->uf_oj);
        FIELD(r->municipio_oj);
        IFIELD(r->id_ultimo_oj);
        FIELD(r->nome);
        FIELD(r->mesano_cnm1);
        FIELD(r->mesano_sent);

        IFIELD(r->casos_novos_2026);
        IFIELD(r->julgados_2026);
        IFIELD(r->prim_sent2026);
        IFIELD(r->suspensos_2026);
        IFIELD(r->dessobrestados_2026);
        IFIELD(r->cumprimento_meta1);

        IFIELD(r->distm2_a);
        IFIELD(r->julgm2_a);
        IFIELD(r->suspm2_a);
        IFIELD(r->cumprimento_meta2a);
        IFIELD(r->distm2_ant);
        IFIELD(r->julgm2_ant);
        IFIELD(r->suspm2_ant);
        IFIELD(r->desom2_ant);
        IFIELD(r->cumprimento_meta2ant);

        IFIELD(r->distm4_a);
        IFIELD(r->julgm4_a);
        IFIELD(r->suspm4_a);
        IFIELD(r->cumprimento_meta4a);
        IFIELD(r->distm4_b);
        IFIELD(r->julgm4_b);
        IFIELD(r->suspm4_b);
        IFIELD(r->cumprimento_meta4b);

        #undef FIELD
        #undef IFIELD

        total_registros++;
    }
    fclose(f);
    printf("Base carregada: %d registros.\n", total_registros);
    return total_registros;
}

void liberarBase(void)
{
    free(base);
    base = NULL;
    total_registros = 0;
}

/* ─────────────────────────────────────────────────────────────
   Pesquisa com filtros (qualquer NULL/"" = wildcard)
   ───────────────────────────────────────────────────────────── */
int pesquisar(const char *tribunal, const char *procedimento,
              const char *grau,     const char *uf,
              const char *municipio,const char *nome,
              int *resultados, int max_res)
{
    int found = 0;
    for (int i = 0; i < total_registros && found < max_res; i++)
    {
        Registro *r = &base[i];
        if (tribunal   && tribunal[0]   && !stricontains(r->sigla_tribunal, tribunal))   continue;
        if (procedimento && procedimento[0] && !stricontains(r->procedimento, procedimento)) continue;
        if (grau       && grau[0]       && !strieq(r->sigla_grau,   grau))       continue;
        if (uf         && uf[0]         && !strieq(r->uf_oj,        uf))         continue;
        if (municipio  && municipio[0]  && !stricontains(r->municipio_oj, municipio))   continue;
        if (nome       && nome[0]       && !stricontains(r->nome,    nome))       continue;
        resultados[found++] = i;
    }
    return found;
}

/* ─────────────────────────────────────────────────────────────
   Agrega cálculos de um conjunto de registros
   ───────────────────────────────────────────────────────────── */
Calculos calcularTotais(const int *indices, int n)
{
    Calculos c = {0};
    for (int i = 0; i < n; i++) {
        Registro *r = &base[indices[i]];
        c.suspm4_b          += r->suspm4_b;
        c.julgm4_b          += r->julgm4_b;
        c.distm4_b          += r->distm4_b;
        c.suspm4_a          += r->suspm4_a;
        c.julgm4_a          += r->julgm4_a;
        c.distm4_a          += r->distm4_a;
        c.suspm2_ant        += r->suspm2_ant;
        c.julgm2_ant        += r->julgm2_ant;
        c.distm2_ant        += r->distm2_ant;
        c.suspm2_a          += r->suspm2_a;
        c.julgm2_a          += r->julgm2_a;
        c.distm2_a          += r->distm2_a;
        c.dessobrestados_2026 += r->dessobrestados_2026;
        c.suspensos_2026    += r->suspensos_2026;
        c.julgados_2026     += r->julgados_2026;
        c.casos_novos_2026  += r->casos_novos_2026;
        c.prim_sent2026     += r->prim_sent2026;
        c.cumprimento_meta1     += r->cumprimento_meta1;
        c.cumprimento_meta2a    += r->cumprimento_meta2a;
        c.cumprimento_meta2ant  += r->cumprimento_meta2ant;
        c.cumprimento_meta4a    += r->cumprimento_meta4a;
        c.cumprimento_meta4b    += r->cumprimento_meta4b;
    }
    return c;
}

/* ─────────────────────────────────────────────────────────────
   Exibe um registro em detalhe
   ───────────────────────────────────────────────────────────── */
void exibirRegistro(int idx)
{
    if (idx < 0 || idx >= total_registros) return;
    Registro *r = &base[idx];
    printf("\n╔══════════════════════════════════════════════════════╗\n");
    printf("  Tribunal  : %s    Grau: %s    UF: %s\n", r->sigla_tribunal, r->sigla_grau, r->uf_oj);
    printf("  Órgão     : %s (%s)\n", r->nome, r->municipio_oj);
    printf("  Proc.     : %s\n", r->procedimento);
    printf("  Mês/Ano CNM1: %-12s  Mês/Ano Sent: %s\n", r->mesano_cnm1, r->mesano_sent);
    printf("──────────────────────────────────────────────────────\n");
    printf("  DADOS 2026\n");
    printf("    Casos Novos    : %8d    Julgados       : %8d\n", r->casos_novos_2026, r->julgados_2026);
    printf("    Prim.Sentença  : %8d    Suspensos      : %8d\n", r->prim_sent2026,    r->suspensos_2026);
    printf("    Dessobrestados : %8d\n",                          r->dessobrestados_2026);
    printf("  META 1\n");
    printf("    Cumprimento    : %8d\n", r->cumprimento_meta1);
    printf("  META 2 (ano anterior)\n");
    printf("    Distrib.       : %8d    Julgados       : %8d    Suspensos: %d\n",
           r->distm2_ant, r->julgm2_ant, r->suspm2_ant);
    printf("    Cumprimento    : %8d\n", r->cumprimento_meta2ant);
    printf("  META 2 (ano atual)\n");
    printf("    Distrib.       : %8d    Julgados       : %8d    Suspensos: %d\n",
           r->distm2_a, r->julgm2_a, r->suspm2_a);
    printf("    Cumprimento    : %8d\n", r->cumprimento_meta2a);
    printf("  META 4 (parte B)\n");
    printf("    Distrib.       : %8d    Julgados       : %8d    Suspensos: %d\n",
           r->distm4_b, r->julgm4_b, r->suspm4_b);
    printf("    Cumprimento    : %8d\n", r->cumprimento_meta4b);
    printf("  META 4 (parte A)\n");
    printf("    Distrib.       : %8d    Julgados       : %8d    Suspensos: %d\n",
           r->distm4_a, r->julgm4_a, r->suspm4_a);
    printf("    Cumprimento    : %8d\n", r->cumprimento_meta4a);
    printf("╚══════════════════════════════════════════════════════╝\n");
}

/* ─────────────────────────────────────────────────────────────
   Exibe resumo de cálculos agregados
   ───────────────────────────────────────────────────────────── */
void exibirCalculos(const Calculos *c, int n)
{
    printf("\n╔══════════════════════════════════════════════════════╗\n");
    printf("  RESUMO AGREGADO  (%d registro(s))\n", n);
    printf("──────────────────────────────────────────────────────\n");
    printf("  DADOS 2026\n");
    printf("    Casos Novos      : %10d\n", c->casos_novos_2026);
    printf("    Julgados         : %10d\n", c->julgados_2026);
    printf("    Prim. Sentença   : %10d\n", c->prim_sent2026);
    printf("    Suspensos        : %10d\n", c->suspensos_2026);
    printf("    Dessobrestados   : %10d\n", c->dessobrestados_2026);
    printf("  META 1\n");
    printf("    Cumprimento      : %10d\n", c->cumprimento_meta1);
    printf("  META 2 (ano anterior)\n");
    printf("    Distribuídos     : %10d    Julgados  : %10d    Suspensos: %d\n",
           c->distm2_ant, c->julgm2_ant, c->suspm2_ant);
    printf("    Cumprimento      : %10d\n", c->cumprimento_meta2ant);
    printf("  META 2 (ano atual)\n");
    printf("    Distribuídos     : %10d    Julgados  : %10d    Suspensos: %d\n",
           c->distm2_a, c->julgm2_a, c->suspm2_a);
    printf("    Cumprimento      : %10d\n", c->cumprimento_meta2a);
    printf("  META 4 (parte B)\n");
    printf("    Distribuídos     : %10d    Julgados  : %10d    Suspensos: %d\n",
           c->distm4_b, c->julgm4_b, c->suspm4_b);
    printf("    Cumprimento      : %10d\n", c->cumprimento_meta4b);
    printf("  META 4 (parte A)\n");
    printf("    Distribuídos     : %10d    Julgados  : %10d    Suspensos: %d\n",
           c->distm4_a, c->julgm4_a, c->suspm4_a);
    printf("    Cumprimento      : %10d\n", c->cumprimento_meta4a);
    printf("╚══════════════════════════════════════════════════════╝\n");
}

/* ─────────────────────────────────────────────────────────────
   Lê uma linha do stdin descartando '\n'
   ───────────────────────────────────────────────────────────── */
static void lerLinha(char *buf, int sz)
{
    buf[0] = '\0';
    if (fgets(buf, sz, stdin)) {
        int len = (int)strlen(buf);
        if (len > 0 && buf[len-1] == '\n') buf[len-1] = '\0';
    }
}

/* ─────────────────────────────────────────────────────────────
   Menu interativo principal
   ───────────────────────────────────────────────────────────── */
void menuPrincipal(void)
{
    int *resultados = malloc(sizeof(int) * MAX_RESULTADOS);
    if (!resultados) { fputs("Memória insuficiente.\n", stderr); return; }

    char tribunal[MAX_STR], procedimento[MAX_STR], grau[MAX_STR];
    char uf[MAX_STR], municipio[MAX_STR], nome[MAX_STR];
    char buf[32];
    int  opcao, n;

    for (;;) {
        printf("\n╔══════════════════════════════════════════════════════╗\n");
        printf("║         SISTEMA DE PESQUISA - JUSTIÇA ELEITORAL     ║\n");
        printf("╠══════════════════════════════════════════════════════╣\n");
        printf("║  1. Pesquisar registros                              ║\n");
        printf("║  2. Ver lista de tribunais disponíveis               ║\n");
        printf("║  0. Sair                                             ║\n");
        printf("╚══════════════════════════════════════════════════════╝\n");
        printf("Opção: ");
        lerLinha(buf, sizeof(buf));
        opcao = atoi(buf);

        if (opcao == 0) break;

        if (opcao == 2) {
            printf("\nTribunais disponíveis:\n");
            printf("  TRE-AC  TRE-AL  TRE-AM  TRE-AP  TRE-BA  TRE-CE  TRE-DF\n");
            printf("  TRE-ES  TRE-GO  TRE-MA  TRE-MG  TRE-MS  TRE-MT  TRE-PA\n");
            printf("  TRE-PB  TRE-PE  TRE-PI  TRE-PR  TRE-RJ  TRE-RN  TRE-RO\n");
            printf("  TRE-RR  TRE-RS  TRE-SC  TRE-SE  TRE-SP  TRE-TO\n");
            printf("\nGraus: G1 (1º Grau)  |  G2 (2º Grau)\n");
            printf("Procedimentos: Conhecimento criminal | Conhecimento não criminal\n");
            continue;
        }

        if (opcao != 1) { printf("Opção inválida.\n"); continue; }

        /* ── Coleta de filtros ── */
        printf("\n--- FILTROS (deixe em branco para ignorar) ---\n");

        printf("Tribunal (ex: TRE-SP): ");
        lerLinha(tribunal, sizeof(tribunal));

        printf("Grau (G1/G2): ");
        lerLinha(grau, sizeof(grau));

        printf("UF (ex: SP): ");
        lerLinha(uf, sizeof(uf));

        printf("Município (busca parcial): ");
        lerLinha(municipio, sizeof(municipio));

        printf("Nome do órgão (busca parcial): ");
        lerLinha(nome, sizeof(nome));

        printf("Procedimento (criminal/nao criminal): ");
        lerLinha(procedimento, sizeof(procedimento));

        /* ── Pesquisa ── */
        n = pesquisar(tribunal, procedimento, grau, uf, municipio, nome,
                      resultados, MAX_RESULTADOS);

        printf("\nEncontrados: %d registro(s).\n", n);
        if (n == 0) continue;

        /* ── O que fazer com os resultados ── */
        printf("\n--- O QUE DESEJA VER? ---\n");
        printf("  1. Resumo agregado (totais/calculos)\n");
        printf("  2. Listar registros (ate 50)\n");
        printf("  3. Ver registro individual (por numero da lista)\n");
        printf("  0. Voltar\n");
        printf("Opção: ");
        lerLinha(buf, sizeof(buf));
        opcao = atoi(buf);

        if (opcao == 0) continue;

        if (opcao == 1) {
            Calculos c = calcularTotais(resultados, n);
            exibirCalculos(&c, n);
            continue;
        }

        if (opcao == 2) {
            int lim = n < 50 ? n : 50;
            printf("\n%-5s %-10s %-4s %-4s %-20s %-30s %-12s\n",
                   "#", "Tribunal", "Grau", "UF", "Município", "Órgão", "Mês/Ano");
            printf("%-5s %-10s %-4s %-4s %-20s %-30s %-12s\n",
                   "─────","──────────","────","────","────────────────────","──────────────────────────────","────────────");
            for (int i = 0; i < lim; i++) {
                Registro *r = &base[resultados[i]];
                printf("%-5d %-10s %-4s %-4s %-20.20s %-30.30s %-12s\n",
                       i+1, r->sigla_tribunal, r->sigla_grau, r->uf_oj,
                       r->municipio_oj, r->nome, r->mesano_cnm1);
            }
            if (n > 50) printf("  ... (%d registros omitidos)\n", n-50);
            continue;
        }

        if (opcao == 3) {
            int lim = n < 50 ? n : 50;
            /* lista compacta antes de pedir número */
            printf("\n%-5s %-10s %-4s %-4s %-20s %-30s\n",
                   "#","Tribunal","Grau","UF","Município","Órgão");
            for (int i = 0; i < lim; i++) {
                Registro *r = &base[resultados[i]];
                printf("%-5d %-10s %-4s %-4s %-20.20s %-30.30s\n",
                       i+1, r->sigla_tribunal, r->sigla_grau, r->uf_oj,
                       r->municipio_oj, r->nome);
            }
            printf("Número do registro (1-%d): ", lim);
            lerLinha(buf, sizeof(buf));
            int num = atoi(buf);
            if (num < 1 || num > lim) { printf("Número inválido.\n"); continue; }
            exibirRegistro(resultados[num-1]);
        }
    }
    free(resultados);
}
