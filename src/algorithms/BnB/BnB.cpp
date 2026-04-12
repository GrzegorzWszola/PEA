#include "BnB.h"
#include <iostream>

Result* BnB::solve(int** subMatrix, int N, const std::string& sAlgo) {
    START_TIMER
    Result* result = new Result();

    // Stwórz węzeł początkowy
    BBNode root(N);
    copyMatrix(root.matrix, subMatrix, N);
    // oblicz startowe lb i zredukuj macierz
    root.lb = reduceMatrix(root.matrix, N);
    // startujemy z miasta 0
    root.visited[0] = 1;                  
    // miasto 0 jako pierwsze w sciezce
    root.path[0] = 0;                     
    root.level = 0;

    IDataStructure<BBNode>* ds = nullptr;

    if (sAlgo == "BFS") {
        // W przypadku BFS struktura danych to kolejka(queue)
        ds = new Queue<BBNode>(1024);
    } else if (sAlgo == "DFS") {
        // Przy DFS użwany jest stos(stack)
        ds = new Stack<BBNode>(1024);
    } else if (sAlgo == "Best_First") {
        // Dla best first używany jest priority queue(min-heap, sterta minimalna)
        ds = new PriorityQueue<BBNode>(1024);
    } else {
        ds = new PriorityQueue<BBNode>(1024);
    }

    // Wkladamu wezel poczatkowy do struktury danych
    ds->push(root);

    int best_cost = INF;
    int* best_path = new int[N + 1];


    // Glowna petla algorytmu
    while(!ds->empty()) {
        // Aktualne maisto
        BBNode curr(ds->pop());

        // Pruning (okrajamy) wezly ktore napewno beda gorsze
        if (curr.lb >= best_cost)
            continue;
        
        // Jezeli wszystkie miasta sa wyswietlone
        if (curr.level == N - 1) {
            // Znajdz ostatnia sciezke
            int final_edge = curr.matrix[curr.path[curr.level]][0];

            if (final_edge == INF) 
                continue;

            // Podlicz calkowity koszt
            int total_cost = curr.lb + final_edge;

            // Jezeli calkowity koszt jest lepszy niz najlepszy zamien wynik
            if (total_cost < best_cost) {
                best_cost = total_cost;
                for (int i = 0; i <= curr.level; i++)
                    best_path[i] = curr.path[i];
                best_path[curr.level + 1] = 0;
            }
            continue;
        }
        
        // Z aktualnego miasta przejdz przez wszytskich sasiadow
        int current_city = curr.path[curr.level];
        for (int i = 0; i < N; i++) {
            if (curr.matrix[current_city][i] != INF) {
                if (curr.visited[i])
                    continue;

                // Stworz wezel potomka
                BBNode child = createChild(curr, current_city, i, N);

                // Wepchnij tylko wezly ktore moga poprawic wynik
                if (child.lb < best_cost) 
                    ds->push(child);
            }
        }
    }

    result->dimension = N;
    result->length = best_cost;
    result->tour = best_path;
    result->time = STOP_TIMER

    delete ds;
    return result;
}

void BnB::copyMatrix(int** &to, int** from, int n) {
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            to[i][j] = (i == j) ? INF : from[i][j];
}

int BnB::reduceMatrix(int** &matrix, int n) {
    int reduction = 0;
    // Redukcja wierszy
    // Dla kazdego wiersza znajdz najmniejsza liczbe i zredukuje reszte elementow o nia
    for (int i = 0; i < n; i++) {
        int row_min = INF;

        // Znajdz najmniejsza
        for (int j = 0; j < n; j++) {
            if (matrix[i][j] < row_min) 
                row_min = matrix[i][j];
        }

        if (row_min != INF && row_min > 0) {
            reduction += row_min;

            // Zredukuje reszte elementow wiersza
            for (int j = 0; j < n; j++) {
                if (matrix[i][j] != INF)
                    matrix[i][j] -= row_min;
            }
        }
    }

    // Redukcja kolumn
    // Dla kazdej kolumny wykonaj to samo co dla wierszy
    for (int j = 0; j < n; j++) {
        int col_min = INF;

        // Znajdz najmniejsza
        for (int i = 0; i < n; i++) {
            if (matrix[i][j] < col_min) 
                col_min = matrix[i][j];
        }

        if (col_min != INF && col_min > 0) {
            reduction += col_min;

            // Zredukuje reszte elementow kolumny
            for (int i = 0; i < n; i++) {
                if (matrix[i][j] != INF)
                    matrix[i][j] -= col_min;
            }
        }
    }

    return reduction;
}

BBNode BnB::createChild(const BBNode &parent, int from, int to, int n) {
    BBNode child(n);
    copyMatrix(child.matrix, parent.matrix, n);

    // Zablokuj wszytskie krawedzie wezla rodzica
    for (int i = 0; i < n; i++)
        child.matrix[from][i] = INF;

    // Zablokuj wszytskie krawedzie wchodzace do wezla
    for (int i = 0; i < n; i++)
        child.matrix[i][to] = INF;

    // Zablokuj krawedz powrotna zeby nie tworzyc przedwczesnego cyklu
    child.matrix[to][from] = INF;

    // Koszt przejscia miedzy wezlami
    int edgecost = parent.matrix[from][to];

    // Koszt trasy to koszt rodzica plus wartosc miedzy wierzcholkami
    child.lb = parent.lb + edgecost;

    // Redukcja na macierzy dziecka
    int reduction = reduceMatrix(child.matrix, n);

    // Dodaj redukcje do aktualnego kosztu
    child.lb += reduction;

    // Zaktualizuj informacje
    child.level = parent.level + 1;
    for (int i = 0; i < n; i++) {
        child.path[i] = parent.path[i];
        child.visited[i] = parent.visited[i];
    }
    child.visited[to] = 1; 
    child.path[child.level] = to;
    return child;
}

