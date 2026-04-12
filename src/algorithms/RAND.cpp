#include "Algorithms.h"
#include <random>

Result* Algorithms::RAND(int** matrix, int N, int iterations) {
    START_TIMER
    Result* best = nullptr;
    // Potrzebujemy iteracji, ile losowych permutacji chcemy sprawdzic
    if (iterations <= 0) {
        iterations = N;
    }

    // Inicjalizacja trasy
    int* tour = new int[N];
    for (int i = 0; i < N; i++)
        tour[i] = i;

    // Generator liczb losowych
    std::mt19937 rng(std::chrono::high_resolution_clock::now().time_since_epoch().count());

    for (int iter = 0; iter < iterations; iter++) {
        // Losowa permutacja z pominieciem miasta 0
        std::shuffle(tour + 1, tour + N, rng);

        // Oblicz koszt
        int cost = 0;
        for (int i = 0; i < N - 1; i++)
            cost += matrix[tour[i]][tour[i+1]];
        cost += matrix[tour[N-1]][tour[0]];

        // Zapisz jesli lepszy
        if (best == nullptr) {
            best = new Result();
            best->tour = new int[N];
            best->length = INT_MAX;
        }
        if (cost < best->length) {
            best->length = cost;
            best->dimension = N;
            for (int i = 0; i < N; i++)
                best->tour[i] = tour[i];
        }
    }

    best->dimension = N;
    best->time = STOP_TIMER
    
    delete[] tour;
    return best;
}