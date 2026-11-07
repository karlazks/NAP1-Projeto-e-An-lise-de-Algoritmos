/* hash.c — Sub-atividade 2
 * Busca Linear x Tabela Hash (encadeamento).
 * Compilar: gcc -O2 -Wall -std=c11 -o hash hash.c
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

static double agora(void) {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec + ts.tv_nsec * 1e-9;
}

typedef struct No { int chave; struct No *prox; } No;
typedef struct { No **balde; int tam; } Hash;

static int eh_primo(int x) {
    if (x < 2) return 0;
    for (int d = 2; (long long)d * d <= x; d++)
        if (x % d == 0) return 0;
    return 1;
}
static int proximo_primo(int x) { while (!eh_primo(x)) x++; return x; }
static int hash_fn(int chave, int tam) {
    return (int)(((unsigned)chave * 2654435761u) % (unsigned)tam);
}
static Hash *hash_criar(int tam) {
    Hash *h = malloc(sizeof(Hash));
    h->tam   = tam;
    h->balde = calloc((size_t)tam, sizeof(No *));
    return h;
}
static void hash_inserir(Hash *h, int chave) {
    int i = hash_fn(chave, h->tam);
    No *novo = malloc(sizeof(No));
    novo->chave = chave;
    novo->prox  = h->balde[i];
    h->balde[i] = novo;
}
static int hash_buscar(Hash *h, int chave) {
    for (No *p = h->balde[hash_fn(chave, h->tam)]; p; p = p->prox)
        if (p->chave == chave) return 1;
    return 0;
}
static void hash_liberar(Hash *h) {
    for (int i = 0; i < h->tam; i++) {
        No *p = h->balde[i];
        while (p) { No *q = p->prox; free(p); p = q; }
    }
    free(h->balde); free(h);
}
static int busca_linear(const int *v, int m, int chave) {
    for (int i = 0; i < m; i++)
        if (v[i] == chave) return 1;
    return 0;
}

int main(int argc, char **argv) {
    srand(2026);
    if (argc >= 2 && strcmp(argv[1], "--bench") == 0) {
        int Ms[] = {1000, 10000, 100000, 1000000};
        printf("M;R;chave;t_linear_s;t_hash_s;aceleracao\n");
        for (size_t i = 0; i < sizeof(Ms)/sizeof(Ms[0]); i++) {
            int M = Ms[i];
            int R = (int)(100000000 / M);
            if (R < 10) R = 10;
            if (R > 10000) R = 10000;
            int *v = malloc(sizeof(int) * (size_t)M);
            for (int j = 0; j < M; j++) v[j] = rand() % (10 * M + 1);
            Hash *h = hash_criar(proximo_primo(2 * M));
            for (int j = 0; j < M; j++) hash_inserir(h, v[j]);
            int chave = -1;
            volatile long s1 = 0, s2 = 0;
            double t0 = agora();
            for (int r = 0; r < R; r++) s1 += busca_linear(v, M, chave);
            double t1 = agora();
            for (int r = 0; r < R; r++) s2 += hash_buscar(h, chave);
            double t2 = agora();
            printf("%d;%d;%d;%.9f;%.9f;%.1f\n",
                   M, R, chave, (t1-t0)/R, (t2-t1)/R,
                   (t1-t0)/(t2-t1+1e-15));
            hash_liberar(h); free(v);
        }
        return 0;
    }
    int M = 10000, R = 1000;
    if (argc >= 2) M = atoi(argv[1]);
    if (argc >= 3) R = atoi(argv[2]);
    int *v = malloc(sizeof(int) * (size_t)M);
    for (int i = 0; i < M; i++) v[i] = rand() % (10 * M + 1);
    int tam = proximo_primo(2 * M);
    Hash *h = hash_criar(tam);
    for (int i = 0; i < M; i++) hash_inserir(h, v[i]);
    printf("Vetor de M=%d inteiros. Tabela hash = %d baldes.\n", M, tam);
    printf("Exemplo de chave presente: v[0] = %d\n", v[0]);
    int chave;
    printf("Chave de busca: ");
    if (scanf("%d", &chave) != 1) chave = -1;
    volatile long s1 = 0, s2 = 0;
    double t0 = agora();
    for (int r = 0; r < R; r++) s1 += busca_linear(v, M, chave);
    double t1 = agora();
    for (int r = 0; r < R; r++) s2 += hash_buscar(h, chave);
    double t2 = agora();
    printf("chave=%d presente=%s R=%d\n", chave, s1?"sim":"nao", R);
    printf("  Linear : %.9f s/busca  (%.3f us)\n", (t1-t0)/R, (t1-t0)/R*1e6);
    printf("  Hash   : %.9f s/busca  (%.3f us)\n", (t2-t1)/R, (t2-t1)/R*1e6);
    printf("  Aceleracao: %.1fx\n", (t1-t0)/(t2-t1+1e-15));
    hash_liberar(h); free(v);
    return 0;
}
