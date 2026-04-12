#include "Algorithms.h"
#include <iostream>

Result* Algorithms::RNN(int** matrix, int N, int iterations) {
    START_TIMER
    Result* best = nullptr;
    if (iterations <= 0 || iterations >= N) {
        iterations = N;
    }

   

    for (int i = 0; i < iterations; i++) {
        // Uruchamiamy algorytm NN dla kolejnych miast
        Result* current = NN(matrix, N, i);

        if (best == nullptr) {
            best = current;
            continue;
        } // Jezeli algorytm NN z 'i' miasta dal lepszy wynik zapisz go jako najlepszy
        else if (current->length < best->length) {
            delete best;
            best = current;
        } else {
            delete current;
        }
    }

    best->time = STOP_TIMER
    return best;
}