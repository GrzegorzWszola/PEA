#pragma once

#include <string>
#include "Data.h"

class DataParser {
    public:
        static Data* parseInputData(const std::string& inputFilePath, const std::string& inputOptimalSolution);
        static Data* parseMatrixData(const std::string& inputFilePath);
        static Data* generateRandomMatrix(int N, int low, int high, bool isSymmetric);
        static Data* parseATSP(const std::string& inputFilePath);
};