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

    void update(sf::Vector2i nextPosition);
    void draw(sf::RenderWindow& window);
    sf::Vector2i getPosition() const;


private:
    sf::Vector2i position;
    float size;
    sf::Texture texture;
    sf::Sprite sprite;
};


#endif //PATHFINDINGASTARELSAIDREXHA_GAMECHARACTER_H
