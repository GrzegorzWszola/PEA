#include <stdio.h>
#include <iostream>

#include "utils/Utils.h"
#include "utils/ConfigParser.h"
#include "utils/DataParser.h"
#include "algorithms/Algorithms.h"

int main(int argc, char *argv[]) {
    int exitcode = 0;
    if (argc != 2) {
        std::cerr << "Nie podano argumentu fukcji" << std::endl;
        return 1;
    }

    // Parsowanie konfigu i danych
    Config* config = nullptr;
    Data* data = nullptr;
    try {
        config = ConfigParser::parseConfig(argv[1]);
        config->setOutPath(Utils::generateOutPath(config->getOutPath()));
        config->print();

        if (config->getMode() == "correctness") {
            std::string ext = std::filesystem::path(config->getInputPath()).extension().string();    

            if (ext == ".txt") {
                data = DataParser::parseMatrixData(config->getInputPath());
            } else if (ext == ".tsp") {
                data = DataParser::parseInputData(config->getInputPath(), config->getOptimalPath());
            } else {
                throw std::runtime_error("Nieznany format pliku: " + ext);
            }
        }

        if (config->getMode() == "correctness" && config->getShowData()){
            data->print();
        }
        
        for (int N : config->getInstanceSizes()) {
            if (config->getMode() == "correctness" && N > data->getDimension()) {
                throw std::runtime_error("N nie moze byc wieksze nic wprowadzone dane");
            }
        }

        for (const auto& algo : config->getAlgorithms()) {
            if (config->getMode() == "correctness") {
                Utils::runAlgorithmsCorrectness(data, config, algo);
            } else if (config->getMode() == "performance") {
                Utils::runAlgorithmsPerformace(config, algo);
            } else {
                throw std::runtime_error("Nieznany tryb pracy");
            }    
        }
    } catch (const std::exception& e) {
        std::cerr << "Blad podczas wykonywania programu: " << e.what() << std::endl;
        exitcode = 1;
    }

    if (data != nullptr) {
        delete data;
    }
    delete config;
    return exitcode;
}