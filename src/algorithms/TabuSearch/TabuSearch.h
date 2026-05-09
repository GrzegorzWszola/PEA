#pragma once

#include "../CommonTypes.h"
#include "../DataStructures.h"
#include <cstdlib> 

struct Move {
    int i, j, cityI, cityJ;
    int cadence;
    Move(int i = -1, int j = -1, int ci = -1, int cj = -1, int cadence = -1) : i(i), j(j), cadence(cadence), cityI(ci), cityJ(cj) {}

    bool operator==(const Move& other) const {
        return (cityI == other.cityI && cityJ == other.cityJ) ||
               (cityI == other.cityJ && cityJ == other.cityI);
    }
};

class TabuSearch {
        static int NN(int** matrix, int N, int* tour);
        static int randomTour(int** matrix, int N, int* tour);
        static void applySwap(int* tour, int i, int j);
        static void applyInsert(int* tour, int N, int i, int j);
        static void applyInvert(int* tour, int i, int j);
        static int calcCost(int** matrix, int* tour, int N);
    public:
        static Result* solve(int** subMatrix,
                             int N,
                             int tabuSize,
                             int maxIterations,
                             int cadence,
                             int swapAlgo,
                             bool aspiration,
                             bool greedyStart);
};