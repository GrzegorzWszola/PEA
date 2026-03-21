#include "Algorithms.h"

Result* Algorithms::RNN(int** matrix, int N, int iterations) {
    Result* best = nullptr;

    if (iterations < 0 || iterations >= N) {
        iterations = N;
    }

    START_TIMER

    for (int i = 0; i < iterations; i++) {
        Result* current = NN(matrix, N, i);

        if (best == nullptr) {
            best = current;
            continue;
        } else if (current->length < best->length) {
            delete best;
            best = current;
        } else {
            delete current;
        }
    }

    best->time = STOP_TIMER
    return best;
}