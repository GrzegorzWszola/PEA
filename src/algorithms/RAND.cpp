#include "Algorithms.h"
#include <random>

Result* Algorithms::RAND(int** matrix, int N, int iterations) {
    Result* best = nullptr;

    if (iterations <= 0) {
        iterations = N;
    }

    START_TIMER

    // Generuj losowe trasy
    int* tour = new int[N];
    tour[0] = 0;
    for (int i = 1; i < N; i++)
        tour[i] = i;

    std::mt19937 rng(std::chrono::high_resolution_clock::now().time_since_epoch().count());
    std::uniform_int_distribution<int> dist(1, N - 1);

    for (int iter = 0; iter < iterations; iter++) {
        // Losowa permutacja
        for (int i = N - 1; i > 0; i--) {
            int j = dist(rng) % i + 1;
            std::swap(tour[i], tour[j]);
        }

        // Oblicz koszt
        int cost = 0;
        for (int i = 0; i < N - 1; i++)
            cost += matrix[tour[i]][tour[i+1]];
        cost += matrix[tour[N-1]][tour[0]];

        // Zapisz jesli lepszy
        if (best == nullptr || cost < best->length) {
            if (best == nullptr) best = new Result();
            best->length = cost;
            best->dimension = N;
            if (best->tour == nullptr) best->tour = new int[N];
            for (int i = 0; i < N; i++)
                best->tour[i] = tour[i];
        }
    }

    best->time = STOP_TIMER
    best->dimension = N;
    
    delete[] tour;
    return best;
}