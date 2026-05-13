#pragma once

#include <vector>
#include <string>
#include <iostream>
#include <array>
#include <algorithm>

constexpr std::array<std::string_view, 7> VALID_ALGORITHMS = {"brute-force", "NN", "RNN", "RAND", "BnB", "TS", "SA"};
constexpr std::array<std::string_view, 6> VALID_SEARCH_ALGORITHMS = {"BFS1", "DFS1", "Best_First1", "BFS", "DFS", "Best_First"};

class Config {
    private:
        std::vector<std::string> algorithms;
        std::vector<int> instanceSizes;
        int repetitions;
        int iterations;
        int startFrom;
        int showData;
        std::string inputPath, optimalPath, outPath;
        std::string mode;
        std::string searchAlgo;
        bool isSymmetric;
        int tabuSize;
        int cadence;
        bool aspiration;
        bool greedy;
        int swapAlgo;
        double T0;
        double alpha;
        int epochLength;
        int coolingScheme;
        int optimalValue;
    public:
        Config( std::vector<std::string> algo,
                std::vector<int> inst,
                int rep,
                std::string inPath,
                std::string optPath,
                std::string mode,
                std::string outPath,
                int iterations,
                int startFrom,
                std::string searchAlgo,
                bool isSymmetric,
                int showData,
                int tabuSize,
                int cadence,
                bool aspiration,
                bool greedy,
                int swapAlgo,
                double T0,
                double alpha,
                int epochLength,
                int coolingScheme,
                int optimalValue)
                    : algorithms(algo),
                    instanceSizes(inst),
                    repetitions(rep),
                    inputPath(inPath),
                    optimalPath(optPath),
                    mode(mode),
                    outPath(outPath),
                    iterations(iterations),
                    startFrom(startFrom),
                    showData(showData),
                    isSymmetric(isSymmetric),
                    searchAlgo(searchAlgo),
                    tabuSize(tabuSize),
                    cadence(cadence),
                    aspiration(aspiration),
                    greedy(greedy),
                    swapAlgo(swapAlgo),
                    T0(T0),
                    alpha(alpha),
                    epochLength(epochLength),
                    coolingScheme(coolingScheme),
                    optimalValue(optimalValue) {
                
                for (const auto& a : algorithms) {
                    if (std::find(VALID_ALGORITHMS.begin(), VALID_ALGORITHMS.end(), a) == VALID_ALGORITHMS.end())
                        throw std::runtime_error("Nierozpoznany algorytm: " + a);
                }
                
                if (std::find(VALID_SEARCH_ALGORITHMS.begin(), VALID_SEARCH_ALGORITHMS.end(), searchAlgo) == VALID_SEARCH_ALGORITHMS.end())
                    throw std::runtime_error("Nierozpoznany algorytm: " + searchAlgo);
            };

        const std::vector<std::string>& getAlgorithms() const { return algorithms; };
        const std::vector<int>& getInstanceSizes() const { return instanceSizes; };
        void setInstanceSizes(int x) { 
            if (instanceSizes.size() > 1)
                return;
            instanceSizes.push_back(x); 
        };
        int getRepetitions() const { return repetitions; };
        int getIterations() const { return iterations; };
        int getStartFrom() const { return startFrom; };
        int getShowData() const { return showData; };
        const std::string& getInputPath() const { return inputPath; };
        const std::string& getOptimalPath() const { return optimalPath; };
        const std::string& getMode() const { return mode; };
        const std::string& getOutPath() const { return outPath; };
        const std::string& getSearchAlgo() const  { return searchAlgo; };
        void setOutPath(const std::string& newPath) { this->outPath = newPath; };
        bool getIsSymmetric() const { return isSymmetric; };
        int getTabuSize() const { return tabuSize; };
        int getCadence() const { return cadence; };
        bool getAspiration() const { return aspiration; };
        bool getGreedy() const { return greedy; };
        int getSwapAlgo() const { return swapAlgo; };
        double getT0() const { return T0; };
        double getAlpha() const { return alpha; };
        int getEpochLength() const { return epochLength; };
        int getCoolingScheme() const { return coolingScheme; };
        int getOptimalValue() const { return optimalValue; };

        void print() const {
            std::cout << "--- Konfiguracja ---" << std::endl;
            std::cout << "Repetycje: " << repetitions << std::endl;
            std::cout << "Sciezka wejsciowa: " << inputPath << std::endl;
            std::cout << "Sciezka optymalna: " << optimalPath << std::endl;
            std::cout << "Sciezka wyjsciowa: " << outPath << std::endl;
            std::cout << "Wyswietl dane: " << showData << std::endl;
            std::cout << "Algorytmy: ";
            for (const auto& alg : algorithms) std::cout << alg << " ";
            std::cout << std::endl << "Wielkości instancji: ";
            for (const auto& inst: instanceSizes) std::cout << inst << " ";
            std::cout << "\n--------------------------" << std::endl;
        }
};