#include <iostream>
#include <SFML/Graphics.hpp>
#include <random>

#include "Map.h"
#include "AStar.h"

void PrintMapConsole(const Map &map);

sf::Font font;

int main() {
    int width = 800;
    int height = 800;
    int menuHeight = 70;
    sf::RenderWindow window(sf::VideoMode(width, height + menuHeight), "A* Pathfinder",sf::Style::Titlebar | sf::Style::Close);
    window.setFramerateLimit(60);


    if (!font.loadFromFile("../Font/OpenSans-VariableFont_wdth,wght.ttf")) {
        std::cerr << "Error loading font\n";
    }

    int mapWidth = 40;
    int mapHeight = 40;
    Map map(mapWidth, mapHeight);

    map.setCellSize(width / mapWidth);

    int numObstacles = 100;
    map.generateObstacles(numObstacles);

    /*
    std::cout << "Mappa iniziale\n";
    PrintMapConsole(map);
    */

    int pathLength = 0;
    int numVisitedCells = 0;

    auto result = AStar::findPath(map, map.getStart(), map.getGoal());

    if (!result.first.empty()) {
        for (const auto& cell : result.second) {
            map.setCellState(cell.x, cell.y, CellState::Visited);
            numVisitedCells++;
        }

        for (const auto& cell : result.first) {
            map.setCellState(cell.x, cell.y, CellState::Path);
            pathLength++;
        }
    }
    else{
        std::cout << "No path found.\n";
    }

    bool dWasPressed = false;
    bool sWasPressed = false;
    bool gWasPressed = false;
    bool rWasPressed = false;
    bool bWasPressed = false;

    bool stateHasChanged = false;

    bool startSetMode = false;
    bool goalSetMode = false;

    bool setBorder = true;

    sf::Text text;
    text.setFont(font);
    text.setCharacterSize(16);
    text.setFillColor(sf::Color::Black);

    sf::Vector2i buttonSize(15, 15);

    sf::RectangleShape startModeButton(sf::Vector2f((float)buttonSize.x, (float)buttonSize.y));
    sf::Vector2i startModeButtonPosition(775, height + 7);
    startModeButton.setPosition((float) startModeButtonPosition.x, (float) startModeButtonPosition.y);

    sf::RectangleShape goalModeButton(sf::Vector2f((float)buttonSize.x, (float)buttonSize.y));
    sf::Vector2i goalModeButtonPosition(775, height + 30);
    goalModeButton.setPosition((float) goalModeButtonPosition.x, (float) goalModeButtonPosition.y);

    sf::RectangleShape hoverShapeCell(sf::Vector2f((float)map.getCellSize(), (float)map.getCellSize()));
    hoverShapeCell.setFillColor(sf::Color(0, 0, 0, 50));
    hoverShapeCell.setOutlineColor(sf::Color::Black);
    hoverShapeCell.setOutlineThickness(2);

    sf::RectangleShape hoverShapeButton(sf::Vector2f((float)buttonSize.x, (float)buttonSize.y));
    hoverShapeButton.setFillColor(sf::Color(0, 0, 0, 50));
    hoverShapeButton.setOutlineColor(sf::Color::Black);
    hoverShapeButton.setOutlineThickness(2);

    sf::RectangleShape borderButton(sf::Vector2f((float)buttonSize.x, (float)buttonSize.y));
    sf::Vector2i borderButtonPosition(775, height + 53);
    borderButton.setPosition((float)borderButtonPosition.x, (float)borderButtonPosition.y);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear(sf::Color::White);

        text.setString("Press 'R' to reset the map\nPress 'D' to toggle debug mode");
        text.setPosition(10, (float)height);
        window.draw(text);

        text.setString("Press 'Left Click' to set an obstacle\nPress 'Right Click' to set a walkable cell");
        text.setPosition(260, (float)height);
        window.draw(text);

        text.setString("Press 'S' to set start point\nPress 'G' to set goal point");
        text.setPosition(570, (float)height);
        window.draw(text);

        text.setStyle(sf::Text::Bold);
        text.setString("Path length: " + std::to_string(pathLength));
        text.setPosition(10, (float)height + 48);
        window.draw(text);
        text.setString("Visited cells: " + std::to_string(numVisitedCells));
        text.setPosition(180, (float)height + 48);
        window.draw(text);
        text.setStyle(sf::Text::Regular);

        text.setString("Press 'B' to toggle border");
        text.setPosition(570, (float)height + 48);
        window.draw(text);

        borderButton.setFillColor(setBorder ? sf::Color::Green : sf::Color::Red);
        window.draw(borderButton);


        startModeButton.setFillColor(startSetMode ? sf::Color::Green : sf::Color::Red);
        window.draw(startModeButton);

        goalModeButton.setFillColor(goalSetMode ? sf::Color::Green : sf::Color::Red);
        window.draw(goalModeButton);

        if(stateHasChanged){
            map.resetForRecalculation();
            /*
            std::cout << "Prima\n";
            PrintMapConsole(map);
            std::cout << "\n\n\n";
            */

            result = AStar::findPath(map, map.getStart(), map.getGoal());

            if (!result.first.empty()) {
                numVisitedCells = 0;
                for (const auto& cell : result.second) {
                    map.setCellState(cell.x, cell.y, CellState::Visited);
                    numVisitedCells++;
                }
                pathLength = 0;
                for (const auto& cell : result.first) {
                    map.setCellState(cell.x, cell.y, CellState::Path);
                    pathLength++;
                }
            }
            else{
                std::cout << "No path found.\n";
            }

            stateHasChanged = false;

            /*
            std::cout << "DOPO\n";
            PrintMapConsole(map);
            std::cout << "\n\n\n";
            */
        }

        // Gestione bottone per il bordo
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::B) ||
            (sf::Mouse::isButtonPressed(sf::Mouse::Left))
            && sf::Mouse::getPosition(window).x >= borderButtonPosition.x && sf::Mouse::getPosition(window).x <= borderButtonPosition.x + buttonSize.x
            && sf::Mouse::getPosition(window).y >= borderButtonPosition.y && sf::Mouse::getPosition(window).y <= borderButtonPosition.y + buttonSize.y) {
            if (!bWasPressed) {
                setBorder = !setBorder;
                bWasPressed = true;
            }
        } else {
            bWasPressed = false;
        }

        // Gestione della modalità di piazzamento del punto di partenza
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::S) ||
           (sf::Mouse::isButtonPressed(sf::Mouse::Left))
           && sf::Mouse::getPosition(window).x >= startModeButtonPosition.x && sf::Mouse::getPosition(window).x <= startModeButtonPosition.x + buttonSize.x
           && sf::Mouse::getPosition(window).y >= startModeButtonPosition.y && sf::Mouse::getPosition(window).y <= startModeButtonPosition.y + buttonSize.y) {
            if (!sWasPressed) {
                startSetMode = !startSetMode;
                goalSetMode = false;
                sWasPressed = true;
            }
        } else {
            sWasPressed = false;
        }

        // Gestione della modalità di piazzamento del punto di arrivo
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::G) ||
            (sf::Mouse::isButtonPressed(sf::Mouse::Left))
            && sf::Mouse::getPosition(window).x >= goalModeButtonPosition.x && sf::Mouse::getPosition(window).x <= goalModeButtonPosition.x + buttonSize.x
            && sf::Mouse::getPosition(window).y >= goalModeButtonPosition.y && sf::Mouse::getPosition(window).y <= goalModeButtonPosition.y + buttonSize.y) {
            if (!gWasPressed) {
                goalSetMode = !goalSetMode;
                startSetMode = false;
                gWasPressed = true;
            }
        } else {
            gWasPressed = false;
        }


        // Gestione del reset della mappa
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::R)) {
            if (!rWasPressed) {
                map.reset();
                map.generateObstacles(numObstacles);
                pathLength = 0;
                rWasPressed = true;
                stateHasChanged = true;
            }
        } else {
            rWasPressed = false;
        }

        // Gestione della modalità debug
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
            if (!dWasPressed) {
                map.toggleDebugMode();
                dWasPressed = true;
            }
        } else {
            dWasPressed = false;
        }

        // Gestione posizionamento delle celle
        // Si ottiene la posizione del mouse e si verifica se è all'interno della mappa
        // Se il tasto destro è premuto si piazza una cella walkable, altrimenti si piazza un ostacolo
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        mousePos.x /= (int)map.getCellSize();
        mousePos.y /= (int)map.getCellSize();
        if(mousePos.x >= 0 && mousePos.x < map.getWidth() && mousePos.y >= 0 && mousePos.y < map.getHeight()){
            if(sf::Mouse::isButtonPressed(sf::Mouse::Right)){
                if(map.canPlaceWalkable(mousePos.x, mousePos.y)){
                    //std::cout << "Cella (" << mousePos.x << ", " << mousePos.y << ") walkable\n";
                    map.setCellState(mousePos.x, mousePos.y, CellState::Walkable);
                    stateHasChanged = true;
                }
            }
            else if(sf::Mouse::isButtonPressed(sf::Mouse::Left)){
                // Si posiziona start/goal con il tasto sinistro. Si verifica se si è in modalità, altrimenti si piazza un ostacolo
                if(startSetMode){
                    //std::cout << "Cella (" << mousePos.x << ", " << mousePos.y << ") start\n";
                    map.setCellState(mousePos.x, mousePos.y, CellState::Start);
                    map.setStart(mousePos);
                    stateHasChanged = true;
                }
                else if(goalSetMode){
                    //std::cout << "Cella (" << mousePos.x << ", " << mousePos.y << ") goal\n";
                    map.setCellState(mousePos.x, mousePos.y, CellState::Goal);
                    map.setGoal(mousePos);
                    stateHasChanged = true;
                }
                else if(map.canPlaceObstacle(mousePos.x, mousePos.y)){
                    //std::cout << "Cella (" << mousePos.x << ", " << mousePos.y << ") ostacolo\n";
                    map.setCellState(mousePos.x, mousePos.y, CellState::Obstacle);
                    stateHasChanged = true;
                }
            }
        }

        map.draw(window, font, setBorder);

        // Gestione dell'hover sulle celle
        if(mousePos.x >= 0 && mousePos.x < map.getWidth() && mousePos.y >= 0 && mousePos.y < map.getHeight()){
            hoverShapeCell.setPosition((float) mousePos.x * map.getCellSize(), (float) mousePos.y * map.getCellSize());
        }
        else{
            hoverShapeCell.setPosition(-100, -100);
        }

        // Gestione dell'hover sui bottoni
        if(sf::Mouse::getPosition(window).x >= goalModeButtonPosition.x && sf::Mouse::getPosition(window).x <= goalModeButtonPosition.x + buttonSize.x
           && sf::Mouse::getPosition(window).y >= goalModeButtonPosition.y && sf::Mouse::getPosition(window).y <= goalModeButtonPosition.y + buttonSize.y)
        {
            hoverShapeButton.setPosition(sf::Vector2f((float) goalModeButtonPosition.x, (float) goalModeButtonPosition.y));
        }
        else if(sf::Mouse::getPosition(window).x >= startModeButtonPosition.x && sf::Mouse::getPosition(window).x <= startModeButtonPosition.x + buttonSize.x
                && sf::Mouse::getPosition(window).y >= startModeButtonPosition.y && sf::Mouse::getPosition(window).y <= startModeButtonPosition.y + buttonSize.y)
        {
            hoverShapeButton.setPosition(sf::Vector2f((float) startModeButtonPosition.x, (float) startModeButtonPosition.y));
        }
        else if(sf::Mouse::getPosition(window).x >= borderButtonPosition.x && sf::Mouse::getPosition(window).x <= borderButtonPosition.x + buttonSize.x
                && sf::Mouse::getPosition(window).y >= borderButtonPosition.y && sf::Mouse::getPosition(window).y <= borderButtonPosition.y + buttonSize.y)
        {
            hoverShapeButton.setPosition(sf::Vector2f((float) borderButtonPosition.x, (float) borderButtonPosition.y));
        }
        else{
            hoverShapeButton.setPosition(-100, -100);
        }

        window.draw(hoverShapeCell);
        window.draw(hoverShapeButton);

        window.display();
    }

    return 0;
}

void PrintMapConsole(const Map &map) {
    for (int i = 0; i < map.getWidth(); i++) {
        for (int j = 0; j < map.getHeight(); j++) {
            CellState state = map.getCellState(i, j);
            std::cout << static_cast<int>(state) << " ";
        }
        std::cout << "\n";
    }
}
