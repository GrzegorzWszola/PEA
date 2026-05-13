#include <stdexcept>
#include <fstream>
#include "DataParser.h"
#include "Utils.h"

Data* DataParser::parseInputData(const std::string& inputFilePath, const std::string& inputOptimalSolution) {
    int dimension = 0;
    Node* nodeList = nullptr;
    int** matrix = nullptr;
    OptimalSolution* optimal = nullptr;

    try {
        std::ifstream file(inputFilePath);
        std::ifstream optimalFile(inputOptimalSolution);

        if (!file.is_open()) 
            throw std::runtime_error("Nie mozna otworzyc pliku");

        // Wczytaj plik z danymi
        std::string line;
        while(getline(file, line)) {
            if (line.find("DIMENSION") != std::string::npos) {
                dimension = stoi(line.substr(line.find(":") + 1));
                nodeList = new Node[dimension];
                matrix = new int*[dimension];
                for (int i = 0; i < dimension; i++)
                    matrix[i] = new int[dimension];
            }
            if (line.find("NODE_COORD_SECTION") != std::string::npos)
                break; 
        }

        if (dimension == 0 || nodeList == nullptr)
            throw std::runtime_error("Brak sekcji DIMENSION w pliku");

        int id, px, py, i = 0;
        while(file >> id >> px >> py) {
            nodeList[i] = Node(id, px, py);
            i++;
        }

        for (int i = 0; i < dimension; i++)
            for (int j = 0; j < dimension; j++)
                matrix[i][j] = Utils::calculateDistance(nodeList[i], nodeList[j]);

        if (inputOptimalSolution == "") {
            std::cout << "Brak pliku z optymalnym rozwiazaniem!" << std::endl;
            return new Data(dimension, nodeList, matrix, nullptr);
        }

        // Wczytaj optymalny wynik
        optimal = new OptimalSolution();
        optimal->dimension = dimension;
        optimal->tour = new int[dimension];

        std::string optLine;
        while(getline(optimalFile, optLine)) {
            if (optLine.find("COMMENT") != std::string::npos) {
                size_t pos = optLine.find_last_of(" ");
                optimal->length = stoi(optLine.substr(pos + 1));
            }
            if (optLine.find("TOUR_SECTION") != std::string::npos)
                break;
        }

        int city, j = 0;
        while(optimalFile >> city && city != -1) {
            optimal->tour[j] = city;
            j++;
        }
        return new Data(dimension, nodeList, matrix, optimal);
    } catch (...) {
        delete[] nodeList;
        if (matrix) {
            for (int i = 0; i < dimension; i++) delete[] matrix[i];
            delete[] matrix;
        }
        if (optimal) { delete[] optimal->tour; delete optimal; }
        throw;
    }
}

Data* DataParser::parseATSP(const std::string& inputFilePath) {
    std::ifstream file(inputFilePath);
    if (!file.is_open())
        throw std::runtime_error("Nie mozna otworzyc pliku: " + inputFilePath);

    int dimension = 0;
    std::string line;
    
    // Czytaj naglowek
    while (getline(file, line)) {
        if (line.find("DIMENSION") != std::string::npos)
            dimension = stoi(line.substr(line.find(":") + 1));
        if (line.find("EDGE_WEIGHT_SECTION") != std::string::npos)
            break;
    }

    if (dimension == 0)
        throw std::runtime_error("Brak DIMENSION w pliku");

    // Alokacja macierzy
    int** matrix = new int*[dimension];
    for (int i = 0; i < dimension; i++)
        matrix[i] = new int[dimension];

    // Wczytaj macierz - zamieniaj 100000000 na 0 na przekatnej
    for (int i = 0; i < dimension; i++)
        for (int j = 0; j < dimension; j++) {
            file >> matrix[i][j];
            if (matrix[i][j] == 100000000)
                matrix[i][j] = 0;
        }

    Node* nodeList = new Node[dimension];
    for (int i = 0; i < dimension; i++)
        nodeList[i] = Node(i + 1, 0, 0);

    return new Data(dimension, nodeList, matrix, nullptr);
}

Data* DataParser::parseMatrixData(const std::string& inputFilePath) {
    std::ifstream file(inputFilePath);
    if (!file.is_open())
        throw std::runtime_error("Nie mozna otworzyc pliku: " + inputFilePath);

    int dimension = 0;
    file >> dimension;

    // Alokacja macierzy
    int** matrix = new int*[dimension];
    for (int i = 0; i < dimension; i++)
        matrix[i] = new int[dimension];

    // Wczytaj macierz
    for (int i = 0; i < dimension; i++)
        for (int j = 0; j < dimension; j++)
            file >> matrix[i][j];

    // Brak wspolrzednych wiec nodeList pusty
    Node* nodeList = new Node[dimension];
    for (int i = 0; i < dimension; i++)
        nodeList[i] = Node(i + 1, 0, 0);

    return new Data(dimension, nodeList, matrix, nullptr);
}

Data* DataParser::generateRandomMatrix(int N, int low, int high, bool symmetric) {
    std::mt19937 rng(std::random_device{}());
    std::uniform_int_distribution<int> dist(low, high);
    
    int** matrix = new int*[N];
    for (int i = 0; i < N; i++)
        matrix[i] = new int[N];

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (i == j) {
                matrix[i][j] = 0;
            } else if (symmetric && j < i) {
                matrix[i][j] = matrix[j][i];
            } else {
                matrix[i][j] = dist(rng);
            }
        }
    }

    Node* nodeList = new Node[N];
    for (int i = 0; i < N; i++)
        nodeList[i] = Node(i + 1, 0, 0);

    return new Data(N, nodeList, matrix, nullptr);
}