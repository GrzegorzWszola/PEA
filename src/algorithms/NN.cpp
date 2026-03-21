#include "Algorithms.h"

Result* Algorithms::NN(int** matrix, int N, int start) {
    Result* result = new Result();
    START_TIMER

    int* tour = new int[N];
    bool* visited = new bool[N]();
    int cost = 0;

    tour[0] = start;
    visited[start] = true;

    for (int step = 1; step < N; step++) {
        int current = tour[step - 1];
        int minDist = 1e18;
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

    cost += matrix[tour[N - 1]][tour[0]];

    result->time = STOP_TIMER
    result->dimension = N;
    result->tour = tour;
    result->length = cost;

    return result;
}