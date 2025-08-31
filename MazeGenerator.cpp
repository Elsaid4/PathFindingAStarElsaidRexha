#include <vector>
#include <random>
#include <algorithm>
#include "MazeGenerator.h"

void MazeGenerator::generateMaze(Map& map) {
    map.initMapAllObstacles();
    auto& maze = map.getGrid();
    std::vector<std::pair<int, int>> stack;
    std::random_device rd;
    std::mt19937 gen(rd());
    int width = map.getWidth();
    int height = map.getHeight();

    auto isValid = [&](int x, int y) {
        return x > 0 && x < width-1 && y > 0 && y < height-1;
    };

    stack.push_back({1, 1});
    maze[1][1] = CellState::Walkable;

    while (!stack.empty()) {
        auto [x, y] = stack.back();
        std::vector<std::pair<int, int>> neighbors;

        for (auto [dx, dy] : std::vector<std::pair<int, int>>{{2,0},{-2,0},{0,2},{0,-2}}) {
            int nx = x + dx, ny = y + dy;
            if (isValid(nx, ny) && maze[ny][nx] == CellState::Obstacle)
                neighbors.push_back({nx, ny});
        }

        if (!neighbors.empty()) {
            std::shuffle(neighbors.begin(), neighbors.end(), gen);
            auto [nx, ny] = neighbors.front();
            maze[ny][nx] = CellState::Walkable;
            maze[y + (ny-y)/2][x + (nx-x)/2] = CellState::Walkable;
            stack.push_back({nx, ny});
        } else {
            stack.pop_back();
        }
    }

    for (int i = 0; i < width; ++i)
        for (int j = 0; j < height; ++j)
            map.setCellState(i, j, maze[i][j] == CellState::Walkable ? CellState::Walkable : CellState::Obstacle);
}