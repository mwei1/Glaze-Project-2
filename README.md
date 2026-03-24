<H1> Glaze Dessert Search </h1>

### COP3530 Project 2 Group 31

Team Members: Erica Lawrence, Zlata Kovrigina, Michael Wei

## Project Overview
Our group, Glaze, built a C++ application that stores and searches through a dataset of 100,000 dessert recipes in both a Trie and a Hash Table, enabling comparison of these data structures for efficient search operations.

[Project Demonstration](https://youtu.be/pDI-Ai6rPNk )

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
## UI Elements
- Toggle Mute/Unmute button for background Music
- Random Search via clicking the Donut
- Toggle Trie Prefix Search (autofill based on string)
- Switch between using Trie/Hash Table to search
- Case-Sensitive responsive search bar that updates as characters are typed
- Recipe Info Display upon search (must click displayed search result from under bar first)
<img width="1199" height="830" alt="sampleui" src="https://github.com/user-attachments/assets/ef7a0a17-fa5a-4ddc-a075-41ab38012b28" />

---

## How to Build and Run

### Installing CLion and MINGW
1. [Install CLion](https://www.jetbrains.com/help/clion/installation-guide.html) by JetBrains
2. [Install MINGW through MSYS2](https://www.msys2.org/)
3. Set CLion Toolchain to MSYS2's MINGW (
<img width="977" height="723" alt="image" src="https://github.com/user-attachments/assets/72d1304a-9a7a-47c1-808a-ff05dfa96e3e" />

### Installing SFML
1. Open MSYS2
2. Run `pacman -S mingw-w64-x86_64-sfml` in the terminal to install SFML

### Using CLion
1. Clone the project in CLion using Git
2. Ensure SFML and CMake are configured
3. Ensure the Working Directory is set to the project directory
<img width="823" height="679" alt="image" src="https://github.com/user-attachments/assets/df3faae0-6977-4236-9806-b5790f507a67" />

4. Click "Run"
   - Upon running the Program, the application will:
       - Load 100,000 recipes from the dataset
       - Insert data into the Trie and Hash Table
       - Open an SFML Window to allow users to search recipes
       - Display matching recipes, detailed information, and the execution time within the window

--- 
