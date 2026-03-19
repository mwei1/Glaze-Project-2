<H1> COP3530 - Project 2 </h1>
<H2> Glaze - Dessert Recipe Search</H2>

## Project Overview
The group, Glaze, built a C++ application that stores and searches through a dataset of 100,000 dessert recipes. The focus of the project is to compare the performance of a Trie and a Hash Table data structures, for efficient search operations.

---

## Features
- Load 100,000 recipe dataset from CSV file
- Trie-based insertion of recipe names
- Exact search by full recipe name
- Prefix search (applicable to Trie implementation only)
- Displays recipe details:
  - Recipe Name
  - Preparation Time
  - Difficulty
  - Main Ingredient
  - Allergens
 
---

## How to Build and Run

### Using CLion
1. Open the project in CLion
2. Ensure the CMake is configured
3. Click "Run"

### Configuring SFML
1. Install MINGW through MSYS2
2. Run `pacman -S mingw-w64-x86_64-sfml` to install sfml
3. Switch to the MSYS2 version of MINGW in CLion Toolchains
