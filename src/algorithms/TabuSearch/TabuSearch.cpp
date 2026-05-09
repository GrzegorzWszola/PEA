#include "TabuSearch.h"
#include <iostream>

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
        bestCost = NN(subMatrix, N, best);
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
                bool isTabu = tabuList.contains(Move(i, j, current[i], current[j]));
                if (isTabu && (!aspiration || cost >= bestCost)) continue;

                // Zapamiętaj jeśli lepszy
                if (cost < bestNeighborCost) {
                    bestNeighborCost = cost;
                    bestMove = Move(i, j, current[i], current[j], cadence);
                }
            }
        }

        // Brak więcej ruchów
        if (bestMove.i == -1 || bestMove.j == -1) break;
        // Dodajemy do tabu list
        tabuList.push_with_limit(bestMove, tabuSize);

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

int TabuSearch::NN(int** matrix, int N, int* tour) {
    bool* visited = new bool[N]();
    int cost = 0;

    tour[0] = 0;
    visited[0] = true;

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

    delete[] visited;
    return cost;
}

int TabuSearch::randomTour(int** matrix, int N, int* tour) {
    int cost = 0;
    for (int i = 0; i < N; i++) tour[i] = i;
    for (int i = N - 1; i > 0; i--) {
        int j = rand() % (i + 1);
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
