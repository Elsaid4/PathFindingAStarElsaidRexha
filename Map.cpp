//
// Created by elsai on 08/07/2025.
//

#include <random>
#include "Map.h"

Map::Map(int x, int y) : X(x), Y(y) {
    grid.resize(X, std::vector<CellState>(Y, CellState::Walkable));
    grid[0][0] = CellState::Start;
    grid[Y - 1][X - 1] = CellState::Goal;
    start = sf::Vector2i(0, 0);
    goal = sf::Vector2i(X - 1, Y - 1);
    if (!font.loadFromFile("../Font/OpenSans-VariableFont_wdth,wght.ttf")) {
        std::cerr << "Error loading font\n";
    }
    CellSize = 0;
}

bool Map::isWalkable(int x, int y) const {
    if (x < 0 || x >= X || y < 0 || y >= Y) {
        return false;
    }
    return grid[x][y] == CellState::Walkable || grid[x][y] == CellState::Start || grid[x][y] == CellState::Goal;
}

void Map::setCellState(int x, int y, CellState state) {
    if (x >= 0 && x < X && y >= 0 && y < Y) {
        if (grid[x][y] != CellState::Start && grid[x][y] != CellState::Goal)
            grid[x][y] = state;
    } else {
        std::cerr << "Coordinates out of bounds: (" << x << ", " << y << ")\n";
    }
}

void Map::draw(sf::RenderWindow& window) {
    for (int i = 0; i < X; i++) {
        for (int j = 0; j < Y; j++) {
            sf::RectangleShape cell(sf::Vector2f(CellSize,CellSize));
            cell.setOutlineColor(sf::Color::Black);
            cell.setOutlineThickness(1);
            cell.setPosition(i * CellSize, j * CellSize);

            cell.setFillColor(GetCellColor(grid[i][j]));

            window.draw(cell);

            if(isDebugMode) {
                sf::Text text;
                text.setFont(font);
                text.setString("("+ std::to_string(i) + "-" + std::to_string(j) + ")");
                text.setCharacterSize(17);
                text.setFillColor(sf::Color::Black);
                text.setPosition(i * CellSize + CellSize / 4, j * CellSize);
                window.draw(text);

                sf::Text state;
                state.setFont(font);
                state.setString(std::to_string(static_cast<int>(grid[i][j])));
                state.setCharacterSize(20);
                state.setFillColor(sf::Color::Black);
                state.setPosition(i * CellSize + CellSize / 2, j * CellSize + CellSize / 2);
                window.draw(state);
            }

        }
    }
}

void Map::reset() {
    for (int i = 0; i < X; ++i)
        for (int j = 0; j < Y; ++j)
            grid[i][j] = CellState::Walkable;


    grid[0][0] = CellState::Start;
    grid[X - 1][Y - 1] = CellState::Goal;
}

void Map::resetForRecalculation() {
    for (int i = 0; i < X; i++)
        for (int j = 0; j < Y; j++)
            if (grid[i][j] == CellState::Visited || grid[i][j] == CellState::Path)
                grid[i][j] = CellState::Walkable;
}

void Map::generateObstacles(int numObstacles) {
    std::random_device rd;
    std::mt19937 g(rd());

    std::vector<std::pair<int, int>> coords;
    for (int i = 0; i < X; i++) {
        for (int j = 0; j < Y; j++) {
            if (grid[i][j] == CellState::Start || grid[i][j] == CellState::Goal)
                continue;
            coords.emplace_back(i, j);
        }
    }

    std::shuffle(coords.begin(), coords.end(), g);

    for (int i = 0; i < numObstacles && i < coords.size(); i++)
        setCellState(coords[i].first, coords[i].second, CellState::Obstacle);
}

int Map::getWidth() const{
    return X;
}

int Map::getHeight() const{
    return Y;
}

float Map::getCellSize() const {
    return CellSize;
}

void Map::setCellSize(float size) {
    CellSize = size;
}

sf::Vector2i Map::getStart() const {
    return start;
}

sf::Vector2i Map::getGoal() const {
    return goal;
}

CellState Map::getCellState(int x, int y) const {
    if (x >= 0 && x < X && y >= 0 && y < Y) {
        return grid[y][x];
    } else {
        std::cerr << "Coordinates out of bounds: (" << x << ", " << y << ")\n";
        return CellState::Walkable;
    }
}