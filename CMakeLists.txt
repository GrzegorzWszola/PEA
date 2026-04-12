cmake_minimum_required(VERSION 3.10)
project(AplikacjaPEA)
set(CMAKE_BUILD_TYPE Release)

include_directories(src/utils)

set(SOURCES
    src/main.cpp
    src/utils/DataParser.cpp
    src/utils/ConfigParser.cpp
    src/algorithms/Brute_Force.cpp
    src/algorithms/NN.cpp
    src/algorithms/RNN.cpp
    src/algorithms/RAND.cpp
    src/algorithms/BnB/BnB.cpp
)


add_executable(AplikacjaPEA ${SOURCES})