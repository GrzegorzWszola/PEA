#include "Algorithms.h"

Result* Algorithms::NN(int** matrix, int N, int start) {
    START_TIMER
    Result* result = new Result();
    // Aktualna trasa
    int* tour = new int[N];
    // Lista odwiedzonych miast
    bool* visited = new bool[N]();
    // Koszt trasy
    int cost = 0;

    // Zaczynamy od miasta 'start' wiec mozemy oznaczyc je jako odwiedzone
    tour[0] = start;
    visited[start] = true;


    for (int step = 1; step < N; step++) {
        // Aktaulnie wybrane miasto
        int current = tour[step - 1];
        // Minimalny dystans do nastepnego miasta
        int minDist = INT_MAX;
        // Najblizsze miasto
        int nearest = -1;

        // Dla wybranego miasta znajdz najblizsza
        // trase do nieodwiedzonego miasta
        for (int j = 0; j < N; j++) {
            if (!visited[j] && matrix[current][j] < minDist) {
                minDist = matrix[current][j];
                nearest = j;
            }
        }

        // Zapisz wyniki
        tour[step] = nearest;
        visited[nearest] = true;
        cost += minDist;
    }
    // Dodaj trase z ostatniego miasta do miasta 'start'
    cost += matrix[tour[N - 1]][tour[0]];

    result->dimension = N;
    result->tour = tour;
    result->length = cost;
    result->time = STOP_TIMER

    return result;
}