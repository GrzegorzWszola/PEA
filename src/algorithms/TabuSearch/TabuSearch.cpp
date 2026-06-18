#include "TabuSearch.h"
#include <iostream>
#include <random>

Result* TabuSearch::solve(int** subMatrix,
                          int N,
                          int tabuSize,
                          int maxIterations,
                          int cadence,
                          int swapAlgo,
                          bool aspiration,
                          bool greedyStart) 
{
    START_TIMER
    int* best = new int[N];
    int bestCost = 0;

    // Jezeli uzywamy greedy startu uzywamy NN do stworzenia listy i obliczenia kosztu
    if (greedyStart) {
        bestCost = RNN(subMatrix, N, best);
    } else {
        bestCost = randomTour(subMatrix, N, best);
    }

    // Tworzymy kolejke zakazanych ruchow
    List<Move> tabuList(tabuSize);

    int* current = new int[N];
    int* neighbor = new int[N];
    for (int i = 0; i < N; i++) current[i] = best[i];
    int currentCost = bestCost;

    for (int iter = 0; iter < maxIterations; iter++) {
        Move bestMove;
        int bestNeighborCost = INT_MAX;
        
        // Zmniejsz kadencje w liscie tabu i usun gdy osiagnie 0
        for (int i = 0; i < tabuList.getSize(); i++) {
            if (--tabuList.get(i).cadence == 0) {
                tabuList.remove(i);
                i--;
            }
        }

        // // Jeśli lista za duża
        while (tabuList.getSize() > tabuSize) {
            tabuList.remove(0);
        }

        // Przegladamy wszystkich sasiadow aby znalezc najlepszy ruch
        for (int i = 0; i < N - 1; i++) {
            for (int j = i + 1; j < N; j++) {

                // Skopiuj current do neighbor
                for (int k = 0; k < N; k++) neighbor[k] = current[k];

                // Zastosuj wybrany ruch
                if (swapAlgo == 0)       applySwap(neighbor, i, j);
                else if (swapAlgo == 1)  applyInsert(neighbor, N, i, j);
                else                     applyInvert(neighbor, i, j);

                // Oblicz koszt
                int cost = calcCost(subMatrix, neighbor, N);

                // Sprawdź tabu oraz aspiracje
                bool isTabu = tabuList.contains(Move(i, j, current[i], current[j], -1, swapAlgo));
                if (isTabu && (!aspiration || cost >= bestCost)) continue;

                // Zapamiętaj jeśli lepszy
                if (cost < bestNeighborCost) {
                    bestNeighborCost = cost;
                    bestMove = Move(i, j, current[i], current[j], cadence, swapAlgo);
                }
            }
        }

        // Brak więcej ruchów
        if (bestMove.i == -1 || bestMove.j == -1) break;
        // Dodajemy do tabu list
        tabuList.push(bestMove);

        // Wykonujemy najlepszy ruch
        if (swapAlgo == 0)       applySwap(current, bestMove.i, bestMove.j);
        else if (swapAlgo == 1)  applyInsert(current, N, bestMove.i, bestMove.j);
        else                     applyInvert(current, bestMove.i, bestMove.j);

        currentCost = bestNeighborCost;

        // Aktualizacja best
        if (currentCost < bestCost) {
            bestCost = currentCost;
            for (int i = 0; i < N; i++) best[i] = current[i];
        }
    }

    Result* result = new Result();
    result->tour = best;
    result->length = bestCost;
    result->dimension = N;
    result->time = STOP_TIMER
    delete[] current;
    delete[] neighbor;
    return result;
}

int TabuSearch::RNN(int** matrix, int N, int* bestTour) {
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

int TabuSearch::randomTour(int** matrix, int N, int* tour, int seed) {
    int cost = 0;
    for (int i = 0; i < N; i++) tour[i] = i;
    
    std::mt19937 rng(std::chrono::high_resolution_clock::now().time_since_epoch().count());
    // std::mt19937 rng(seed);  
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

void TabuSearch::applySwap(int* tour, int i, int j) {
    int tmp = tour[i];
    tour[i] = tour[j];
    tour[j] = tmp;
}

void TabuSearch::applyInsert(int* tour, int N, int i, int j) {
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

void TabuSearch::applyInvert(int* tour, int i, int j) {
    int left = i, right = j;
    while (left < right) {
        int tmp = tour[left];
        tour[left] = tour[right];
        tour[right] = tmp;
        left++;
        right--;
    }
}

int TabuSearch::calcCost(int** matrix, int* tour, int N) {
    int cost = 0;
    for (int k = 0; k < N - 1; k++)
        cost += matrix[tour[k]][tour[k + 1]];
    cost += matrix[tour[N-1]][tour[0]];
    return cost;
}
