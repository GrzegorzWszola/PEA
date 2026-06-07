cmake_minimum_required(VERSION 3.10)
project(AplikacjaPEA)
option(USE_ASAN "Włącz AddressSanitizer do wykrywania wycieków" OFF)

set(CMAKE_BUILD_TYPE Release)

# Jeśli włączymy USE_ASAN, dodaj flagi dla kompilatora
if(USE_ASAN)
    if(MSVC)
        # Dla Visual Studio
        add_compile_options(/fsanitize=address)
    else()
        # Dla GCC (Linux/WSL/MinGW) lub Clang
        add_compile_options(-fsanitize=address -fno-omit-frame-pointer)
        add_link_options(-fsanitize=address)
    endif()
    message(STATUS "AddressSanitizer jest WŁĄCZONY")
endif()
include_directories(src/utils)
include_directories(src/algorithms)

set(SOURCES
    src/main.cpp
    src/utils/DataParser.cpp
    src/utils/ConfigParser.cpp
    src/algorithms/Brute_Force.cpp
    src/algorithms/NN.cpp
    src/algorithms/RNN.cpp
    src/algorithms/RAND.cpp
    src/algorithms/BnB/BnB.cpp
    src/algorithms/TabuSearch/TabuSearch.cpp
    src/algorithms/SimulatedAnnealing/SimulatedAnnealing.cpp
    src/algorithms/GeneticAlgorithm/GeneticAlgorithm.cpp
)


add_executable(AplikacjaPEA ${SOURCES})