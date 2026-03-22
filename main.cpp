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
void structureSwitch(const bool usingTrie, sf::Text& top, sf::Text& bot, sf::FloatRect sizeRect)
{
    if (usingTrie)
    {
        top.setString("Using Trie");
        sizeRect = top.getLocalBounds();
        top.setOrigin(sizeRect.getCenter());
        top.setPosition({1050,108});

        bot.setString("Switch to\nHash Map");
        sizeRect = bot.getLocalBounds();
        bot.setOrigin(sizeRect.getCenter());
        bot.setPosition({1050, 150});
    }
    else
    {
        top.setString("Using Hash Map");
        sizeRect = top.getLocalBounds();
        top.setOrigin(sizeRect.getCenter());
        top.setPosition({1050,108});

        bot.setString("Switch to\n     Trie");
        sizeRect = bot.getLocalBounds();
        bot.setOrigin(sizeRect.getCenter());
        bot.setPosition({1050, 150});
    }
}

//toggles checkbox for trie prefix search
void checkboxSwitch(const bool prefixSearch, sf::Sprite& spr, sf::Texture& checked, sf::Texture& unchecked)
{
    if (prefixSearch)
        spr.setTexture(checked);
    else
        spr.setTexture(unchecked);
}

//returns vector of Recipe objects from vector of recipe indices
vector<Recipe> indicesToRecipes(vector<Recipe>& recipes, vector<int>& indices)
{
    vector<Recipe> result;
    for (int i: indices)
        result.push_back(recipes[i]);
    return result;
}
//returns vector of Recipe object from singular index
vector<Recipe> indexToRecipe(vector<Recipe>& recipes, int index)
{
    return {recipes[index]};
}

//searches Trie for given query using either prefix or fulltext search
//returns time in nanoseconds of search
int searchTrie(Trie& t, const bool prefixSearch, const string query, vector<Recipe>& result, vector<Recipe>& recipes)
{
    auto t1 = chrono::steady_clock::now();
    vector<int> indices;
    if (prefixSearch)
        indices = t.searchByPrefix(query);
    else
        indices = t.search(query);
    auto t2 = chrono::steady_clock::now();
    auto duration = chrono::duration_cast<chrono::nanoseconds>(t2 - t1).count();
    result.clear();
    result = indicesToRecipes(recipes, indices);
    return duration;
}
//fills text and rectangles vector with COLORLESS text and rectangles corresponding to recipes
void recipesToText(vector<Recipe>& recipes, vector<sf::Text>& text, vector<sf::RectangleShape>& rectangles, const sf::Font& font)
{
    text.clear();
    rectangles.clear();
    for (int i = 0; i < recipes.size(); i++)
    {
        sf::RectangleShape tempRect({580, 35});
        tempRect.setOutlineThickness(1);
        tempRect.setOrigin({290, 17.5});
        tempRect.setPosition({600, 241.f + 35.f * i});
        rectangles.push_back(tempRect);

        sf::Text tempText(font, recipes[i].name, 25);
        sf::FloatRect temp = tempText.getLocalBounds();
        if (temp.size.x > 550.f)
        {
            float scaleFactor = 550.f/temp.size.x;
            tempText.setScale({scaleFactor, 1.0f});
        }
        tempText.setOrigin(temp.getCenter());
        tempText.setPosition(tempRect.getPosition());
        text.push_back(tempText);
    }
}

//splits given input into at most four strings to roughly fit within box of width 244
//returns vector of strings
vector<string> splitString(string input, sf::Font font)
{
    vector<string> result;
    vector<string> words;

    //separate words
    auto pos = input.find(' ');
    while (pos != string::npos)
    {
        words.push_back(input.substr(0, pos));
        input.erase(0, pos + 1);
        pos = input.find(' ');
    }
    words.push_back(input);

    //start with first word
    string str = words[0];
    sf::Text text(font, str, 28);

    //add words on
    for (int i = 1; i < words.size(); i++)
    {
        string temp = str + " " + words[i];
        text.setString(temp);

        //if added length gets too long, push what was before adding
        if (text.getLocalBounds().size.x > 250.0f)
        {
            result.push_back(str);
            str = words[i];
        }
        //if added length not too long, continue to next word
        else
            str = temp;
    }
    result.push_back(str);
    return result;

    sf::FloatRect sizeRect = text.getLocalBounds();
    string temp;
    while (sizeRect.size.x > 244.0f)
    {
        temp = str[str.size() - 1] + temp;
        temp.pop_back();
        text.setString(str);
        sizeRect = text.getLocalBounds();
    }
    auto index = temp.find(' ');

    result.push_back(str);
}

int main() {
    //random number generator
    random_device rd;
    mt19937 rng(rd());
    uniform_real_distribution<float> unif(0.0, 1.0);
    uniform_int_distribution<int> random_index(0, 99999);

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
    music.play();

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

    //data structure in use
    sf::Text structureText1(text, "Using Trie", 20);
    structureText1.setFillColor(accent);
    sizeRect = structureText1.getLocalBounds();
    structureText1.setOrigin(sizeRect.getCenter());
    structureText1.setPosition({1050,108});

    //data structure toggle button
    sf::RectangleShape structureButton ({130.f, 56.f});
    structureButton.setFillColor(accent);
    structureButton.setOutlineThickness(2);
    structureButton.setOutlineColor(subtitleColor);
    structureButton.setOrigin({65, 28});
    structureButton.setPosition({1050, 150});

    //data structure toggle button label
    sf::Text structureText2(text, "Switch to\nHash Map", 20);
    structureText2.setFillColor(backgroundColor);
    structureText2.setLineSpacing(0.6f);
    sizeRect = structureText2.getLocalBounds();
    structureText2.setOrigin(sizeRect.getCenter());
    structureText2.setPosition({1050, 150});

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
    bool searchBoxHighlight = false; //used for search box highlight
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
    sf::RectangleShape resultsBar({580, 35});
    resultsBar.setFillColor(backgroundColor);
    resultsBar.setOutlineThickness(1);
    resultsBar.setOutlineColor(accent);
    resultsBar.setOrigin({290, 17.5});
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

    //performance indicators box
    sf::RectangleShape performanceBox({180.f, 120.0f});
    performanceBox.setOutlineThickness(2);
    performanceBox.setFillColor(backgroundColor);
    performanceBox.setOutlineColor(titleColor);
    performanceBox.setOrigin({90, 0});
    performanceBox.setPosition({1050, 185});

    //trie time text
    sf::Text triePerformance(text, "Last Trie Time:\n N/A ns", 23);
    triePerformance.setFillColor(accent);
    triePerformance.setLineSpacing(0.7);
    sizeRect = triePerformance.getLocalBounds();
    triePerformance.setOrigin({sizeRect.getCenter().x, 0});
    triePerformance.setPosition({1050, 187});

    //hash map time text
    sf::Text mapPerformance(text, "Last Map Time:\n N/A ns", 23);
    mapPerformance.setFillColor(accent);
    mapPerformance.setLineSpacing(0.7);
    sizeRect = mapPerformance.getLocalBounds();
    mapPerformance.setOrigin({sizeRect.getCenter().x, 0});
    mapPerformance.setPosition({1050, 245});

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
    checkbox.setPosition({1105, 60});

    //prefix search toggle text
    sf::Text prefix(text, "Toggle Trie\nPrefix Search", 15);
    prefix.setFillColor(accent);
    prefix.setLineSpacing(0.7);
    prefix.setStyle(sf::Text::Bold);
    sizeRect = prefix.getLocalBounds();
    prefix.setOrigin(sizeRect.getCenter());
    prefix.setPosition({1035, 65});

    //recipe box
    sf::RectangleShape recipeBox({250, 310});
    recipeBox.setOutlineThickness(2);
    recipeBox.setFillColor(backgroundColor);
    recipeBox.setOutlineColor(accent);
    recipeBox.setOrigin({125, 0});
    recipeBox.setPosition({1050, 390});

    //recipe box label text
    sf::Text recipeDetails(subtitle, "Recipe Info", 30);
    recipeDetails.setFillColor(accent);
    sizeRect = recipeDetails.getLocalBounds();
    recipeDetails.setOrigin(sizeRect.getCenter());
    recipeDetails.setPosition({1050, 375});

    //filler name in recipe box
    sf::Text recipeDefault(text, "Choose a Recipe", 30);
    recipeDefault.setStyle(sf::Text::Bold);
    recipeDefault.setStyle(sf::Text::Underlined);
    recipeDefault.setFillColor(accent);
    sizeRect = recipeDefault.getLocalBounds();
    recipeDefault.setOrigin({sizeRect.getCenter().x, 0});
    recipeDefault.setPosition({1050, 435});

    //difficulty header
    sf::Text diffHead(subtitle, "Difficulty", 23);
    diffHead.setStyle(sf::Text::Underlined);
    diffHead.setFillColor(accent);
    sizeRect = diffHead.getLocalBounds();
    diffHead.setOrigin({sizeRect.getCenter().x, 0});
    diffHead.setPosition({980, 510});

    //prep time header
    sf::Text timeHead(subtitle, "Prep Time", 23);
    timeHead.setStyle(sf::Text::Underlined);
    timeHead.setFillColor(accent);
    sizeRect = timeHead.getLocalBounds();
    timeHead.setOrigin({sizeRect.getCenter().x, 0});
    timeHead.setPosition({1110, 510});

    //main ingredient header
    sf::Text mainHead(subtitle, "Main Ingredient", 25);
    mainHead.setFillColor(accent);
    mainHead.setStyle(sf::Text::Underlined);
    sizeRect = mainHead.getLocalBounds();
    mainHead.setOrigin({sizeRect.getCenter().x, 0});
    mainHead.setPosition({1050, 570});

    //allergens header
    sf::Text allHead(subtitle, "Allergens: ", 25);
    allHead.setStyle(sf::Text::Underlined);
    allHead.setFillColor(accent);
    sizeRect = allHead.getLocalBounds();
    allHead.setOrigin({sizeRect.getCenter().x, 0});
    allHead.setPosition({1050, 635});

    //recipe box text preformatting
    sf::Text name(text, "", 28);
    name.setFillColor(accent);
    sf::Text diff(text, "", 23);
    diff.setFillColor(accent);
    sf::Text time(text, "", 23);
    time.setFillColor(accent);
    sf::Text main(text, "", 25);
    main.setFillColor(accent);
    sf::Text all(text, "", 25);
    all.setFillColor(accent);

    //Group Name!
    sf::Text group(text, "Team Glaze: Erica Lawrence, Zlata Kovrigina, Michael Wei", 20);
    group.setFillColor(accent);
    sizeRect = group.getLocalBounds();
    group.setOrigin({sizeRect.getCenter().x, sizeRect.size.y});
    group.setPosition({600, 795});


    //data structures implementation / logic components
    //int times in nanoseconds
    int trieTime = 0;
    int mapTime = 0;

    bool usingTrie = true;
    bool prefixSearch = false;

    Recipe currentRecipe;
    currentRecipe.prepTime = -1;
    vector<Recipe> currentResults;
    vector<sf::Text> resultText;
    vector<sf::RectangleShape> resultRect;

    //load recipes from csv
    vector<Recipe> recipes = CSVReader::loadRecipesFromFile("resources/recipes.csv");
    cout << recipes.size() << "\n\n";

    Trie t;
    for (int i=0;i<recipes.size();i++) {
        t.insert(recipes[i].name, i);
    }

    //display loop
    while (window.isOpen())
    {
        sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

        //music button shrinkage
        if (toggleMusic.getGlobalBounds().contains(mousePos))
            toggleMusic.setScale(sf::Vector2f(0.18, 0.18));
        else
            toggleMusic.setScale(sf::Vector2f(0.2, 0.2));

        //structure switch button shrinkage
        if (structureButton.getGlobalBounds().contains(mousePos))
            structureButton.setScale(sf::Vector2f(0.9, 0.9));
        else
            structureButton.setScale(sf::Vector2f(1, 1));

        //search icon shrinkage
        if (searchIcon.getGlobalBounds().contains(mousePos))
            searchIcon.setScale(sf::Vector2f(0.06, 0.06));
        else
            searchIcon.setScale(sf::Vector2f(.065, .065));

        //checkbox shrinkage
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

        //hovering over results shrinkage
        for (int i = 0; i < min(static_cast<int>(resultText.size()), 15); i++)
        {
            if (resultRect[i].getGlobalBounds().contains(mousePos))
            {
                resultRect[i].setScale(sf::Vector2f(0.96, 0.96));
                resultText[i].setScale(sf::Vector2f(0.95, 0.95));
            }
            else
            {
                resultRect[i].setScale(sf::Vector2f(1, 1));
                resultText[i].setScale(sf::Vector2f(1, 1));
            }
        }

        //event logic
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();

            //mouse press
            if (event->is<sf::Event::MouseButtonReleased>())
            {
                searchBoxHighlight = false;

                //toggle music
                if (toggleMusic.getGlobalBounds().contains(mousePos))
                    musicSwitch(music, toggleMusic, muted, unmuted);

                //execute random search upon clicking donut (displays too)
                if (quirkyDonut.getGlobalBounds().contains(mousePos))
                {
                    int index = random_index(rng);
                    currentResults = indexToRecipe(recipes, index);
                    recipesToText(currentResults, resultText, resultRect, text);
                    currentRecipe = currentResults[0];
                    query = currentResults[0].name;
                    searchBar.setString(query);
                    sizeRect = searchBar.getLocalBounds();
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
                    searchBoxHighlight = true;

                //search icon also searches
                if (searchIcon.getGlobalBounds().contains(mousePos))
                {
                    searchBoxHighlight = true;
                    if (query.empty())
                        continue;
                    if (usingTrie)
                    {
                        trieTime = searchTrie(t, prefixSearch, query, currentResults, recipes);
                        recipesToText(currentResults, resultText, resultRect, text);
                    }
                    else
                    {
                        //search with Hash Map
                    }
                }

                //display clicked recipe
                for (int i = 0; i < min(static_cast<int>(resultText.size()), 15); i++)
                {
                    if (resultRect[i].getGlobalBounds().contains(mousePos))
                    {
                        currentRecipe = currentResults[i];
                    }
                }
            }

            //treat any typing as search
            //stores as unicode, using https://symbl.cc/en/unicode-table/ as reference
            if (auto* textEvent = event->getIf<sf::Event::TextEntered>())
            {
                searchBoxHighlight = true;

                //backspace clears last character
                if (textEvent->unicode == 8)
                {
                    if (!query.empty())
                        query.pop_back();
                }

                //enter runs search
                else if (textEvent->unicode == 13 || textEvent->unicode == 10)
                {
                    if (query.empty())
                        continue;
                    if (usingTrie)
                    {
                        trieTime = searchTrie(t, prefixSearch, query, currentResults, recipes);
                        recipesToText(currentResults, resultText, resultRect, text);
                    }
                    else
                    {
                        //search with Hash Map
                    }
                }
                //valid typable characters (no special characters / arrow keys)
                else if (textEvent->unicode >= 32 && textEvent->unicode <= 128)
                    query+=textEvent->unicode;

                //display current search query
                searchBar.setString(query);
                sizeRect = searchBar.getLocalBounds();
                //cut off front part of oversized strings like google search bar
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

                //if using Trie with prefix Search, search current query (without enter)
                if (usingTrie && prefixSearch && !query.empty())
                {
                    trieTime = searchTrie(t, prefixSearch, query, currentResults, recipes);
                    recipesToText(currentResults, resultText, resultRect, text);
                }
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

        //default results bar only shows if nothing is being searched
        if (query.empty())
        {
            window.draw(resultsBar);
            window.draw(defaultResult);
            resultText.clear();
            resultRect.clear();
        }
        //if results exist, display them
        if (!resultText.empty())
        {
            for (int i = 0; i < min(static_cast<int>(resultText.size()), 15); i++)
            {
                resultText[i].setFillColor(accent);
                sizeRect = resultText[i].getLocalBounds();
                while (sizeRect.size.x > 575.0f)
                {
                    resultText[i].setLetterSpacing(resultText[i].getLetterSpacing() - 0.01);
                    sizeRect = resultText[i].getLocalBounds();
                }
                resultText[i].setOrigin(sizeRect.getCenter());
                resultRect[i].setFillColor(backgroundColor);
                resultRect[i].setOutlineColor(accent);
                window.draw(resultRect[i]);
                window.draw(resultText[i]);
            }
        }

        //search bar display
        if (searchBoxHighlight)
            bar.setOutlineThickness(3.f);
        else
            bar.setOutlineThickness(1.f);
        window.draw(bar);
        window.draw(searchBar);
        window.draw(searchIcon);

        //performance indicators
        window.draw(performanceBox);
        triePerformance.setString("Last Trie Time:\n" + to_string(trieTime) + " ns");
        window.draw(triePerformance);
        mapPerformance.setString("Last Map Time:\n" + to_string(mapTime) + " ns");
        window.draw(mapPerformance);

        //recipe box and headers
        window.draw(recipeBox);
        window.draw(recipeDetails);
        window.draw(diffHead);
        window.draw(mainHead);
        window.draw(timeHead);
        window.draw(allHead);
        //display recipe info if prepTime is nonnegative (preset to -1)
        if (currentRecipe.prepTime >= 0)
        {
            vector<string> temp = splitString(currentRecipe.name, text);
            for (int i = 0; i < temp.size(); i++)
            {
                sf::Text name(text, "", 28);
                name.setFillColor(accent);
                name.setString(temp[i]);
                sizeRect = name.getLocalBounds();
                if (sizeRect.size.x > 244.0f)
                    name.setScale({244.0f/sizeRect.size.x, 1.0f});
                else
                    name.setScale({1.0f, 1.0f});
                name.setOrigin({sizeRect.getCenter().x, 0});
                name.setPosition({1050, 450.0f - 15.0f * temp.size() + 30.0f * i});
                window.draw(name);
            }

            diff.setString(currentRecipe.difficulty);
            sizeRect = diff.getLocalBounds();
            diff.setOrigin({sizeRect.getCenter().x, 0});
            diff.setPosition({980, 540});

            main.setString(currentRecipe.mainIngredient);
            sizeRect = main.getLocalBounds();
            main.setOrigin({sizeRect.getCenter().x, 0});
            main.setPosition({1050, 600});

            time.setString(to_string(currentRecipe.prepTime));
            sizeRect = time.getLocalBounds();
            time.setOrigin({sizeRect.getCenter().x, 0});
            time.setPosition({1110, 540});

            all.setString(currentRecipe.allergens);
            sizeRect = all.getLocalBounds();
            all.setOrigin({sizeRect.getCenter().x, 0});
            if (sizeRect.size.x > 244.0f)
                all.setScale(sf::Vector2f{244.0f/sizeRect.size.x, 1.0f});
            else
                all.setScale(sf::Vector2f{1.0, 1.0});
            all.setPosition({1050, 665});

            window.draw(name);
            window.draw(diff);
            window.draw(main);
            window.draw(time);
            window.draw(all);
        }
        else
        {
            window.draw(recipeDefault);
        }

        //fun things
        window.draw(quirkyDonut);
        window.draw(group);

        window.display();
    }

    return 0;
}

