#pragma once

#include "../CommonTypes.h"
#include <random>

class GeneticAlgorithm {
    private:
        std::mt19937 gen;

        struct Individual {
            int* tour;
            int fitness;
            int n;

            // Konstruktor podstawowy
            Individual(int n) : n(n), fitness(0) {
                tour = new int[n];
            }

            // Konstruktor kopiujący
            Individual(const Individual& other) : n(other.n), fitness(other.fitness) {
                tour = new int[n];
                for (int i = 0; i < n; i++) tour[i] = other.tour[i];
            }

            // Operator przypisania
            Individual& operator=(const Individual& other) {
                if (this != &other) {
                    if (n != other.n) {
                        delete[] tour;
                        n = other.n;
                        tour = new int[n];
                    }
                    fitness = other.fitness;
                    for (int i = 0; i < n; i++) tour[i] = other.tour[i];
                }
                return *this;
            }

            // Destruktor
            ~Individual() {
                delete[] tour;
            }
        };

        // Pola przechowujące parametry (kontekst)
        int** matrix;
        int N;
        int populationSize;
        double crossoverRate;
        double mutationRate;
        int crossoverType;
        int mutationType;

        // Prywatny konstruktor instancji
        GeneticAlgorithm(int** matrix, int N, int popSize, double crossRate, double mutRate, int crossType, int mutType);

        // Metody algorytmu operujące na tablicach wskaźników
        void initializePopulation(Individual** population);
        int calculateFitness(int* tour);
        Individual* selection(Individual** population);
        
        void crossover(Individual* p1, Individual* p2, Individual* c1, Individual* c2);
        void mutate(Individual* ind);
        
        // Metody krzyżowania
        void crossoverPMX(Individual* p1, Individual* p2, Individual* c1, Individual* c2);
        void crossoverOX(Individual* p1, Individual* p2, Individual* c1, Individual* c2);

        // Mutacja
        void mutateSwap(Individual* ind);

    public:
        static Result* solve(int** matrix,
                            int N,
                            int populationSize,
                            double crossoverRate,
                            double mutationRate,
                            int timeLimit,
                            int crossoverType,
                            int mutationType);
};
