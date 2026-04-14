import matplotlib.pyplot as plt
import matplotlib.ticker as ticker

# --- NOWE DANE (TSP SYMERYCZNY) ---

# Pierwsza tabela: Best First
best_first = {
    'N':        [7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20],
    'czas':     [0.1539, 0.3648, 0.8638, 2.9373, 6.2267, 15.4393, 33.1541, 105.1918, 227.6244, 692.2128, 1398.7367, 2173.3614, 6666.5947, 22510.8064],
    'wierzch':  [116, 258, 467, 2070, 3984, 6956, 10087, 59262, 69305, 295261, 327315, 552229, 1684841, 4083856],
}

# Druga tabela: BFS
bfs = {
    'N':        [7, 8, 9, 10, 11],
    'czas':     [0.6359, 6.0615, 57.2060, 843.2337, 9624.1729],
    'wierzch':  [720, 5040, 40320, 362880, 3628800],
}

# Trzecia tabela: DFS
dfs = {
    'N':        [8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20],
    'czas':     [0.0764, 0.1518, 0.4682, 1.0551, 2.4137, 4.7429, 11.5034, 22.0974, 82.8216, 78.6929, 420.6217, 547.5565, 1371.8881],
    'wierzch':  [22, 29, 37, 46, 56, 67, 79, 92, 106, 121, 137, 154, 172],
}

COLORS = {
    'Best First': '#185FA5',
    'BFS':        '#993C1D',
    'DFS':        '#0F6E56',
}

fig, axes = plt.subplots(1, 2, figsize=(14, 6))
fig.suptitle('BnB – porównanie strategii (TSP Symetryczny, bez upper bound)', fontsize=14, fontweight='bold')

# --- Wykres 1: Czas ---
ax1 = axes[0]
ax1.plot(best_first['N'], best_first['czas'], marker='s', color=COLORS['Best First'], label='Best First', linewidth=2)
ax1.plot(bfs['N'],        bfs['czas'],        marker='o', color=COLORS['BFS'],        label='BFS',        linewidth=2, linestyle='--')
ax1.plot(dfs['N'],        dfs['czas'],        marker='^', color=COLORS['DFS'],        label='DFS',        linewidth=2, linestyle=':')

ax1.set_xlabel('N (rozmiar instancji)', fontsize=12)
ax1.set_ylabel('Średni czas [ms]', fontsize=12)
ax1.set_title('Czas wykonania', fontsize=12)
ax1.set_yscale('log')
ax1.yaxis.set_major_formatter(ticker.FuncFormatter(lambda x, _: f'{x:g}'))
ax1.legend(fontsize=11)
ax1.grid(True, which='both', linestyle='--', alpha=0.4)
ax1.set_xticks(range(7, 21))

# --- Wykres 2: Odwiedzone wierzchołki ---
ax2 = axes[1]
ax2.plot(best_first['N'], best_first['wierzch'], marker='s', color=COLORS['Best First'], label='Best First', linewidth=2)
ax2.plot(bfs['N'],        bfs['wierzch'],        marker='o', color=COLORS['BFS'],        label='BFS',        linewidth=2, linestyle='--')
ax2.plot(dfs['N'],        dfs['wierzch'],        marker='^', color=COLORS['DFS'],        label='DFS',        linewidth=2, linestyle=':')

ax2.set_xlabel('N (rozmiar instancji)', fontsize=12)
ax2.set_ylabel('Odwiedzone wierzchołki', fontsize=12)
ax2.set_title('Odwiedzone wierzchołki', fontsize=12)
ax2.set_yscale('log')
ax2.yaxis.set_major_formatter(ticker.FuncFormatter(lambda x, _: f'{x:,.0f}'))
ax2.legend(fontsize=11)
ax2.grid(True, which='both', linestyle='--', alpha=0.4)
ax2.set_xticks(range(7, 21))

plt.tight_layout()
plt.savefig('bnb_tsp_symetryczny.png', dpi=150, bbox_inches='tight')
plt.show()