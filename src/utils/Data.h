#pragma once

#include "Node.h"
#include<iomanip>
#include<iostream>

struct OptimalSolution {
    int length;
    int* tour;
    int dimension;
};

class Data {
    private:
        int dimension;
        Node* nodeList;
        // Macierz przechowujaca odleglosci pomiedzy "miastami"
        int** matrix;
        OptimalSolution* optimal;
    public:
        Data(int dimension, Node* nodeList, int** matrix, OptimalSolution* optimal = nullptr)
            : dimension(dimension), nodeList(nodeList), matrix(matrix), optimal(optimal) {}

        ~Data() {
            for (int i = 0; i < dimension; i++)
                delete[] matrix[i];
            delete[] matrix;
            delete[] nodeList;
            if (optimal != nullptr) {
                delete[] optimal->tour;
                delete optimal;
            }
        }       
        const int getDimension() const { return dimension; }
        const Node* getNodeList() const { return nodeList; }
        int** getMatrix() const { return matrix; } 
        const OptimalSolution* getOptimal() const { return optimal; }
        void print() const {
            std::cout << "--- Dane ---" << std::endl;
            std::cout << "Wielkość: " << dimension << std::endl;
            
            std::cout << "\nLista punktow:" << std::endl;
            for (int i = 0; i < dimension; i++) {
                std::cout << "  [" << nodeList[i].getId() << "] "
                        << "x=" << nodeList[i].getX() << " "
                        << "y=" << nodeList[i].getY() << std::endl;
            }

            std::cout << "\nMacierz odleglosci:" << std::endl;
            for (int i = 0; i < dimension; i++) {
                for (int j = 0; j < dimension; j++) {
                    std::cout << std::setw(8) << std::fixed << std::setprecision(2) << matrix[i][j];
                }
                std::cout << std::endl;
            }
            std::cout << "------------------------" << std::endl;
        }
        void printOptimal() const {
            if (optimal == nullptr) {
                std::cout << "Brak optymalnego rozwiazania" << std::endl;
                return;
            }
            std::cout << "--- Log: Optimal Solution ---" << std::endl;
            std::cout << "Length: " << optimal->length << std::endl;
            std::cout << "Tour: ";
            for (int i = 0; i < optimal->dimension; i++)
                std::cout << optimal->tour[i] << " ";
            std::cout << "\n-----------------------------" << std::endl;
        }

};