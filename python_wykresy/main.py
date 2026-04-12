import matplotlib.pyplot as plt
import matplotlib.ticker as ticker
import numpy as np

# --- Dane wydajnościowe dla dużych N ---

# Brute Force - tylko małe N, nie uwzględniamy (zbyt wolny)

# NN - dane z pomiarów
N_NN = np.array([5000, 10000, 15000, 20000, 25000, 30000, 35000])
czas_NN = np.array([12.21, 54.02, 139.77, 171.59, 272.52, 434.61, 652.60])

# RNN - dane z pomiarów (iterations = N)
N_RNN = np.array([1000, 3000, 5000, 8000, 10000, 15000, 20000])
czas_RNN = np.array([615.01, 18175.32, 71811.95, 269378.82, 434141.71, 1372948.00, 3183923.87])

# RAND - dane z pomiarów (10000 iteracji)
N_RAND = np.array([5000, 10000, 15000, 20000, 25000, 30000, 33000])
czas_RAND = np.array([8710.98, 16093.16, 24067.89, 35839.15, 44361.21, 50065.19, 62575.13])

# --- Wspólny zakres N do osi X ---
N_all = np.array([1000, 3000, 5000, 8000, 10000, 15000, 20000,
                  25000, 30000, 33000, 35000])

fig, axes = plt.subplots(1, 2, figsize=(15, 6))
fig.suptitle("Porównanie czasu wykonania algorytmów – duże N", fontsize=13, fontweight="bold")

colors = {
    "NN":   "#5c8ee0",
    "RNN":  "#e05c5c",
    "RAND": "#5cc47c",
}

# --- Lewy: skala liniowa ---
ax1 = axes[0]
ax1.plot(N_NN,   czas_NN,   marker="s", linewidth=2, markersize=6, label="NN O(N²)",   color=colors["NN"])
ax1.plot(N_RNN,  czas_RNN,  marker="o", linewidth=2, markersize=6, label="RNN O(N³)",  color=colors["RNN"])
ax1.plot(N_RAND, czas_RAND, marker="D", linewidth=2, markersize=6, label="RAND O(N)",  color=colors["RAND"])

ax1.set_title("Skala liniowa")
ax1.set_xlabel("Liczba miast N")
ax1.set_ylabel("Średni czas [ms]")
ax1.legend()
ax1.grid(True, linestyle=":", alpha=0.5)
ax1.xaxis.set_major_formatter(ticker.FuncFormatter(lambda x, _: f"{int(x):,}"))
ax1.yaxis.set_major_formatter(ticker.FuncFormatter(lambda x, _: f"{x:,.0f} ms"))

# --- Prawy: skala logarytmiczna ---
ax2 = axes[1]
ax2.semilogy(N_NN,   czas_NN,   marker="s", linewidth=2, markersize=6, label="NN O(N²)",   color=colors["NN"])
ax2.semilogy(N_RNN,  czas_RNN,  marker="o", linewidth=2, markersize=6, label="RNN O(N³)",  color=colors["RNN"])
ax2.semilogy(N_RAND, czas_RAND, marker="D", linewidth=2, markersize=6, label="RAND O(N)",  color=colors["RAND"])

# Adnotacje ostatnich punktów
ax2.annotate(f"NN: {czas_NN[-1]:.0f} ms",
             (N_NN[-1], czas_NN[-1]), textcoords="offset points",
             xytext=(6, 4), fontsize=8, color=colors["NN"])
ax2.annotate(f"RNN: {czas_RNN[-1]/1000:.0f} s",
             (N_RNN[-1], czas_RNN[-1]), textcoords="offset points",
             xytext=(6, 4), fontsize=8, color=colors["RNN"])
ax2.annotate(f"RAND: {czas_RAND[-1]:.0f} ms",
             (N_RAND[-1], czas_RAND[-1]), textcoords="offset points",
             xytext=(6, -14), fontsize=8, color=colors["RAND"])

ax2.set_title("Skala logarytmiczna")
ax2.set_xlabel("Liczba miast N")
ax2.set_ylabel("Średni czas [ms] (skala log)")
ax2.legend()
ax2.grid(True, which="both", linestyle=":", alpha=0.5)
ax2.xaxis.set_major_formatter(ticker.FuncFormatter(lambda x, _: f"{int(x):,}"))
ax2.yaxis.set_major_formatter(ticker.FuncFormatter(lambda x, _: f"{x:g} ms"))

plt.tight_layout()
plt.savefig("wykres_porownanie_duze_N.png", dpi=150, bbox_inches="tight")
print("Zapisano: wykres_porownanie_duze_N.png")
plt.show()