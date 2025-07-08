//
// Created by elsai on 08/07/2025.
//

#include <random>
#include "Map.h"

Map::Map(int x, int y) : X(x), Y(y) {
    grid.resize(Y, std::vector<CellState>(X, CellState::Walkable));
    grid[0][0] = CellState::Start;
    grid[Y - 1][X - 1] = CellState::Goal;
}

bool Map::isWalkable(int x, int y) const {
    if (x < 0 || x >= X || y < 0 || y >= Y) {
        return false;
    }
    return grid[y][x] == CellState::Walkable;
}
void Map::setObstacle(int x, int y) {
    if (x >= 0 && x < X && y >= 0 && y < Y) {
        grid[y][x] = CellState::Obstacle;
    } else {
        std::cerr << "Coordinates out of bounds: (" << x << ", " << y << ")\n";
    }
}
void Map::draw(sf::RenderWindow& window) {
    CellSize = window.getSize().x / X;
    for (int i = 0; i < Y; ++i) {
        for (int j = 0; j < X; ++j) {
            sf::RectangleShape cell(sf::Vector2f(CellSize, CellSize));
            cell.setOutlineColor(sf::Color::Black);
            cell.setOutlineThickness(1);
            cell.setPosition(i * CellSize, j * CellSize);

            cell.setFillColor(GetCellColor(grid[i][j]));

            window.draw(cell);
        }
    }
}

void Map::reset() {
    for (int i = 0; i < Y; ++i)
        for (int j = 0; j < X; ++j)
            grid[i][j] = CellState::Walkable;


    grid[0][0] = CellState::Start;
    grid[Y - 1][X - 1] = CellState::Goal;
}

void Map::GenerateObstacles(int numObstacles) {
    std::random_device rd;
    std::mt19937 g(rd());

    std::vector<std::pair<int, int>> coords;
    for (int y = 0; y < Y; y++) {
        for (int x = 0; x < X; x++) {
            if (grid[y][x] == CellState::Start || grid[y][x] == CellState::Goal)
                continue;
            coords.emplace_back(x, y);
        }
    }

    std::shuffle(coords.begin(), coords.end(), g);

    for (int i = 0; i < numObstacles && i < coords.size(); i++)
        setObstacle(coords[i].first, coords[i].second);
}
