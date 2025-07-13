//
// Created by elsai on 12/07/2025.
//

#include "GameCharacter.h"

GameCharacter::GameCharacter(sf::Vector2i startPos, float size) : position(startPos), size(size) {}

sf::Vector2i GameCharacter::update(Map& map, std::vector<sf::Vector2i> path) {
    if (!path.empty()) {
        position = path.front();
        path.erase(path.begin());
    }
    return position;
}

void GameCharacter::draw(sf::RenderWindow& window, float cellSize) {
    sf::CircleShape characterShape(size);
    characterShape.setFillColor(sf::Color::Yellow);
    characterShape.setPosition((float)position.x * cellSize, (float)position.y * cellSize);
    window.draw(characterShape);
}