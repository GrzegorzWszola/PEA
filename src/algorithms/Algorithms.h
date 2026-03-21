#pragma once

#include <numeric>
#include <algorithm>
#include <chrono>

#define START_TIMER auto _start = std::chrono::high_resolution_clock::now();
#define STOP_TIMER std::chrono::duration<double, std::milli>( \
    std::chrono::high_resolution_clock::now() - _start).count();

struct Result {
    int length;
    int* tour;
    int dimension;
    double time;

    ~Result() { delete[] tour; }
};

class Algorithms {
    public:
        static Result* RAND(int** subMatrix, int N, int iterations);
        static Result* NN(int** subMatrix, int N, int start = 0);
        static Result* RNN(int** subMatrix, int N, int iterations);
        static Result* brute_force(int** subMatrix, int N);
};