#pragma once

#include <numeric>
#include <string>
#include <algorithm>
#include "CommonTypes.h"
#include "BnB/BnB.h"
#include "TabuSearch/TabuSearch.h"
#include "SimulatedAnnealing/SimulatedAnnealing.h"

class Algorithms {
    public:
        static Result* RAND(int** subMatrix, int N, int iterations);
        static Result* NN(int** subMatrix, int N, int start = 0);
        static Result* RNN(int** subMatrix, int N, int iterations);
        static Result* brute_force(int** subMatrix, int N);

        static inline Result* branch_and_bound(int** subMatrix, int N, const std::string& sAlgo) {
            return BnB::solve(subMatrix, N, sAlgo);
        };

        static inline Result* tabu_search(int** subMatrix,
                                          int N,
                                          int tabuSize,
                                          int maxIterations,
                                          int cadence,
                                          int swapAlgo,
                                          bool aspiration,
                                          bool greedyStart) 
        {
            return TabuSearch::solve(subMatrix, N, tabuSize, maxIterations, cadence, swapAlgo, aspiration, greedyStart);
        };

        static inline Result* simulated_annealing(  int** matrix,
                                                    int N,
                                                    double T0,
                                                    double alpha,
                                                    int epochLength,
                                                    int coolingScheme,
                                                    int swapAlgo,
                                                    bool greedyStart)
        {
            return SimulatedAnnealing::solve(matrix, N, T0, alpha, epochLength, coolingScheme, swapAlgo, greedyStart);
        }
};
