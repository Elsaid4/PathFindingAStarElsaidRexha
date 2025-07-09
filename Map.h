//
// Created by elsai on 08/07/2025.
//

#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include "CellState.h"

#ifndef PATHFINDINGASTARELSAIDREXHA_MAP_H
#define PATHFINDINGASTARELSAIDREXHA_MAP_H


class Map {
public:
    Map(int width, int height);
    bool isWalkable(int x, int y) const;
    void setObstacle(int x, int y);
    void setCellState(int x, int y, CellState state);
    void draw(sf::RenderWindow& window);
    void reset();
    void generateObstacles(int numObstacles);
    int getWidth();
    int getHeight();
    sf::Vector2i getStart() const;
    sf::Vector2i getGoal() const;


private:
    std::vector<std::vector<CellState>> grid;
    int X, Y;
    int CellSize;
    sf::Vector2i start;
    sf::Vector2i goal;
    sf::Font font;
    bool isDebugMode = false;
};


#endif //PATHFINDINGASTARELSAIDREXHA_MAP_H
