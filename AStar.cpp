#include "AStar.h"
#include "Map.h"
#include "Node.h"

#include <queue>
#include <unordered_map>
#include <algorithm>
#include <limits>
#include <cmath>

float heuristic(const sf::Vector2i& a, const sf::Vector2i& b) {
    return static_cast<float>(std::abs(a.x - b.x) + std::abs(a.y - b.y)); // Manhattan
}

std::pair<std::vector<sf::Vector2i>, std::vector<sf::Vector2i>>
AStar::findPath(Map& map, const sf::Vector2i& start, const sf::Vector2i& goal)
{
    int width  = map.getWidth();
    int height = map.getHeight();

    std::priority_queue<Node, std::vector<Node>, std::greater<>> openSet;
    std::vector<std::vector<float>> gScore(width,std::vector<float>(height,std::numeric_limits<float>::infinity()));
    std::vector<std::vector<bool>> closedSet(width,std::vector<bool>(height, false));
    std::unordered_map<int, sf::Vector2i> cameFrom;

    gScore[start.x][start.y] = 0.f;
    openSet.emplace(start.x, start.y, 0.f, heuristic(start, goal));

    const std::vector<sf::Vector2i> directions = {
            { 1,  0}, {-1,  0},  // destra, sinistra
            { 0,  1}, { 0, -1}   // giù, su
    };

    while (!openSet.empty()) {
        Node current = openSet.top();
        openSet.pop();

        if (current.x == goal.x && current.y == goal.y) {
            std::vector<sf::Vector2i> path;
            sf::Vector2i cur = goal;
            while (cur != start) {
                path.push_back(cur);
                cur = cameFrom[cur.x * height + cur.y];
            }
            std::reverse(path.begin(), path.end());

            std::vector<sf::Vector2i> visited;
            for (int x = 0; x < width; ++x)
                for (int y = 0; y < height; ++y)
                    if (closedSet[x][y])
                        visited.emplace_back(x, y);

            return { path, visited };
        }

        if (closedSet[current.x][current.y]) continue;
        closedSet[current.x][current.y] = true;

        for (const auto& dir : directions) {
            int nx = current.x + dir.x;
            int ny = current.y + dir.y;

            if (!map.isWalkable(nx, ny)) continue;

            float tentativeG = gScore[current.x][current.y] + 1.f;

            if (tentativeG < gScore[nx][ny]) {
                gScore[nx][ny] = tentativeG;
                float h = heuristic({nx, ny}, goal);
                openSet.emplace(nx, ny, tentativeG, h);
                cameFrom[nx * height + ny] = { current.x, current.y };
            }
        }
    }

    return {}; // nessun percorso trovato
}
