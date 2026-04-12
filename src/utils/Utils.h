#pragma once

#include <filesystem>
#include <fstream>
#include <iostream>
#include <cmath>
#include <iomanip>
#include <string>
#include <random>
#include <chrono>
#include <ctime>
#include "Node.h"
#include "../algorithms/Algorithms.h"
#include "Data.h"
#include "Config.h"
#include "DataParser.h"

namespace Utils {
    inline bool checkFileExists(const std::string& path) {
        if (!std::filesystem::exists(path)) {
            std::cerr << "[Error]: File " << path << " not found!\n";
            return false;
        }
        return true;
    }

    inline std::string generateOutPath(const std::string& basePath) {
        auto now = std::chrono::system_clock::now();
        std::time_t t = std::chrono::system_clock::to_time_t(now);
        std::tm* tm = std::localtime(&t);
        
        char timestamp[20];
        std::strftime(timestamp, sizeof(timestamp), "%Y-%m-%d_%H-%M-%S", tm);
        
        size_t dot = basePath.find_last_of(".");
        if (dot == std::string::npos)
            return basePath + "_" + timestamp;
        return basePath.substr(0, dot) + "_" + timestamp + basePath.substr(dot);
    }

    // Oblicza odleglosc euklidesowa pomiedzy punktami
    inline double calculateDistance(const Node& a, const Node& b) {
        double dx = a.getX() - b.getX();
        double dy = a.getY() - b.getY();
        return std::round(std::sqrt(dx * dx + dy * dy));
    }

    // Wylicza podmacierz na podstawie danych z pliku aby dopasowac ilosc danych do N
    inline int** extractSubMatrix(int** matrix, int n) {
        int** sub = new int*[n];
        for (int i = 0; i < n; i++) {
            sub[i] = new int[n];
            for (int j = 0; j < n; j++) {
                sub[i][j] = matrix[i][j];
            }
        }
        return sub;
    }

    inline void printAndSaveResult( const Result* result,
                                    const Data* data,
                                    const std::string& outPath,
                                    const std::string& algo,
                                    double error,
                                    int N,
                                    int iterations,
                                    const std::string& sAlgo,
                                    int offset = 0) {
        // Wyswietl w konsoli
        std::cout << "Algorytm:" << algo << std::endl;
        if (algo == "BnB") std::cout << "Algorytm przeszukiwania: " << sAlgo << std::endl;
        std::cout << "Ilosc miast (N): " << N << std::endl;
        std::cout << "Trasa dla punktow: " << offset + 1 << ", do: " << offset + N << std::endl;
        std::cout << "Dlugosc trasy: " << result->length << std::endl;
        if (data->getOptimal() != nullptr && N == data->getDimension())
            std::cout << "Dlugosc trasy optymalnej: " << data->getOptimal()->length << std::endl;
        std::cout << "Czas wykonywania: " << result->time << std::endl;
        std::cout << "Blad: " << error << "%" << std::endl;
        if (algo == "RAND") 
            std::cout << "iteracje: " << iterations << std::endl;
        if (algo == "BnB") {
            std::cout << "Szczytowa wielkosc: " << result->peak_size << std::endl;
            std::cout << "Ilość odwiedzonych wierzchołków: " << result->visited_nodes << std::endl;
        }
        if (data->getDimension() < 200) {
            std::cout << "Trasa: ";
            for (int i = 0; i < result->dimension; i++)
                std::cout << data->getNodeList()[result->tour[i] + offset].getId() << " -> ";
            std::cout << data->getNodeList()[result->tour[0] + offset].getId() << std::endl;
            std::cout << "--------------" << std::endl;
        }


        // Zapisz do pliku
        std::ofstream file(outPath, std::ios::app);
        if (!file.is_open()) {
            std::cerr << "Nie mozna otworzyc pliku: " << outPath << std::endl;
            return;
        }

        file << "Algorytm: " << algo << " Ilosc miast (N): " << N << "\n";  
        if (algo == "BnB") file << "Algorytm przeszukiwania: " << sAlgo << "\n";
        file << "dlugosc: " << result->length << "\n";
        if (data->getOptimal() != nullptr && N == data->getDimension())
            file << "Dlugosc trasy optymalnej: " << data->getOptimal()->length << std::endl;
        file << "Czas wykonywania: " << result->time << "\n";
        file << "Blad: " << error << "%" << "\n";
        if (algo == "RAND") {
            file << "iteracje: " << iterations << "\n";
        }
        if (algo == "BnB") {
            file << "Szczytowa wielkosc: " << result->peak_size << "\n";
            file << "Ilość odwiedzonych wierzchołków: " << result->visited_nodes << "\n";
        }
        file << "trasa: ";
        for (int i = 0; i < result->dimension; i++)
            file << data->getNodeList()[result->tour[i] + offset].getId() << " -> ";
        file << data->getNodeList()[result->tour[0] + offset].getId() << "\n";
        file << "--------------\n";

        file.close();
    }

    inline void printAndSavePerformanceResults(const std::string& outPath,
                                               const std::string& algo, 
                                               int N,
                                               double avgTime) {
        // Wyswietl w konsoli
        std::cout << "Algorytm: " << algo << std::endl;
        std::cout << "Ilosc miast (N): " << N << std::endl;
        std::cout << "Sredni czas: " << std::fixed << std::setprecision(4) << avgTime << " ms" << std::endl;
        std::cout << "------------------------" << std::endl;

        // Zapisz do pliku
        std::ofstream file(outPath, std::ios::app);
        if (!file.is_open()) {
            std::cerr << "Nie mozna otworzyc pliku: " << outPath << std::endl;
            return;
        }

        if (file.tellp() == 0)
            file << "algorytm;N;sredni_czas_ms;srednia_dlugosc\n";

        file << algo << ";" 
            << N << ";" 
            << std::fixed << std::setprecision(4) << avgTime << ";" ;
        file << '\n';
        file.close();
    }

    inline double compareResults(const Data* data, const Result* result, int** subMatrix, int N) {
        if (data->getOptimal() != nullptr && N == data->getDimension()) {
            // Optymalne rozwiazanie z pliku
            return ((double)(result->length - data->getOptimal()->length) / data->getOptimal()->length) * 100;
        } else if (N <= 15) {
            // Oblicz brute force dla malych instancji
            Result* optimalSolution = Algorithms::branch_and_bound(subMatrix, N, "Best_First1");
            double error = ((double)(result->length - optimalSolution->length) / optimalSolution->length) * 100;
            delete optimalSolution;
            return error;
        } else {
            // Za duze N zeby liczyc brute force - blad nieznany
            return -1;
        }
    }

    inline void runAlgorithmsPerformace(Config* config, const std::string& algoName) {
        std::cout << "--- Wynik wydajnosci ---" << std::endl;
        if (config->getInstanceSizes().empty())
            return;
        
        for (int N : config->getInstanceSizes()) {   

            double totalTime = 0;
            for (int rep = 0; rep < config->getRepetitions(); rep++) {
                Data* data = DataParser::generateRandomMatrix(N);
                Result* result = nullptr;

                if (algoName == "brute-force") {
                    result = Algorithms::brute_force(data->getMatrix(), N);
                } else if (algoName == "NN") {
                    result = Algorithms::NN(data->getMatrix(), N);
                } else if (algoName == "RNN") {
                    result = Algorithms::RNN(data->getMatrix(), N, config->getIterations());
                } else if (algoName == "RAND") {
                    result = Algorithms::RAND(data->getMatrix(), N, config->getIterations());
                } else if (algoName == "BnB") {
                    result = Algorithms::branch_and_bound(data->getMatrix(), N, config->getSearchAlgo());
                }

                delete data;
                if (result == nullptr) continue;

                totalTime += result->time;
                delete result;
            }

            double avgTime = totalTime / config->getRepetitions();
            Utils::printAndSavePerformanceResults(config->getOutPath(), algoName, N, avgTime);
        }
    }

    inline void runAlgorithmsCorrectness(const Data* data, Config* config, const std::string& algoName) {   
        std::cout << "--- Wynik ---" << std::endl;
        if (config->getInstanceSizes().empty())
            config->setInstanceSizes(data->getDimension());

        for (int N : config->getInstanceSizes()) {
            int startOffset = config->getStartFrom();
            if (config->getStartFrom() + N > data->getDimension())
                throw std::runtime_error("start_from + N przekracza rozmiar danych dla N=" + std::to_string(N));

            int** subMatrix = Utils::extractSubMatrix(data->getMatrix(), N);
            Result* result = nullptr;
            double error = 0;

            if (algoName == "brute-force") {
                result = Algorithms::brute_force(subMatrix, N);
            } else if (algoName == "NN") {
                result = Algorithms::NN(subMatrix, N);
                error = Utils::compareResults(data, result, subMatrix, N);
            } else if (algoName == "RNN") {
                result = Algorithms::RNN(subMatrix, N, config->getIterations());
                error = Utils::compareResults(data, result, subMatrix, N);
            } else if (algoName == "RAND") {
                result = Algorithms::RAND(subMatrix, N, config->getIterations());
                error = Utils::compareResults(data, result, subMatrix, N);
            } else if (algoName == "BnB") {
                result = Algorithms::branch_and_bound(subMatrix, N, config->getSearchAlgo());
                error = Utils::compareResults(data, result, subMatrix, N);
            }
            
            for (int i = 0; i < N; i++) delete[] subMatrix[i];
            delete[] subMatrix;

            if (result == nullptr) continue;

            Utils::printAndSaveResult(result, data, config->getOutPath(), algoName, error, N, config->getIterations(), config->getSearchAlgo(), startOffset);
            delete result;
        }
    }
}
