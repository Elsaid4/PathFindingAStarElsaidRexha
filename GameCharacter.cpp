//
// Created by elsai on 12/07/2025.
//

#include "GameCharacter.h"

GameCharacter::GameCharacter(sf::Vector2i startPos, float size) : position(startPos), size(size) {
    if (!texture.loadFromFile("../Assets/Isaac.png")) {
        std::cerr << "Error loading character texture\n";
    }
    sprite.setTexture(texture);
    sprite.setScale(size / 32.f, size / 32.f);
}

void GameCharacter::update(sf::Vector2i nextPosition) {
    position = nextPosition;
}

void GameCharacter::draw(sf::RenderWindow& window) {
    sprite.setPosition((float)position.x * size, (float)position.y * size);
    window.draw(sprite);
    /*
    sf::CircleShape characterShape(size);
    characterShape.setFillColor(sf::Color::Yellow);
    characterShape.setPosition((float)position.x * cellSize, (float)position.y * cellSize);
    window.draw(characterShape);
    */
}

sf::Vector2i GameCharacter::getPosition() const {
    return position;
}