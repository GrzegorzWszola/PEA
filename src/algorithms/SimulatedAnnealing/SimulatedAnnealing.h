#pragma once

#include "../CommonTypes.h"
#include "../DataStructures.h"
#include <cmath>
#include <cstdlib>

class SimulatedAnnealing {
        static int NN(int** matrix, int N, int* tour);
        static int randomTour(int** matrix, int N, int* tour);
        static void applySwap(int* tour, int i, int j);
        static void applyInsert(int* tour, int N, int i, int j);
        static void applyInvert(int* tour, int i, int j);
        static int calcCost(int** matrix, int* tour, int N);
    public:
        static Result* solve(   int** matrix,
                                int N,
                                double T0,
                                double alpha,
                                int epochLength,
                                int coolingScheme,
                                int swapAlgo,
                                bool greedyStart);
};