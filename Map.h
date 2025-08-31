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
    Map(int width, int height, float cellSize);
    bool isWalkable(int x, int y) const;
    void setCellState(int x, int y, CellState state);
    void draw(sf::RenderWindow& window, sf::Font& font, bool drawBorder = false, bool drawTexture = true);
    void reset();
    void resetForRecalculation();
    void generateRandomObstacles(int numObstacles);
    void generateObstaclesPerlin(float threshold, float scale, int seed);
    void generateMaze();
    void initMapAllObstacles(){
        for (int i = 0; i < X; ++i)
            for (int j = 0; j < Y; ++j)
                grid[i][j] = CellState::Obstacle;
        grid[start.x][start.y] = CellState::Start;
        grid[goal.x][goal.y] = CellState::Goal;
    }
    std::vector<std::vector<CellState>>& getGrid() { return grid; }
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
    float cellSize;
    sf::Vector2i start;
    sf::Vector2i goal;
    bool isDebugMode = false;
    sf::Texture grassTexture;
    sf::Texture waterTexture;
    sf::Sprite cellSprite;
};


#endif //PATHFINDINGASTARELSAIDREXHA_MAP_H
