import csv, os
import matplotlib.pyplot as plt

BASE = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
RES  = os.path.join(BASE, 'results')
FIG  = os.path.join(BASE, 'figures')
os.makedirs(FIG, exist_ok=True)

def ler(nome, sep=';'):
    with open(os.path.join(RES, nome)) as f:
        return list(csv.DictReader(f, delimiter=sep))

d  = ler('primos.csv')
N  = [int(r['N']) for r in d]
fb = [float(r['t_FB(s)']) for r in d]
fe = [float(r['t_Fermat(s)']) for r in d]
plt.figure()
plt.loglog(N, fb, 'o-', label='Força bruta')
plt.loglog(N, fe, 's-', label='Fermat (k=10)')
plt.xlabel('N'); plt.ylabel('Tempo (s)')
plt.title('Sub-atividade 1 — Primalidade')
plt.grid(True, which='both'); plt.legend()
plt.savefig(os.path.join(FIG, 'fig_primos.png'), dpi=150, bbox_inches='tight')
plt.close()

d  = ler('hash.csv')
M  = [int(r['M']) for r in d]
tl = [float(r['t_linear_s']) for r in d]
th = [float(r['t_hash_s'])   for r in d]
plt.figure()
plt.loglog(M, tl, 'o-', label='Busca linear')
plt.loglog(M, th, 's-', label='Tabela hash')
plt.xlabel('M'); plt.ylabel('Tempo por busca (s)')
plt.title('Sub-atividade 2 — Busca')
plt.grid(True, which='both'); plt.legend()
plt.savefig(os.path.join(FIG, 'fig_hash.png'), dpi=150, bbox_inches='tight')
plt.close()

d   = ler('grafos.csv')
met = [r['metodo'] for r in d]
vis = [float(r['visitados_medio']) for r in d]
tmp = [float(r['t_medio_s']) * 1000 for r in d]
fig, ax = plt.subplots(1, 2, figsize=(9, 3.5))
ax[0].bar(met, vis); ax[0].set_ylabel('Vértices visitados (média)')
ax[0].set_title('Exploração até achar o alvo')
ax[1].bar(met, tmp); ax[1].set_ylabel('Tempo médio (ms)')
ax[1].set_title('Tempo médio de busca')
plt.tight_layout()
plt.savefig(os.path.join(FIG, 'fig_grafos.png'), dpi=150, bbox_inches='tight')
plt.close()
print('Figuras geradas em', FIG)
