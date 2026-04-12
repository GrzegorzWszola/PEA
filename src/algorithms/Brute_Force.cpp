#include "Algorithms.h" 

Result* Algorithms::brute_force(int** matrix, int N) {
    START_TIMER
    // Aktualna permutacja miast
    int* currentTour = new int[N];
    for (int i = 0; i < N; i++)
        currentTour[i] = i;
    // Wynik koncowy
    Result* result = new Result();
    // Tablica uzywana do przechowywania najkrotszej sciezki
    int* bestTour = new int[N];
    // Przechowuje minimalny koszt aktualnej trasy
    int minCost = INT_MAX;


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


    result->dimension = N;
    result->tour = bestTour;
    result->length = minCost;
    result->time = STOP_TIMER
    
    delete[] currentTour;
    return result;
}
