//
// Created by lawre on 3/2/2026.
//
#include <iostream>
#include <optional>
#include <chrono>
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
    sf::Color backgroundColor(245, 228, 218);
    sf::Color titleColor(254, 136, 150);
    sf::Color subtitleColor(254, 147, 152);

    sf::FloatRect textRect;

    sf::RenderWindow window(sf::VideoMode({1200, 800}), "Dessert Search");
    window.clear(backgroundColor);

    //load window icon
    sf::Image icon;
    icon.loadFromFile("resources/images/taskbar_icon.png");
    window.setIcon(icon.getSize(), icon.getPixelsPtr());

    //load music!!!  French Accordion Waltz Paris Atmosphere by Denis Pavlov Music
    //https://pixabay.com/music/france-french-accordion-waltz-paris-atmosphere-477503/
    sf::Music music("resources/audio/Music.mp3");
    music.setLooping(true);
    //music.play();

    //music mute button
    sf::Texture unmuted;
    unmuted.loadFromFile("resources/images/unmute.png");

    sf::Texture muted;
    muted.loadFromFile("resources/images/mute.png");

    sf::Sprite toggleMusic(unmuted);
    toggleMusic.setScale(sf::Vector2f(0.2, 0.2));
    toggleMusic.setPosition({0.f, 750.f});

    //load fonts
    const sf::Font title("resources/fonts/ButterGarlic.ttf");
    const sf::Font subtitle("resources/fonts/Million Dreams.otf");
    const sf::Font text("resources/fonts/Martel-Light.ttf");

    sf::Text text1(title);
    text1.setString("Welcome to Dessert Searcher!");
    text1.setCharacterSize(60);
    text1.setFillColor(titleColor);
    textRect = text1.getLocalBounds();
    text1.setOrigin({textRect.getCenter()});
    text1.setPosition({600, 75});

    sf::Text text2(subtitle);
    text2.setString("Search for a Dessert Below");
    text2.setCharacterSize(40);
    text2.setFillColor(subtitleColor);
    textRect = text2.getLocalBounds();
    text2.setOrigin({textRect.getCenter()});
    text2.setPosition({600, 140});






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

        window.clear(backgroundColor);

        window.draw(toggleMusic);
        window.draw(text1);
        window.draw(text2);

        window.display();
    }

    return 0;
}

