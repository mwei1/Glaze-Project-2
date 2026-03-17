//
// Created by lawre on 3/2/2026.
//

#ifndef GLAZE_PROJECT_2_CSVREADER_H
#define GLAZE_PROJECT_2_CSVREADER_H
#include <string>
#include <vector>
#include "Recipe.h"
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <iostream>

class CSVReader {
public:
    static std::vector<Recipe> loadRecipesFromFile(const std::string& fileName) {
        std::ifstream file(fileName);
        if (!file.is_open()) {
            throw std::runtime_error("File could not be opened.");
        }
        std::vector<Recipe> recipes;
        std::string line;
        if (!std::getline(file, line)) {
            return recipes;
        }
        while (std::getline(file, line)) {
            if (line.empty()) {
                continue;
            }
            std::stringstream ss(line);
            std::string name, prepTimestr, difficulty, mainIngredient, allergens;
            if (std::getline(ss, name, ',') && std::getline(ss, prepTimestr, ',') && std::getline(ss, difficulty, ',') && std::getline(ss, mainIngredient, ',')) {
                std::getline(ss, allergens);
                //must remove the trailing whitespace at the end of each line
                if (!allergens.empty() && allergens.back()=='\r') {
                    allergens.pop_back();
                }
                Recipe recipe;
                recipe.name = name;
                recipe.prepTime = std::stoi(prepTimestr);
                recipe.difficulty = difficulty;
                recipe.mainIngredient = mainIngredient;
                recipe.allergens = allergens;
                recipes.push_back(recipe);
            }
        }
        return recipes;
    }
};


#endif //GLAZE_PROJECT_2_CSVREADER_H