//
// Created by elsai on 08/07/2025.
//

#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include "CellState.h"
#include "PerlinNoise.hpp"

#ifndef PATHFINDINGASTARELSAIDREXHA_MAP_H
#define PATHFINDINGASTARELSAIDREXHA_MAP_H


class Map {
public:
    Map(int width, int height);
    bool isWalkable(int x, int y) const;
    void setCellState(int x, int y, CellState state);
    void draw(sf::RenderWindow& window, sf::Font& font, bool drawBorder = true);
    void reset();
    void resetForRecalculation();
    void generateRandomObstacles(int numObstacles);
    void generateObstaclesPerlin(float threshold, float scale, int seed);
    int getWidth() const;
    int getHeight() const;
    float getCellSize() const;
    void setCellSize(float size);
    CellState getCellState(int x, int y) const;
    bool canPlaceObstacle(int x, int y) const;
    bool canPlaceWalkable(int x, int y) const;
    sf::Vector2i getStart() const;
    sf::Vector2i getGoal() const;
    void setStart(const sf::Vector2i& pos);
    void setGoal(const sf::Vector2i& pos);
    void toggleDebugMode() { isDebugMode = !isDebugMode; }
    bool isDebug() const { return isDebugMode; }


private:
    std::vector<std::vector<CellState>> grid;
    int X, Y;
    float CellSize;
    sf::Vector2i start;
    sf::Vector2i goal;
    bool isDebugMode = false;
};


#endif //PATHFINDINGASTARELSAIDREXHA_MAP_H
