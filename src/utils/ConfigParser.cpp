#include <../toml++/toml.hpp>
#include <stdexcept>
#include "ConfigParser.h"
#include "Utils.h"

Config* ConfigParser::parseConfig(const std::string& filePath) {
    if (!Utils::checkFileExists(filePath)) 
        throw std::runtime_error("Nie udalo sie otworzyc pliku konfiguracyjnego: " + filePath);

    auto tbl = toml::parse_file(filePath);

    // Wyciagamy algorytmy
    std::vector<std::string> algoList;
    if (auto* arr = tbl["config"]["algorithms"].as_array()) {
        for (auto&& el : *arr) {
            algoList.push_back(std::string(el.value_or("")));
        }
    }

    // Wyciagamy rozmiary instancji
    std::vector<int> instSizes;
    if (auto* arr = tbl["config"]["instance_sizes"].as_array()) {
        for (auto&& el : *arr) {
            instSizes.push_back(el.value_or(0));
        }
    }

    std::string inPath = tbl["data"]["input"].value_or("");
    std::string optPath = tbl["data"]["optimal"].value_or("");

    if (!Utils::checkFileExists(inPath) && tbl["config"]["mode"].value_or("") == "correctness")
        throw std::runtime_error("Podane sciezki nie istnieja");

    // Tworzenie i zwracanie obiektu
    return new Config(
        algoList,
        instSizes,
        tbl["config"]["repetitions"].value_or(1),
        tbl["data"]["input"].value_or(""),
        tbl["data"]["optimal"].value_or(""),
        tbl["config"]["mode"].value_or(""),
        tbl["data"]["output_path"].value_or(""),
        tbl["config"]["iterations"].value_or(0),
        tbl["config"]["start_from"].value_or(0),
        tbl["config"]["search_algorithm"].value_or("BFS"),
        tbl["config"]["is_symmetric"].value_or(false),
        tbl["data"]["show_data"].value_or(0),
        tbl["config"]["tabu_size"].value_or(3),
        tbl["config"]["cadence"].value_or(3),
        tbl["config"]["aspiration"].value_or(false),
        tbl["config"]["greedy"].value_or(true),
        tbl["config"]["swap_algo"].value_or(0),
        tbl["config"]["T0"].value_or(1.0),
        tbl["config"]["alpha"].value_or(1.0),
        tbl["config"]["epoch_length"].value_or(100),
        tbl["config"]["cooling_scheme"].value_or(0),
        tbl["data"]["optimal_value"].value_or(-1),
        tbl["config"]["population_size"].value_or(100),
        tbl["config"]["crossover_rate"].value_or(0.8),
        tbl["config"]["mutation_rate"].value_or(0.01),
        tbl["config"]["time_limit"].value_or(60),
        tbl["config"]["crossover_type"].value_or(0),
        tbl["config"]["mutation_type"].value_or(0)
    );
}