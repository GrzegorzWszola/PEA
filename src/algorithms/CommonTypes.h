#pragma once

#include <limits.h>
#include <chrono>

#define START_TIMER auto _start = std::chrono::high_resolution_clock::now();
#define STOP_TIMER std::chrono::duration<double, std::milli>( \
    std::chrono::high_resolution_clock::now() - _start).count();
const int INF = INT_MAX;

struct Result {
    int length;
    int* tour;
    int dimension;
    double time;

    Result() : length(0), tour(nullptr), dimension(0), time(0.0) {}

    ~Result() { delete[] tour; }
};
