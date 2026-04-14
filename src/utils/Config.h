#pragma once

#include <vector>
#include <string>
#include <iostream>
#include <array>
#include <algorithm>

constexpr std::array<std::string_view, 5> VALID_ALGORITHMS = {"brute-force", "NN", "RNN", "RAND", "BnB"};
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
    public:
        Config( std::vector<std::string> algo, std::vector<int> inst, int rep, std::string inPath,
                std::string optPath, std::string mode, std::string outPath, int iterations, int startFrom, std::string searchAlgo, bool isSymmetric, int showData)
                : algorithms(algo), instanceSizes(inst), repetitions(rep), inputPath(inPath),
                optimalPath(optPath), mode(mode), outPath(outPath), iterations(iterations), startFrom(startFrom), showData(showData), isSymmetric(isSymmetric),
                searchAlgo(searchAlgo) {
                
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