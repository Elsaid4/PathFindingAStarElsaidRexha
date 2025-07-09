//
// Created by elsai on 08/07/2025.
//

#include <random>
#include "Map.h"

Map::Map(int x, int y) : X(x), Y(y) {
    grid.resize(Y, std::vector<CellState>(X, CellState::Walkable));
    grid[0][0] = CellState::Start;
    grid[Y - 1][X - 1] = CellState::Goal;
    start = sf::Vector2i(0, 0);
    goal = sf::Vector2i(X - 1, Y - 1);
    if (!font.loadFromFile("C:\\Users\\elsai\\CLionProjects\\PathFindingAStarElsaidRexha\\Font\\OpenSans-VariableFont_wdth,wght.ttf")) { // Ensure you have a valid font file
        std::cerr << "Error loading font\n";
    }
}

bool Map::isWalkable(int x, int y) const {
    if (x < 0 || x >= X || y < 0 || y >= Y) {
        return false;
    }
    return grid[y][x] == CellState::Walkable || grid[y][x] == CellState::Start || grid[y][x] == CellState::Goal;
}

void Map::setObstacle(int x, int y) {
    if (x >= 0 && x < X && y >= 0 && y < Y) {
        grid[y][x] = CellState::Obstacle;
    } else {
        std::cerr << "Coordinates out of bounds: (" << x << ", " << y << ")\n";
    }
}

void Map::setCellState(int x, int y, CellState state) {
    if (x >= 0 && x < X && y >= 0 && y < Y) {
        if (grid[y][x] != CellState::Start && grid[y][x] != CellState::Goal)
            grid[y][x] = state;
    } else {
        std::cerr << "Coordinates out of bounds: (" << x << ", " << y << ")\n";
    }
}

void Map::draw(sf::RenderWindow& window) {
    CellSize = window.getSize().x / X;
    for (int i = 0; i < Y; ++i) {
        for (int j = 0; j < X; ++j) {
            sf::RectangleShape cell(sf::Vector2f(CellSize, CellSize));
            //cell.setOutlineColor(sf::Color::Black);
            //cell.setOutlineThickness(1);
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
    for (int i = 0; i < Y; ++i)
        for (int j = 0; j < X; ++j)
            grid[i][j] = CellState::Walkable;


    grid[0][0] = CellState::Start;
    grid[Y - 1][X - 1] = CellState::Goal;
}

void Map::generateObstacles(int numObstacles) {
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

int Map::getWidth() {
    return X;
}

int Map::getHeight() {
    return Y;
}

sf::Vector2i Map::getStart() const {
    return start;
}

sf::Vector2i Map::getGoal() const {
    return goal;
}