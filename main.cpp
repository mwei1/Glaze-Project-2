//
// Created by lawre on 3/2/2026.
//
#include <iostream>
#include <optional>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "CSVReader.h"
#include <vector>
#include "Trie.h"

using namespace std;

void musicSwitch(sf::Music& music, sf::Sprite& spr, sf::Texture& mute, sf::Texture& unmute)
{
    if (music.getStatus() == sf::Sound::Status::Playing)
    {
        music.pause();
        spr.setTexture(mute);
    }
    else
    {
        music.play();
        spr.setTexture(unmute);
    }
}

int main() {

    sf::RenderWindow window(sf::VideoMode({1200, 800}), "Dessert Search");
    window.clear(sf::Color::White);

    //load window icon
    sf::Image icon;
    icon.loadFromFile("resources/images/taskbar_icon.png");
    window.setIcon(icon.getSize(), icon.getPixelsPtr());

    //load music!!!  French Accordion Waltz Paris Atmosphere by Denis Pavlov Music
    //https://pixabay.com/music/france-french-accordion-waltz-paris-atmosphere-477503/
    sf::Music music("resources/audio/Music.mp3");
    music.setLooping(true);
    music.play();

    //buttons
    //sf::Sprite toggleDataStructure;

    sf::Texture unmuted;
    unmuted.loadFromFile("resources/images/unmute.png");

    sf::Texture muted;
    muted.loadFromFile("resources/images/mute.png");

    sf::Sprite toggleMusic(unmuted);
    toggleMusic.setScale(sf::Vector2f(0.2, 0.2));
    toggleMusic.setPosition({0.f, 750.f});


    vector<Recipe> recipes = CSVReader::loadRecipesFromFile("resources/recipes.csv");
    cout << recipes.size() << "\n\n";

    Trie t;
    for (int i=0;i<recipes.size();i++) {
        t.insert(recipes[i].name, i);
    }


    /*cout << "\n\n";
    string testSearchName = "Balsamic Bombe";
    vector<int> testFoundIndex = t.search(testSearchName);
    if (!testFoundIndex.empty()) {
        for (int index : testFoundIndex) {
            std::cout << recipes[index].name << " " << recipes[index].prepTime << " " << recipes[index].difficulty << " " << recipes[index].mainIngredient << " " << recipes[index].allergens << "\n";
        }
    }else {
        std::cout << "Recipe not found.";
    }

    cout << endl;

    string testPrefix = "Bal";
    vector<int> testFoundPrefix = t.searchByPrefix(testPrefix);
    if (!testFoundPrefix.empty()) {
        for (int index : testFoundPrefix) {
            cout << recipes[index].name << " " << recipes[index].prepTime << " " << recipes[index].difficulty << " " << recipes[index].mainIngredient << " " << recipes[index].allergens << "\n";
        }
    }else {
        cout << "No recipes found associated with that index.";
    }*/


    while (window.isOpen())
    {
        sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

        if (toggleMusic.getGlobalBounds().contains(mousePos))
            toggleMusic.setScale(sf::Vector2f(0.18, 0.18));
        else
            toggleMusic.setScale(sf::Vector2f(0.2, 0.2));

        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();

            if (event->is<sf::Event::MouseButtonReleased>())
            {
                if (toggleMusic.getGlobalBounds().contains(mousePos))
                    musicSwitch(music, toggleMusic, muted, unmuted);
            }
        }

        window.clear(sf::Color::White);
        window.draw(toggleMusic);
        window.display();
    }

    return 0;
}

