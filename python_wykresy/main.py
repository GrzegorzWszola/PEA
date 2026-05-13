import matplotlib.pyplot as plt
import numpy as np

# Dane
instancje = ['ftv33\n(N=34)', 'ft53\n(N=53)', 'ftv70\n(N=71)',
             'kro124\n(N=124)', 'ftv170\n(N=171)', 'rbg323\n(N=323)', 'rbg443\n(N=443)']

greedy = [5.83, 24.32, 17.28, 19.56, 30.02, 28.36, 41.84]
random = [27.29, 48.27, 75.13, 39.46, 198.62, 166.59, 89.41]

x = np.arange(len(instancje))
width = 0.35

fig, ax = plt.subplots(figsize=(12, 6))

bars1 = ax.bar(x - width/2, greedy, width, label='Start zachłanny (RNN)',
               color='steelblue', edgecolor='black', linewidth=0.5)
bars2 = ax.bar(x + width/2, random, width, label='Start losowy',
               color='tomato', edgecolor='black', linewidth=0.5)

# Etykiety wartosci nad slupkami
for bar in bars1:
    h = bar.get_height()
    ax.annotate(f'{h:.1f}%',
                xy=(bar.get_x() + bar.get_width() / 2, h),
                xytext=(0, 3), textcoords='offset points',
                ha='center', va='bottom', fontsize=8)

for bar in bars2:
    h = bar.get_height()
    ax.annotate(f'{h:.1f}%',
                xy=(bar.get_x() + bar.get_width() / 2, h),
                xytext=(0, 3), textcoords='offset points',
                ha='center', va='bottom', fontsize=8)

ax.set_xlabel('Instancja', fontsize=12)
ax.set_ylabel('Błąd względem optimum [%]', fontsize=12)
ax.set_title('SA — wpływ metody wyboru rozwiązania początkowego na błąd', fontsize=13)
ax.set_xticks(x)
ax.set_xticklabels(instancje, fontsize=9)
ax.legend(fontsize=11)
ax.grid(axis='y', linestyle='--', alpha=0.5)
ax.set_ylim(0, 230)

plt.tight_layout()
plt.savefig('plot_swap_vs_random.png', dpi=150, bbox_inches='tight')
plt.show()
print("Zapisano: plot_swap_vs_random.png")