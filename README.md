# NAP1 — Projeto e Análise de Algoritmos (UFRA/BSI)

Comparação experimental de três pares de algoritmos:

| Sub-atividade | Algoritmo A | Algoritmo B |
|---|---|---|
| 1 — Primalidade | Força bruta | Teste de Fermat (k=10) |
| 2 — Busca | Busca linear | Tabela hash (encadeamento) |
| 3 — Grafos | BFS | DFS |

**Disciplina:** Projet

**Checkpoint:** `ls` deve mostrar `src data results figures scripts docs bin README.md Makefile` *(Makefile ainda não existe — ok, criamos no Bloco 5)*.

---

## 🔹 Bloco 2 — `src/primos.c`

```bash
cat > src/primos.c << 'EOF'
/* primos.c — Sub-atividade 1
 * Força bruta x Fermat.
 * Compilar: gcc -O2 -Wall -std=c11 -o primos primos.c
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

static double agora(void) {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec + ts.tv_nsec * 1e-9;
}

static bool forca_bruta(unsigned long long n) {
    if (n < 2) return false;
    for (unsigned long long d = 2; d < n; d++)
        if (n % d == 0) return false;
    return true;
}

static unsigned long long mulmod(unsigned long long a, unsigned long long b,
                                 unsigned long long m) {
    return (unsigned long long)((unsigned __int128)a * b % m);
}

static unsigned long long powmod(unsigned long long a, unsigned long long e,
                                 unsigned long long m) {
    unsigned long long r = 1 % m;
    a %= m;
    while (e) {
        if (e & 1ULL) r = mulmod(r, a, m);
        a = mulmod(a, a, m);
        e >>= 1;
    }
    return r;
}

static bool fermat(unsigned long long n, int k) {
    if (n < 2) return false;
    if (n < 4) return true;
    if ((n & 1ULL) == 0) return false;
    for (int i = 0; i < k; i++) {
        unsigned long long a = 2 + (unsigned long long)rand() % (n - 3);
        if (powmod(a, n - 1, n) != 1) return false;
    }
    return true;
}

static void testar(unsigned long long n, int k, int cabecalho) {
    if (cabecalho)
        printf("N;ForcaBruta;t_FB(s);Fermat;t_Fermat(s);aceleracao\n");
    double t0 = agora();
    bool p1 = forca_bruta(n);
    double t1 = agora();
    bool p2 = fermat(n, k);
    double t2 = agora();
    double d1 = t1 - t0, d2 = t2 - t1;
    printf("%llu;%s;%.9f;%s;%.9f;%.1f\n",
           n,
           p1 ? "primo" : "composto", d1,
           p2 ? "provavelmente_primo" : "composto", d2,
           d1 / (d2 + 1e-12));
}

int main(int argc, char **argv) {
    srand((unsigned)time(NULL));
    int k = 10;
    if (argc >= 2 && strcmp(argv[1], "--bench") == 0) {
        unsigned long long valores[] = {
            7ULL, 101ULL, 1009ULL, 10007ULL, 100003ULL,
            1000003ULL, 10000019ULL, 100000007ULL, 1000000007ULL
        };
        size_t q = sizeof(valores) / sizeof(valores[0]);
        for (size_t i = 0; i < q; i++)
            testar(valores[i], k, i == 0);
        return 0;
    }
    unsigned long long n;
    printf("Digite N: ");
    if (scanf("%llu", &n) != 1) return 1;
    printf("Digite k (iteracoes de Fermat, ex.: 10): ");
    if (scanf("%d", &k) != 1) k = 10;
    printf("\nN;ForcaBruta;t_FB(s);Fermat;t_Fermat(s);aceleracao\n");
    testar(n, k, 0);
    return 0;
}
