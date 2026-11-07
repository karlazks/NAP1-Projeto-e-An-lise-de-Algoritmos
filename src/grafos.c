/* grafos.c — Sub-atividade 3
 * BFS x DFS na busca de um vértice alvo.
 * Compilar: gcc -O2 -Wall -std=c11 -o grafos grafos.c
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

typedef struct { int n, cap, m; int *head, *to, *nxt; } Grafo;

static void grafo_alocar(Grafo *g, int n, int cap) {
    g->n = n; g->m = 0; g->cap = cap;
    g->head = malloc(sizeof(int) * (size_t)n);
    g->to   = malloc(sizeof(int) * (size_t)cap);
    g->nxt  = malloc(sizeof(int) * (size_t)cap);
    for (int i = 0; i < n; i++) g->head[i] = -1;
}
static void aresta(Grafo *g, int u, int v) {
    g->to [g->m] = v;
    g->nxt[g->m] = g->head[u];
    g->head[u]   = g->m++;
}
static void grafo_liberar(Grafo *g) {
    free(g->head); free(g->to); free(g->nxt);
}

static int bfs(const Grafo *g, int origem, int alvo, long *visitados) {
    char *vis = calloc((size_t)g->n, 1);
    int  *fila = malloc(sizeof(int) * (size_t)g->n);
    int ini = 0, fim = 0;
    fila[fim++] = origem; vis[origem] = 1;
    *visitados = 0;
    while (ini < fim) {
        int u = fila[ini++];
        (*visitados)++;
        if (u == alvo) { free(vis); free(fila); return 1; }
        for (int e = g->head[u]; e != -1; e = g->nxt[e]) {
            int v = g->to[e];
            if (!vis[v]) { vis[v] = 1; fila[fim++] = v; }
        }
    }
    free(vis); free(fila);
    return 0;
}

static int dfs(const Grafo *g, int origem, int alvo, long *visitados) {
    char *vis   = calloc((size_t)g->n, 1);
    int  *pilha = malloc(sizeof(int) * ((size_t)g->m + 1));
    int topo = 0;
    pilha[topo++] = origem;
    *visitados = 0;
    while (topo > 0) {
        int u = pilha[--topo];
        if (vis[u]) continue;
        vis[u] = 1;
        (*visitados)++;
        if (u == alvo) { free(vis); free(pilha); return 1; }
        for (int e = g->head[u]; e != -1; e = g->nxt[e]) {
            int v = g->to[e];
            if (!vis[v]) pilha[topo++] = v;
        }
    }
    free(vis); free(pilha);
    return 0;
}

static void ler_arquivo(Grafo *g, const char *nome) {
    FILE *f = fopen(nome, "r");
    if (!f) { perror("fopen"); exit(1); }
    int n, m;
    if (fscanf(f, "%d %d", &n, &m) != 2) exit(1);
    grafo_alocar(g, n, 2 * m);
    for (int i = 0; i < m; i++) {
        int u, v;
        if (fscanf(f, "%d %d", &u, &v) != 2) break;
        aresta(g, u, v); aresta(g, v, u);
    }
    fclose(f);
}

static void gerar_aleatorio(Grafo *g, int n, int m, unsigned seed) {
    if (m < n - 1) m = n - 1;
    grafo_alocar(g, n, 2 * m);
    srand(seed);
    for (int v = 1; v < n; v++) {
        int u = rand() % v;
        aresta(g, u, v); aresta(g, v, u);
    }
    for (int i = n - 1; i < m; i++) {
        int u = rand() % n, v = rand() % n;
        aresta(g, u, v); aresta(g, v, u);
    }
}

int main(int argc, char **argv) {
    Grafo g;
    int origem = 0, alvo;

    if (argc >= 2 && strcmp(argv[1], "--media") == 0) {
        int K = (argc >= 3) ? atoi(argv[2]) : 100;
        int n = (argc >= 4) ? atoi(argv[3]) : 100000;
        int m = (argc >= 5) ? atoi(argv[4]) : 500000;
        unsigned seed = (argc >= 6) ? (unsigned)atoi(argv[5]) : 42;
        gerar_aleatorio(&g, n, m, seed);
        srand(seed + 1);
        double somaB = 0, somaD = 0;
        long visB = 0, visD = 0;
        int okB = 0, okD = 0;
        for (int i = 0; i < K; i++) {
            int alvo_i = 1 + rand() % (n - 1);
            long vB = 0, vD = 0;
            double t0 = agora();
            okB += bfs(&g, origem, alvo_i, &vB);
            double t1 = agora();
            okD += dfs(&g, origem, alvo_i, &vD);
            double t2 = agora();
            somaB += t1 - t0; somaD += t2 - t1;
            visB += vB; visD += vD;
        }
        printf("K;n;m;metodo;t_medio_s;visitados_medio;achou\n");
        printf("%d;%d;%d;BFS;%.6f;%.1f;%d\n", K, n, m, somaB/K, (double)visB/K, okB);
        printf("%d;%d;%d;DFS;%.6f;%.1f;%d\n", K, n, m, somaD/K, (double)visD/K, okD);
        grafo_liberar(&g);
        return 0;
    }

    if (argc >= 2 && strcmp(argv[1], "--random") == 0) {
        int n = (argc >= 3) ? atoi(argv[2]) : 100000;
        int m = (argc >= 4) ? atoi(argv[3]) : 500000;
        unsigned seed = (argc >= 5) ? (unsigned)atoi(argv[4]) : 42;
        alvo = (argc >= 6) ? atoi(argv[5]) : n - 1;
        gerar_aleatorio(&g, n, m, seed);
    } else if (argc >= 2) {
        ler_arquivo(&g, argv[1]);
        alvo = (argc >= 3) ? atoi(argv[2]) : g.n - 1;
    } else {
        gerar_aleatorio(&g, 100000, 500000, 42);
        alvo = g.n - 1;
    }

    long vB = 0, vD = 0;
    double t0 = agora();
    int okB = bfs(&g, origem, alvo, &vB);
    double t1 = agora();
    int okD = dfs(&g, origem, alvo, &vD);
    double t2 = agora();
    printf("Grafo: n=%d, m=%d (nao-dirigido), origem=%d, alvo=%d\n",
           g.n, g.m / 2, origem, alvo);
    printf("Metodo;encontrado;tempo_s;visitados\n");
    printf("BFS;%d;%.6f;%ld\n", okB, t1 - t0, vB);
    printf("DFS;%d;%.6f;%ld\n", okD, t2 - t1, vD);
    grafo_liberar(&g);
    return 0;
}
