import matplotlib.pyplot as plt
import matplotlib.ticker as ticker

# --- DANE ---

# Brute-force
bf_data = {
    'N': [8, 9, 10, 11, 12, 13, 14],
    'czas': [0.0155, 0.1236, 1.1609, 11.7924, 136.1298, 1728.6625, 25684.8162]
}

# Best First (Symetryczny, Bez UB)
bnb_bf_no_ub = {
    'N': [7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20],
    'czas': [0.1539, 0.3648, 0.8638, 2.9373, 6.2267, 15.4393, 33.1541, 105.1918, 227.6244, 692.2128, 1398.7367, 2173.3614, 6666.5947, 22510.8064]
}

# BFS (Symetryczny, Bez UB)
bnb_bfs_no_ub = {
    'N': [7, 8, 9, 10, 11],
    'czas': [0.6359, 6.0615, 57.2060, 843.2337, 9624.1729]
}

# DFS (Symetryczny, Bez UB)
bnb_dfs_no_ub = {
    'N': [8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20],
    'czas': [0.0764, 0.1518, 0.4682, 1.0551, 2.4137, 4.7429, 11.5034, 22.0974, 82.8216, 78.6929, 420.6217, 547.5565, 1371.8881]
}

plt.figure(figsize=(12, 7))

# Wykresy
plt.plot(bf_data['N'], bf_data['czas'], marker='X', color='black', label='Brute-force', linewidth=3, markersize=10)
plt.plot(bnb_bfs_no_ub['N'], bnb_bfs_no_ub['czas'], marker='o', color='#993C1D', label='BFS (Bez UB)', linewidth=2)
plt.plot(bnb_bf_no_ub['N'], bnb_bf_no_ub['czas'], marker='s', color='#185FA5', label='Best First (Bez UB)', linewidth=2)
plt.plot(bnb_dfs_no_ub['N'], bnb_dfs_no_ub['czas'], marker='^', color='#0F6E56', label='DFS (Bez UB)', linewidth=2)

plt.yscale('log')
plt.title('Porównanie: Brute-force vs Strategie BnB (Bez UB)', fontsize=14, fontweight='bold')
plt.xlabel('N (rozmiar instancji)', fontsize=12)
plt.ylabel('Średni czas [ms] (skala log)', fontsize=12)

plt.gca().yaxis.set_major_formatter(ticker.FuncFormatter(lambda x, _: f'{x:g}'))
plt.grid(True, which='both', linestyle='--', alpha=0.5)
plt.legend()
plt.xticks(range(7, 21))

plt.tight_layout()
plt.savefig('bnb_wszystkie_vs_bruteforce.png', dpi=150)
plt.show()