#pragma once

#include "../CommonTypes.h"
#include "../DataStructures.h"
#include <cstdlib> 

struct Move {
    int i, j, cityI, cityJ;
    int cadence;
    int type; // 0=swap, 1=insert, 2=invert

    Move(int i=-1, int j=-1, int ci=-1, int cj=-1, int cadence=-1, int type=0) 
        : i(i), j(j), cityI(ci), cityJ(cj), cadence(cadence), type(type) {}

    bool operator==(const Move& other) const {
        if (type != other.type) return false;
        
        if (type == 0) {
            // swap
            return (cityI == other.cityI && cityJ == other.cityJ) ||
                   (cityI == other.cityJ && cityJ == other.cityI);
        } 
        else if (type == 1) {
            // insert
            return cityI == other.cityI && j == other.j;
        } 
        else {
            // invert
            return  (cityI == other.cityI && cityJ == other.cityJ) ||
                    (cityI == other.cityJ && cityJ == other.cityI);
        }
    }
};

class TabuSearch {
        static int RNN(int** matrix, int N, int* tour);
        static int randomTour(int** matrix, int N, int* tour, int seed=42);
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