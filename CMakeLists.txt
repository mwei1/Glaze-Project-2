cmake_minimum_required(VERSION 3.28)
project(GlazeProject2)

set(CMAKE_CXX_STANDARD 17)

#Installed SFML through msys mingw https://packages.msys2.org/packages/mingw-w64-x86_64-sfml
find_package(SFML 3 COMPONENTS Graphics Window System Audio REQUIRED)

add_executable(GlazeProject2
        main.cpp
        Recipe.h
        Recipe.cpp
        CSVReader.h
        CSVReader.cpp
        Trie.cpp
        Trie.h
)

target_link_libraries(GlazeProject2 sfml-graphics sfml-audio sfml-window sfml-system)