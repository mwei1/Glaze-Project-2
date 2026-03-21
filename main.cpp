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

//toggles music, pauses if currently playing, plays if currently stopped
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

//toggles buttons and text for structure switching
void structureSwitch(bool usingTrie, sf::Text& top, sf::Text& bot, sf::FloatRect sizeRect)
{
    if (usingTrie)
    {
        top.setString("Using Trie");
        sizeRect = top.getLocalBounds();
        top.setOrigin(sizeRect.getCenter());
        top.setPosition({1090,58});

        bot.setString("Switch to\nHash Map");
        sizeRect = bot.getLocalBounds();
        bot.setOrigin(sizeRect.getCenter());
        bot.setPosition({1090, 100});
    }
    else
    {
        top.setString("Using Hash Map");
        sizeRect = top.getLocalBounds();
        top.setOrigin(sizeRect.getCenter());
        top.setPosition({1090,58});

        bot.setString("Switch to\n     Trie");
        sizeRect = bot.getLocalBounds();
        bot.setOrigin(sizeRect.getCenter());
        bot.setPosition({1090, 100});
    }
}

int main() {
    //colors
    sf::Color backgroundColor(245, 228, 218);
    sf::Color titleColor(254, 136, 150);
    sf::Color subtitleColor(254, 147, 152);
    sf::Color accent(211, 101, 130);

    //load fonts
    //https://www.dafont.com/butter-garlic.font
    const sf::Font title("resources/fonts/ButterGarlic.ttf");
    //https://www.dafont.com/es/million-dreams.font
    const sf::Font subtitle("resources/fonts/Million Dreams.otf");
    //https://fonts.google.com/specimen/Martel
    const sf::Font text("resources/fonts/Martel-Light.ttf");


    sf::FloatRect sizeRect; //for text resizing/centering

    sf::RenderWindow window(sf::VideoMode({1200, 800}), "Dessert Search");
    window.clear(backgroundColor);

    //load window icon
    //https://png.pngtree.com/png-clipart/20190906/original/pngtree-cake-dessert-pixel-painting-illustration-png-image_4558418.jpg
    sf::Image icon;
    icon.loadFromFile("resources/images/taskbar_icon.png");
    window.setIcon(icon.getSize(), icon.getPixelsPtr());

    //load music!!!  French Accordion Waltz Paris Atmosphere by Denis Pavlov Music
    //https://pixabay.com/music/france-french-accordion-waltz-paris-atmosphere-477503/
    sf::Music music("resources/audio/Music.mp3");
    music.setLooping(true);
    //music.play();

    //music mute button
    //https://thenounproject.com/icon/loud-speaker-3452892/
    sf::Texture unmuted;
    unmuted.loadFromFile("resources/images/unmute.png");
    //https://thenounproject.com/icon/mute-1683888/
    sf::Texture muted;
    muted.loadFromFile("resources/images/mute.png");

    sf::Sprite toggleMusic(unmuted);
    toggleMusic.setScale(sf::Vector2f(0.2, 0.2));
    toggleMusic.setPosition({0.f, 750.f});

    //switch data structure button
    sf::Text structureText1(text, "Using Trie", 20);
    structureText1.setFillColor(accent);
    sizeRect = structureText1.getLocalBounds();
    structureText1.setOrigin(sizeRect.getCenter());
    structureText1.setPosition({1090,58});

    sf::RectangleShape structureButton ({130.f, 56.f});
    structureButton.setFillColor(accent);
    structureButton.setOutlineThickness(2);
    structureButton.setOutlineColor(subtitleColor);
    structureButton.setOrigin({65, 28});
    structureButton.setPosition({1090, 100});

    sf::Text structureText2(text, "Switch to\nHash Map", 20);
    structureText2.setFillColor(backgroundColor);
    structureText2.setLineSpacing(0.6f);
    sizeRect = structureText2.getLocalBounds();
    structureText2.setOrigin(sizeRect.getCenter());
    structureText2.setPosition({1090, 100});


    //beautiful donut
    //https://pngtree.com/freepng/funny-donut-with-colorful-icing-cartoon-eyes-expressive-face-hand-and-foot-on-transparent-background_23206177.html
    sf::Texture donut;
    donut.loadFromFile("resources/images/quirky_donut.png");
    sf::Sprite quirkyDonut(donut);
    quirkyDonut.setScale(sf::Vector2f(0.09, 0.09));
    sizeRect = quirkyDonut.getLocalBounds();
    quirkyDonut.setOrigin(sizeRect.getCenter());
    quirkyDonut.setPosition({0.f + quirkyDonut.getGlobalBounds().size.x / 2.0f, 130.f + quirkyDonut.getGlobalBounds().size.y / 2.0f});


    //title
    sf::Text titleText(title,"Welcome to Dessert Searcher!", 60);
    titleText.setFillColor(titleColor);
    titleText.setOutlineThickness(2);
    titleText.setOutlineColor(accent);
    sizeRect = titleText.getLocalBounds();
    titleText.setOrigin({sizeRect.getCenter()});
    titleText.setPosition({600, 75});

    //subtitle
    sf::Text subtitleText(subtitle, "Search for a Dessert Below", 40);
    subtitleText.setFillColor(subtitleColor);
    sizeRect = subtitleText.getLocalBounds();
    subtitleText.setOrigin({sizeRect.getCenter()});
    subtitleText.setPosition({600, 140});

    //search box text
    bool searching = false; //used for search box highlight
    string query = "";
    sf::Text searchBar(text, query, 30);
    searchBar.setFillColor(sf::Color::Black);
    sizeRect = searchBar.getLocalBounds();
    searchBar.setOrigin({sizeRect.getCenter().x-sizeRect.size.x/2.0f, 18});
    searchBar.setPosition({600, 200});

    //search bar outline
    sf::RectangleShape bar({600.f, 42.f});
    bar.setOrigin({300.f, 21.f});
    bar.setPosition({600, 200});
    bar.setOutlineColor(accent);
    bar.setOutlineThickness(1.f);

    bool usingTrie = true;

    //load recipes from csv
    vector<Recipe> recipes = CSVReader::loadRecipesFromFile("resources/recipes.csv");
    cout << recipes.size() << "\n\n";

    Trie t;
    for (int i=0;i<recipes.size();i++) {
        t.insert(recipes[i].name, i);
    }




    while (window.isOpen())
    {
        sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

        //button grow/shrink
        if (toggleMusic.getGlobalBounds().contains(mousePos))
            toggleMusic.setScale(sf::Vector2f(0.18, 0.18));
        else
            toggleMusic.setScale(sf::Vector2f(0.2, 0.2));

        if (structureButton.getGlobalBounds().contains(mousePos))
            structureButton.setScale(sf::Vector2f(0.9, 0.9));
        else
            structureButton.setScale(sf::Vector2f(1, 1));

        //donut rotation
        if (quirkyDonut.getGlobalBounds().contains(mousePos))
        {
            quirkyDonut.setScale(sf::Vector2f(0.085, 0.085));
            quirkyDonut.rotate(sf::degrees(0.1));
        }
        else
            quirkyDonut.setScale(sf::Vector2f(0.089, 0.089));


        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();

            //mouse press
            if (event->is<sf::Event::MouseButtonReleased>())
            {
                searching = false;

                //toggle music
                if (toggleMusic.getGlobalBounds().contains(mousePos))
                    musicSwitch(music, toggleMusic, muted, unmuted);

                //execute random search
                if (quirkyDonut.getGlobalBounds().contains(mousePos))
                    continue; // random search?

                //switch data structures
                if (structureButton.getGlobalBounds().contains(mousePos))
                {
                    usingTrie = !usingTrie;
                    structureSwitch(usingTrie, structureText1, structureText2, sizeRect);
                }

                //highlight search bar
                if (bar.getGlobalBounds().contains(mousePos))
                    searching = true;
            }

            //treat any typing as search
            //stores as unicode, using https://symbl.cc/en/unicode-table/ as reference
            if (auto* textEvent = event->getIf<sf::Event::TextEntered>())
            {
                searching = true;
                //backspace clears last character
                if (textEvent->unicode == 8)
                {
                    if (!query.empty())
                        query.pop_back();
                }

                //enter runs search
                else if (textEvent->unicode == 13 || textEvent->unicode == 10)
                {
                    continue;
                }
                else
                    query+=textEvent->unicode;
                searchBar.setString(query);
                sizeRect = searchBar.getLocalBounds();
                searchBar.setOrigin({sizeRect.getCenter().x-sizeRect.size.x/2.0f, 18});
                searchBar.setPosition({305, 200});


            }
        }

        window.clear(backgroundColor);

        window.draw(toggleMusic);
        window.draw(quirkyDonut);

        //title + subtitle
        window.draw(titleText);
        window.draw(subtitleText);

        //structure
        window.draw(structureButton);
        window.draw(structureText1);
        window.draw(structureText2);


        if (searching)
            bar.setOutlineThickness(3.f);
        else
            bar.setOutlineThickness(1.f);
        window.draw(bar);
        window.draw(searchBar);

        window.display();
    }

    return 0;
}

