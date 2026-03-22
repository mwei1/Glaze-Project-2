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
2. Ensure SFML and CMake are configured
3. Ensure the Working Directory is set to the project directory
<img width="823" height="679" alt="image" src="https://github.com/user-attachments/assets/df3faae0-6977-4236-9806-b5790f507a67" />

4. Click "Run"
   - Upon running the Program, the application will:
       - Load 100,000 recipes from the dataset
       - Insert data into the Trie and Hash Table
       - Allow users to search recipes via the SFML interface
       - Display matching recipes, detailed information, and the execution time

### Configuring SFML
1. [Install MINGW through MSYS2](https://www.msys2.org/)
2. Run `pacman -S mingw-w64-x86_64-sfml` to install sfml
3. Switch to the MSYS2 version of MINGW in CLion Toolchains
<img width="977" height="723" alt="image" src="https://github.com/user-attachments/assets/72d1304a-9a7a-47c1-808a-ff05dfa96e3e" />

