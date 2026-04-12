#pragma once

#include <numeric>
#include <string>
#include <algorithm>
#include "CommonTypes.h"
#include "BnB/BnB.h"

class Algorithms {
    public:
        static Result* RAND(int** subMatrix, int N, int iterations);
        static Result* NN(int** subMatrix, int N, int start = 0);
        static Result* RNN(int** subMatrix, int N, int iterations);
        static Result* brute_force(int** subMatrix, int N);

        static inline Result* branch_and_bound(int** subMatrix, int N, const std::string& sAlgo) {
            return BnB::solve(subMatrix, N, sAlgo);
        };
};
