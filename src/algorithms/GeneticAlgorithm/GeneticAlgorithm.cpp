#include "GeneticAlgorithm.h"
#include "../../utils/Utils.h"
#include <algorithm>
#include <random>
#include <chrono>

Result* GeneticAlgorithm::solve(int** matrix,
                                int N,
                                int populationSize,
                                double crossoverRate,
                                double mutationRate,
                                int timeLimit,
                                int crossoverType,
                                int mutationType) 
{
    // Inicjalizacja instancji algorytmu (generator gen jest tworzony w konstruktorze)
    GeneticAlgorithm ga(matrix, N, populationSize, crossoverRate, mutationRate, crossoverType, mutationType);
    
    START_TIMER
    
    // Alokacja buforów populacji
    Individual** popCurrent = new Individual*[populationSize];
    Individual** popNext = new Individual*[populationSize];
    for (int i = 0; i < populationSize; i++) {
        popCurrent[i] = new Individual(N);
        popNext[i] = new Individual(N);
    }

    ga.initializePopulation(popCurrent);

    int bestLength = INF;
    int* bestTour = new int[N];

    auto startTime = std::chrono::high_resolution_clock::now();
    std::uniform_real_distribution<double> d01(0.0, 1.0);

    while (true) {
        auto currentTime = std::chrono::high_resolution_clock::now();
        if (std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - startTime).count() >= timeLimit) break;

        // Elityzm
        int bestIdx = 0;
        for (int i = 1; i < populationSize; i++) {
            if (popCurrent[i]->fitness < popCurrent[bestIdx]->fitness) bestIdx = i;
        }
        
        if (popCurrent[bestIdx]->fitness < bestLength) {
            bestLength = popCurrent[bestIdx]->fitness;
            for(int j = 0; j < N; j++) bestTour[j] = popCurrent[bestIdx]->tour[j];
        }

        *popNext[0] = *popCurrent[bestIdx];

        // Tworzenie nowej generacji
        for (int i = 1; i < populationSize; i += 2) {
            Individual* p1 = ga.selection(popCurrent);
            Individual* p2 = ga.selection(popCurrent);

            int attempts = 0;
            while (p1 == p2 && attempts < 10) {
                p2 = ga.selection(popCurrent);
                attempts++;
            }

            if (i + 1 < populationSize) {
                if (d01(ga.gen) < crossoverRate) {
                    ga.crossover(p1, p2, popNext[i], popNext[i+1]);
                } else {
                    *popNext[i] = *p1;
                    *popNext[i+1] = *p2;
                }
                ga.mutate(popNext[i]);
                ga.mutate(popNext[i+1]);
                popNext[i]->fitness = ga.calculateFitness(popNext[i]->tour);
                popNext[i+1]->fitness = ga.calculateFitness(popNext[i+1]->tour);
            } else {
                *popNext[i] = *p1;
                ga.mutate(popNext[i]);
                popNext[i]->fitness = ga.calculateFitness(popNext[i]->tour);
            }
        }

        // Swap buforów
        Individual** temp = popCurrent;
        popCurrent = popNext;
        popNext = temp;
    }
    
    Result* result = new Result();
    result->dimension = N;
    result->length = bestLength;
    result->tour = new int[N];
    for (int i = 0; i < N; i++) result->tour[i] = bestTour[i];
    result->time = STOP_TIMER

    for (int i = 0; i < populationSize; i++) {
        delete popCurrent[i];
        delete popNext[i];
    }
    delete[] popCurrent;
    delete[] popNext;
    delete[] bestTour;

    return result;
}

// Konstruktor z inicjalizacją generatora
GeneticAlgorithm::GeneticAlgorithm(int** matrix, int N, int popSize, double crossRate, double mutRate, int crossType, int mutType)
    : matrix(matrix), N(N), populationSize(popSize), 
      crossoverRate(crossRate), mutationRate(mutRate), 
      crossoverType(crossType), mutationType(mutType) 
{
    std::random_device rd;
    gen.seed(rd());
}

void GeneticAlgorithm::initializePopulation(Individual** population) {
    int* baseTour = new int[N];
    for (int i = 0; i < N; i++) baseTour[i] = i;

    for (int i = 0; i < populationSize; i++) {
        for (int j = 0; j < N; j++) population[i]->tour[j] = baseTour[j];
        for (int j = N - 1; j > 0; j--) {
            std::uniform_int_distribution<int> dist(0, j);
            std::swap(population[i]->tour[j], population[i]->tour[dist(gen)]);
        }
        population[i]->fitness = calculateFitness(population[i]->tour);
    }
    delete[] baseTour;
}

int GeneticAlgorithm::calculateFitness(int* tour) {
    int cost = 0;
    for (int i = 0; i < N - 1; i++) cost += matrix[tour[i]][tour[i + 1]];
    cost += matrix[tour[N - 1]][tour[0]];
    return cost;
}

GeneticAlgorithm::Individual* GeneticAlgorithm::selection(Individual** population) {
    std::uniform_int_distribution<int> dist(0, populationSize - 1);
    Individual* best = population[dist(gen)];
    for (int i = 1; i < 10; i++) {
        Individual* contestant = population[dist(gen)];
        if (contestant->fitness < best->fitness) best = contestant;
    }
    return best;
}

void GeneticAlgorithm::mutate(Individual* ind) {
    std::uniform_int_distribution<int> dist(0, N - 1);
    int i = dist(gen), j = dist(gen);
    if (i > j) std::swap(i, j);
    std::reverse(ind->tour + i, ind->tour + j + 1);
}

void GeneticAlgorithm::crossover(Individual* p1, Individual* p2, Individual* c1, Individual* c2) {
    if (crossoverType == 1) crossoverOX(p1, p2, c1, c2);
    else crossoverPMX(p1, p2, c1, c2);
}

void GeneticAlgorithm::crossoverPMX(Individual* p1, Individual* p2, Individual* c1, Individual* c2) {
    std::uniform_int_distribution<int> dist(0, N - 1);
    int cut1 = dist(gen), cut2 = dist(gen);
    if (cut1 > cut2) std::swap(cut1, cut2);

    for (int i = 0; i < N; i++) { c1->tour[i] = -1; c2->tour[i] = -1; }
    for (int i = cut1; i <= cut2; i++) { c1->tour[i] = p1->tour[i]; c2->tour[i] = p2->tour[i]; }

    auto mapChild = [&](Individual* child, Individual* parent1, Individual* parent2) {
        for (int i = cut1; i <= cut2; i++) {
            int val = parent2->tour[i];
            bool found = false;
            for (int j = cut1; j <= cut2; j++) if (child->tour[j] == val) { found = true; break; }
            if (!found) {
                int curr = i;
                while (true) {
                    int vP1 = parent1->tour[curr];
                    int next = -1;
                    for (int k = 0; k < N; k++) if (parent2->tour[k] == vP1) { next = k; break; }
                    if (next < cut1 || next > cut2) { child->tour[next] = val; break; }
                    curr = next;
                }
            }
        }
        for (int i = 0; i < N; i++) if (child->tour[i] == -1) child->tour[i] = parent2->tour[i];
    };
    mapChild(c1, p1, p2);
    mapChild(c2, p2, p1);
}

void GeneticAlgorithm::crossoverOX(Individual* p1, Individual* p2, Individual* c1, Individual* c2) {
    std::uniform_int_distribution<int> dist(0, N - 1);
    int c1_idx = dist(gen), c2_idx = dist(gen);
    if (c1_idx > c2_idx) std::swap(c1_idx, c2_idx);

    auto fillChild = [&](Individual* child, Individual* parent1, Individual* parent2) {
        bool* taken = new bool[N]{false};
        for (int i = c1_idx; i <= c2_idx; i++) {
            child->tour[i] = parent1->tour[i];
            taken[parent1->tour[i]] = true;
        }
        int curr = (c2_idx + 1) % N;
        for (int i = 0; i < N; i++) {
            int city = parent2->tour[(c2_idx + 1 + i) % N];
            if (!taken[city]) {
                child->tour[curr] = city;
                curr = (curr + 1) % N;
            }
        }
        delete[] taken;
    };
    fillChild(c1, p1, p2);
    fillChild(c2, p2, p1);
}

void GeneticAlgorithm::mutateSwap(Individual* ind) {
    std::uniform_int_distribution<int> dist(0, N - 1);
    std::swap(ind->tour[dist(gen)], ind->tour[dist(gen)]);
}
