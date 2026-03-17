//
// Created by lawre on 3/2/2026.
//

#ifndef GLAZE_PROJECT_2_RECIPE_H
#define GLAZE_PROJECT_2_RECIPE_H
#include <string>

struct Recipe {
    std::string name;
    int prepTime = 0;
    std::string difficulty;
    std::string mainIngredient;
    std::string allergens;
};

#endif //GLAZE_PROJECT_2_RECIPE_H