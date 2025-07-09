#include <iostream>
#include <SFML/Graphics.hpp>
#include <random>

#include "Map.h"

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 800), "A* Pathfinder");
    int mapWidth = 40;
    int mapHeight = 40;
    Map map(mapWidth, mapHeight);

    int numObstacles = 500;
    map.generateObstacles(numObstacles);

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
        }
        map.draw(window);
        window.display();
    }

    return 0;
}
