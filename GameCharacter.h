//
// Created by elsai on 12/07/2025.
//

#ifndef PATHFINDINGASTARELSAIDREXHA_GAMECHARACTER_H
#define PATHFINDINGASTARELSAIDREXHA_GAMECHARACTER_H


#include <SFML/System/Vector2.hpp>
#include "Map.h"

class GameCharacter {
public:
    GameCharacter(sf::Vector2i startPos, float size);

    sf::Vector2i update(Map& map, std::vector<sf::Vector2i> path);
    void draw(sf::RenderWindow& window, float cellSize);


private:
    sf::Vector2i position;
    float size;
};


#endif //PATHFINDINGASTARELSAIDREXHA_GAMECHARACTER_H
