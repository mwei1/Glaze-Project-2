//
// Created by lawre on 3/2/2026.
//
#include <iostream>
#include <optional>
#include <chrono>
#include <random>
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

//toggles text for structure switching
void structureSwitch(bool usingTrie, sf::Text& top, sf::Text& bot, sf::FloatRect sizeRect)
{
    if (usingTrie)
    {
        top.setString("Using Trie");
        sizeRect = top.getLocalBounds();
        top.setOrigin(sizeRect.getCenter());
        top.setPosition({1090,108});

        bot.setString("Switch to\nHash Map");
        sizeRect = bot.getLocalBounds();
        bot.setOrigin(sizeRect.getCenter());
        bot.setPosition({1090, 150});
    }
    else
    {
        top.setString("Using Hash Map");
        sizeRect = top.getLocalBounds();
        top.setOrigin(sizeRect.getCenter());
        top.setPosition({1090,108});

        bot.setString("Switch to\n     Trie");
        sizeRect = bot.getLocalBounds();
        bot.setOrigin(sizeRect.getCenter());
        bot.setPosition({1090, 150});
    }
}

//toggles checkbox for trie prefix search
void checkboxSwitch(bool prefixSearch, sf::Sprite& spr, sf::Texture& checked, sf::Texture& unchecked)
{
    if (prefixSearch)
        spr.setTexture(checked);
    else
        spr.setTexture(unchecked);
}

int main() {
    //random number generator
    random_device rd;
    mt19937 rng(rd());
    uniform_real_distribution<float> unif(0.0, 1.0);
    uniform_int_distribution<int> random_index(0, 999999);

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
    structureText1.setPosition({1090,108});

    sf::RectangleShape structureButton ({130.f, 56.f});
    structureButton.setFillColor(accent);
    structureButton.setOutlineThickness(2);
    structureButton.setOutlineColor(subtitleColor);
    structureButton.setOrigin({65, 28});
    structureButton.setPosition({1090, 150});

    sf::Text structureText2(text, "Switch to\nHash Map", 20);
    structureText2.setFillColor(backgroundColor);
    structureText2.setLineSpacing(0.6f);
    sizeRect = structureText2.getLocalBounds();
    structureText2.setOrigin(sizeRect.getCenter());
    structureText2.setPosition({1090, 150});

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
    searchBar.setOrigin({0, 18});
    searchBar.setPosition({600, 200});

    //search bar outline
    sf::RectangleShape bar({600.f, 42.f});
    bar.setOrigin({300.f, 21.f});
    bar.setPosition({600, 200});
    bar.setOutlineColor(accent);
    bar.setOutlineThickness(1.f);

    //result bar outline
    sf::RectangleShape resultsBar({560, 35});
    resultsBar.setFillColor(backgroundColor);
    resultsBar.setOutlineThickness(1);
    resultsBar.setOutlineColor(accent);
    resultsBar.setOrigin({280, 17.5});
    resultsBar.setPosition({600, 240});

    //result bar default text
    sf::Text defaultResult(text, "No Results Yet! Try Searching Something!", 25);
    defaultResult.setFillColor(accent);
    sizeRect = defaultResult.getLocalBounds();
    defaultResult.setOrigin({sizeRect.getCenter()});
    defaultResult.setPosition({resultsBar.getPosition().x, resultsBar.getPosition().y + 2});

    //search button
    //https://www.flaticon.com/free-icon/search-interface-symbol_54481?term=search&page=1&position=2&origin=search&related_id=54481
    sf::Texture search;
    search.loadFromFile("resources/images/search.png");
    sf::Sprite searchIcon(search);
    searchIcon.setScale(sf::Vector2f(0.065, 0.065));
    sizeRect = searchIcon.getLocalBounds();
    searchIcon.setOrigin(sizeRect.getCenter());
    searchIcon.setPosition({880, 200});

    //performance indicators
    float trieTime = 0.0;
    float mapTime = 0.0;

    sf::RectangleShape performanceBox({180.f, 120.0f});
    performanceBox.setOutlineThickness(2);
    performanceBox.setFillColor(backgroundColor);
    performanceBox.setOutlineColor(titleColor);
    performanceBox.setOrigin({90, 0});
    performanceBox.setPosition({1090, 185});

    sf::Text triePerformance(text, "Last Trie Time:\n   " + to_string(trieTime), 23);
    triePerformance.setFillColor(accent);
    triePerformance.setLineSpacing(0.7);
    sizeRect = triePerformance.getLocalBounds();
    triePerformance.setOrigin({sizeRect.getCenter().x, 0});
    triePerformance.setPosition({1090, 187});

    sf::Text mapPerformance(text, "Last Map Time:\n   " + to_string(mapTime), 23);
    mapPerformance.setFillColor(accent);
    mapPerformance.setLineSpacing(0.7);
    sizeRect = mapPerformance.getLocalBounds();
    mapPerformance.setOrigin({sizeRect.getCenter().x, 0});
    mapPerformance.setPosition({1090, 245});

    //toggle checkbox for trie prefix search
    //https://www.flaticon.com/free-icon/unchecked_8924271
    sf::Texture unchecked;
    unchecked.loadFromFile("resources/images/unchecked.png");
    //https://www.flaticon.com/free-icon/checkbox_2618312
    sf::Texture checked;
    checked.loadFromFile("resources/images/checked.png");

    sf::Sprite checkbox(unchecked);
    checkbox.setScale(sf::Vector2f(0.2, 0.2));
    sizeRect = checkbox.getLocalBounds();
    checkbox.setOrigin(sizeRect.getCenter());
    checkbox.setPosition({1145, 65});

    //prefix search toggle text
    sf::Text prefix(text, "Toggle Trie\nPrefix Search", 15);
    prefix.setFillColor(accent);
    prefix.setLineSpacing(0.7);
    prefix.setStyle(sf::Text::Bold);
    sizeRect = prefix.getLocalBounds();
    prefix.setOrigin(sizeRect.getCenter());
    prefix.setPosition({1070, 65});


    //Group Name!
    sf::Text name(text, "Team Glaze: Erica Lawrence, Zlata Kovrigina, Michael Wei", 20);
    name.setFillColor(accent);
    sizeRect = name.getLocalBounds();
    name.setOrigin({sizeRect.getCenter().x, sizeRect.size.y});
    name.setPosition({600, 795});



    bool usingTrie = true;
    bool prefixSearch = false;
    vector<Recipe> currentResults;

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

        if (searchIcon.getGlobalBounds().contains(mousePos))
            searchIcon.setScale(sf::Vector2f(0.06, 0.06));
        else
            searchIcon.setScale(sf::Vector2f(.065, .065));

        if (checkbox.getGlobalBounds().contains(mousePos))
            checkbox.setScale(sf::Vector2f(0.18, 0.18));
        else
            checkbox.setScale(sf::Vector2f(0.2, 0.2));

        //donut rotation
        if (quirkyDonut.getGlobalBounds().contains(mousePos))
        {
            quirkyDonut.setScale(sf::Vector2f(0.085, 0.085));
            quirkyDonut.rotate(sf::degrees(unif(rng) / 3.0f));
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

                //toggle trie prefix search
                if (checkbox.getGlobalBounds().contains(mousePos))
                {
                    prefixSearch = !prefixSearch;
                    checkboxSwitch(prefixSearch, checkbox, checked, unchecked);
                }

                //highlight search bar
                if (bar.getGlobalBounds().contains(mousePos))
                    searching = true;

                if (searchIcon.getGlobalBounds().contains(mousePos))
                {
                    //search
                }
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
                //cut off front part of oversized strings
                if (searchBar.getLocalBounds().size.x > 550)
                {
                    string temp = query;
                    while (searchBar.getLocalBounds().size.x > 550)
                    {
                        temp.erase(0, 1);
                        searchBar.setString(temp);
                    }
                }
                searchBar.setOrigin({0, 18});
                searchBar.setPosition({305, 200});
            }
        }

        window.clear(backgroundColor);

        //title + subtitle
        window.draw(titleText);
        window.draw(subtitleText);

        //buttons
        window.draw(toggleMusic);
        window.draw(checkbox);
        window.draw(prefix);

        //structure
        window.draw(structureButton);
        window.draw(structureText1);
        window.draw(structureText2);

        //search bar + results
        window.draw(resultsBar);
        window.draw(defaultResult);

        if (searching)
            bar.setOutlineThickness(3.f);
        else
            bar.setOutlineThickness(1.f);
        window.draw(bar);
        window.draw(searchBar);
        window.draw(searchIcon);

        //performance indicators
        window.draw(performanceBox);
        window.draw(triePerformance);
        window.draw(mapPerformance);

        //fun things
        window.draw(quirkyDonut);
        window.draw(name);

        window.display();
    }

    return 0;
}

