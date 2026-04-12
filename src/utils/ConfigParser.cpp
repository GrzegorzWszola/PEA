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
        tbl["data"]["show_data"].value_or(0)
    );
}