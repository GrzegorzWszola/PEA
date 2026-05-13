#include "SimulatedAnnealing.h"
#include <random>

Result* SimulatedAnnealing::solve(  int** matrix,
                                    int N,
                                    double T0,
                                    double alpha,
                                    int epochLength,
                                    int coolingScheme,
                                    int swapAlgo,
                                    bool greedyStart)
{
    START_TIMER
    int* best = new int[N];
    int bestCost = 0;

    // Rozwiazanie poczatkowe
    if (greedyStart)
        bestCost = RNN(matrix, N, best);
    else
        bestCost = randomTour(matrix, N, best);

    int* current = new int[N];
    for (int i = 0; i < N; i++) current[i] = best[i];
    int currentCost = bestCost;

    double T = T0;
    double Tmin = 0.001;

    int* neighbor = new int[N];

    while (T > Tmin) {
        // Epoka
        for (int e = 0; e < epochLength; e++) {

            // Generuj losowego sasiada
            int i = rand() % N;
            int j = rand() % N;
            while (i == j) j = rand() % N;
            if (i > j) { int tmp = i; i = j; j = tmp; }

            // Skopiuj current do neighbor
            for (int k = 0; k < N; k++) neighbor[k] = current[k];

            // Zastosuj ruch
            if (swapAlgo == 0)      applySwap(neighbor, i, j);
            else if (swapAlgo == 1) applyInsert(neighbor, N, i, j);
            else                    applyInvert(neighbor, i, j);

            // Oblicz koszt
            int neighborCost = calcCost(matrix, neighbor, N);
            int delta = neighborCost - currentCost;

            // Czy przyjac ruch
            if (delta < 0) {
                // jezeli lepszy zawsze przyjmij
                for (int k = 0; k < N; k++) current[k] = neighbor[k];
                currentCost = neighborCost;

                // aktualizacja
                if (currentCost < bestCost) {
                    bestCost = currentCost;
                    for (int k = 0; k < N; k++) best[k] = current[k];
                }
            } else {
                // jezeli gorszy przyjmij z prawdopodobienstwem e^(-delta/T)
                double probability = exp(-delta / T);
                double r = (double)rand() / RAND_MAX;
                if (r < probability) {
                    for (int k = 0; k < N; k++) current[k] = neighbor[k];
                    currentCost = neighborCost;
                }
            }
        }

        // Schlodz temperature
        if (coolingScheme == 0)
            T *= alpha;
        else
            T -= alpha;

        // Zabezpieczenie dla schematu liniowego
        if (T < Tmin) break;
    }

    delete[] neighbor;
    delete[] current;

    Result* result = new Result();
    result->tour = best;
    result->length = bestCost;
    result->dimension = N;
    result->time = STOP_TIMER
    return result;
}

int SimulatedAnnealing::RNN(int** matrix, int N, int* bestTour) {
    int bestCost = INT_MAX;

    bool* visited = new bool[N];
    int* tour = new int[N];

    for (int start = 0; start < N; start++) {
        // Reset
        for (int i = 0; i < N; i++) visited[i] = false;
        int cost = 0;

        tour[0] = start;
        visited[start] = true;

        for (int step = 1; step < N; step++) {
            int current = tour[step - 1];
            int minDist = INT_MAX;
            int nearest = -1;

            for (int j = 0; j < N; j++) {
                if (!visited[j] && matrix[current][j] < minDist) {
                    minDist = matrix[current][j];
                    nearest = j;
                }
            }

            tour[step] = nearest;
            visited[nearest] = true;
            cost += minDist;
        }
        cost += matrix[tour[N-1]][tour[0]];

        if (cost < bestCost) {
            bestCost = cost;
            for (int i = 0; i < N; i++) bestTour[i] = tour[i];
        }
    }

    delete[] visited;
    delete[] tour;
    return bestCost;
}

int SimulatedAnnealing::randomTour(int** matrix, int N, int* tour, int seed) {
    int cost = 0;
    for (int i = 0; i < N; i++) tour[i] = i;
    
    // std::mt19937 rng(std::chrono::high_resolution_clock::now().time_since_epoch().count());
    std::mt19937 rng(seed);
    for (int i = N - 1; i > 0; i--) {
        int j = std::uniform_int_distribution<int>(0, i)(rng);
        int tmp = tour[i];
        tour[i] = tour[j];
        tour[j] = tmp;
    }
    
    for (int i = 0; i < N - 1; i++)
        cost += matrix[tour[i]][tour[i + 1]];
    cost += matrix[tour[N-1]][tour[0]];
    return cost;
}

void SimulatedAnnealing::applySwap(int* tour, int i, int j) {
    int tmp = tour[i];
    tour[i] = tour[j];
    tour[j] = tmp;
}

void SimulatedAnnealing::applyInsert(int* tour, int N, int i, int j) {
    int city = tour[i];
    if (i < j) {
        for (int k = i; k < j; k++)
            tour[k] = tour[k + 1];
        tour[j] = city;
    } else {
        for (int k = i; k > j; k--)
            tour[k] = tour[k - 1];
        tour[j] = city;
    }
}

void SimulatedAnnealing::applyInvert(int* tour, int i, int j) {
    int left = i, right = j;
    while (left < right) {
        int tmp = tour[left];
        tour[left] = tour[right];
        tour[right] = tmp;
        left++;
        right--;
    }
}

int SimulatedAnnealing::calcCost(int** matrix, int* tour, int N) {
    int cost = 0;
    for (int k = 0; k < N - 1; k++)
        cost += matrix[tour[k]][tour[k + 1]];
    cost += matrix[tour[N-1]][tour[0]];
    return cost;
}