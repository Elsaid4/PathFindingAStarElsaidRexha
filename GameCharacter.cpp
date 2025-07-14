//
// Created by elsai on 12/07/2025.
//

#include "GameCharacter.h"

GameCharacter::GameCharacter(sf::Vector2i startPos, float size) : position(startPos), size(size) {
    if (!texture.loadFromFile("../Assets/Character.png")) {
        std::cerr << "Error loading character texture\n";
    }
    sprite.setTexture(texture);
    sprite.setScale(size / 16.f, size / 16.f);
}

sf::Vector2i GameCharacter::update(Map& map, std::vector<sf::Vector2i> path) {
    if (!path.empty()) {
        position = path.front();
        path.erase(path.begin());
    }
    return position;
}

void GameCharacter::draw(sf::RenderWindow& window) {
    sprite.setPosition((float)position.x * size - size / 2, (float)position.y * size - size /2);
    window.draw(sprite);
    /*
    sf::CircleShape characterShape(size);
    characterShape.setFillColor(sf::Color::Yellow);
    characterShape.setPosition((float)position.x * cellSize, (float)position.y * cellSize);
    window.draw(characterShape);
    */
}