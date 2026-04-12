#pragma once

#include "../CommonTypes.h"
#include "../DataStructures.h"
#include <string>
#include <iostream>

struct BBNode {
    int** matrix;
    int* path;
    int* visited;
    int lb;
    int level;
    int n;

    BBNode (int n = 0) {
        this->n = n;
        if (n == 0) {
            // bezpieczny pusty obiekt
            matrix = nullptr;
            path = nullptr;
            visited = nullptr;
            lb = 0;
            level = 0;
            return;
        }
        matrix = new int*[n];
        for (int i = 0; i < n; i++)
            matrix[i] = new int[n];

        path = new int[n]();
        visited = new int[n]();
        lb = 0;
        level = 0;
    }

    BBNode& operator=(const BBNode& other) {
        if (this == &other) return *this;
        static int count = 0;

        if (matrix) {
            for (int i = 0; i < n; i++)
                delete[] matrix[i];
            delete[] matrix;
        }
        delete[] path;
        delete[] visited;

        n = other.n;
        lb = other.lb;
        level = other.level;

        matrix = new int*[n];
        for (int i = 0; i < n; i++) {
            matrix[i] = new int[n];
            for (int j = 0; j < n; j++)
                matrix[i][j] = other.matrix[i][j];
        }

        path    = new int[n]();
        visited = new int[n]();
        for (int i = 0; i < n; i++) {
            path[i]    = other.path[i];
            visited[i] = other.visited[i];
        }

        return *this;
    }

    // Konstruktor kopiujący
    BBNode(const BBNode& other) {
        n = other.n;
        lb = other.lb;
        level = other.level;

        matrix = new int*[n];
        for (int i = 0; i < n; i++) {
            matrix[i] = new int[n];
            for (int j = 0; j < n; j++)
                matrix[i][j] = other.matrix[i][j];
        }

        path = new int[n];
        visited = new int[n];
        for (int i = 0; i < n; i++) {
            path[i]    = other.path[i];
            visited[i] = other.visited[i];
        }
    }

    ~BBNode () {
        if (matrix) {
            for (int i = 0; i < n; i++)
                delete[] matrix[i];
            delete[] matrix;
        }
        delete[] path;
        delete[] visited;
    }
};


class BnB {
        static int reduceMatrix(int** &matrix, int n);
        static BBNode createChild(const BBNode &parent, int from, int to, int n);
        static void copyMatrix(int** &to, int** from, int n) ;
    public:
        static Result* solve(int** subMatrix, int N, const std::string& sAlgo);
};