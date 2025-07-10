#include <iostream>
#include <SFML/Graphics.hpp>
#include <random>

#include "Map.h"
#include "AStar.h"

void PrintMapConsole(const Map &map);

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 800), "A* Pathfinder");
    window.setFramerateLimit(60);
    int mapWidth = 10;
    int mapHeight = 10;
    Map map(mapWidth, mapHeight);

    map.setCellSize(window.getSize().x / mapWidth);

    int numObstacles = 10;
    map.generateObstacles(numObstacles);

    std::cout << "Mappa iniziale\n";
    PrintMapConsole(map);

    auto result = AStar::findPath(map, map.getStart(), map.getGoal());

    if (result.first.empty()) {
        std::cout << "No path found.\n";
    }

    for (const auto& cell : result.second) {
        map.setCellState(cell.x, cell.y, CellState::Visited);
    }

    for (const auto& cell : result.first) {
        map.setCellState(cell.x, cell.y, CellState::Path);
    }

    bool dWasPressed = false;

    bool stateHasChanged = false;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();

        if(stateHasChanged){
            map.resetForRecalculation();
            std::cout << "Prima\n";
            PrintMapConsole(map);
            std::cout << "\n\n\n";



            result = AStar::findPath(map, map.getStart(), map.getGoal());

            if (result.first.empty()) {
                std::cout << "No path found.\n";
            }

            for (const auto& cell : result.second) {
                map.setCellState(cell.x, cell.y, CellState::Visited);
            }

            for (const auto& cell : result.first) {
                map.setCellState(cell.x, cell.y, CellState::Path);
            }
            stateHasChanged = false;

            std::cout << "DOPO\n";
            PrintMapConsole(map);
            std::cout << "\n\n\n";
        }

        // Gestione del reset della mappa
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::R)) {
            map.reset();
            map.generateObstacles(numObstacles);

            stateHasChanged = true;
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


        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        mousePos.x /= (int)map.getCellSize();
        mousePos.y /= (int)map.getCellSize();
        if(mousePos.x >= 0 && mousePos.x < map.getWidth() && mousePos.y >= 0 && mousePos.y < map.getHeight()){
            if(sf::Mouse::isButtonPressed(sf::Mouse::Right)){
                if(map.getCellState(mousePos.x, mousePos.y) != CellState::Start && map.getCellState(mousePos.x, mousePos.y) != CellState::Goal){
                    //std::cout << "Cella (" << mousePos.x << ", " << mousePos.y << ") walkable\n";
                    map.setCellState(mousePos.x, mousePos.y, CellState::Walkable);
                    stateHasChanged = true;
                }
            }
            else if(sf::Mouse::isButtonPressed(sf::Mouse::Left)){
                if(map.getCellState(mousePos.x, mousePos.y) != CellState::Start && map.getCellState(mousePos.x, mousePos.y) != CellState::Goal){
                    //std::cout << "Cella (" << mousePos.x << ", " << mousePos.y << ") ostacolo\n";
                    map.setCellState(mousePos.x, mousePos.y, CellState::Obstacle);
                    stateHasChanged = true;
                }

            }

        }


        map.draw(window);
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
