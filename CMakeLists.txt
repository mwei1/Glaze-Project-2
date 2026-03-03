cmake_minimum_required(VERSION 3.26)
project(GlazeProject2)

set(CMAKE_CXX_STANDARD 17)

add_executable(GlazeProject2
        main.cpp
        Recipe.h
        Recipe.cpp
        CSVReader.h
        CSVReader.cpp
        Trie.cpp
        Trie.h
)