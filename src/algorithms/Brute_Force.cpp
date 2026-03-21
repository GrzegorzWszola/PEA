#include "Algorithms.h" 

Result* Algorithms::brute_force(int** matrix, int N) {
    int* currentTour = new int[N];
    for (int i = 0; i < N; i++)
        currentTour[i] = i;

    Result* result = new Result();

    int* bestTour = new int[N];
    int minCost = 1e18;
    START_TIMER


    do {
        int currentCost = 0;

        // Koszt dla aktualnej permutacji (miasto A do B az dojdziemy do N-1)
        for (int i = 0; i < N - 1; i++) {
            currentCost += matrix[currentTour[i]][currentTour[i+1]];
        }

        // Powrot do miasta startowego (miasto N-1 do miasta 0)
        currentCost += matrix[currentTour[N-1]][currentTour[0]];

        // Zapis gdy najkrotsza trasa
        if (currentCost < minCost) {
            minCost = currentCost;
            for (int i = 0; i < N; i++) 
                bestTour[i] = currentTour[i];
        }

    // Przestawia elementy na następną permutację i zwraca false gdy wszystkie permutacje zostały sprawdzone
    } while (std::next_permutation(currentTour + 1 /*pomijamy node 0*/, currentTour + N));



    result->time = STOP_TIMER
    result->dimension = N;
    result->tour = bestTour;
    result->length = minCost;
    
    delete[] currentTour;
    return result;
}
