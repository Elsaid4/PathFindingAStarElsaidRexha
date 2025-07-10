#include <iostream>
#include <SFML/Graphics.hpp>
#include <random>

#include "Map.h"
#include "AStar.h"

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 800), "A* Pathfinder");
    int mapWidth = 10;
    int mapHeight = 10;
    Map map(mapWidth, mapHeight);

    map.setCellSize(window.getSize().x / mapWidth);

    int numObstacles = 0;
    map.generateObstacles(numObstacles);

    /*
    auto result = AStar::findPath(map, map.getStart(), map.getGoal());

    if (result.first.empty()) {
        std::cout << "No path found.\n";
        //return 0;
    }

    for (const auto& cell : result.second) {
        map.setCellState(cell.x, cell.y, CellState::Visited);
    }

    for (const auto& cell : result.first) {
        map.setCellState(cell.x, cell.y, CellState::Path);
    }
    */

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::R)) {
            // Rigenera gli ostacoli
            map.reset();
            map.generateObstacles(numObstacles);

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
        }

        if (event.mouseButton.button == sf::Mouse::Left) {
            int x = event.mouseButton.x / map.getCellSize();
            int y = event.mouseButton.y / map.getCellSize();
            if (map.isWalkable(x, y)) {
                map.setCellState(x, y, CellState::Obstacle);
            }
        } else if (event.mouseButton.button == sf::Mouse::Right) {
            int x = event.mouseButton.x / map.getCellSize();
            int y = event.mouseButton.y / map.getCellSize();
            if (map.isWalkable(x, y)) {
                map.setCellState(x, y, CellState::Walkable);
            }
        }

        map.draw(window);
        window.display();
    }

    return 0;
}
