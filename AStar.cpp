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

std::vector<sf::Vector2i> AStar::findPath(Map& map, const sf::Vector2i& start, const sf::Vector2i& goal) {
    int width = map.getWidth();
    int height = map.getHeight();

    std::priority_queue<Node, std::vector<Node>, std::greater<Node>> openSet;
    std::vector<std::vector<float>> gScore(height, std::vector<float>(width, std::numeric_limits<float>::infinity()));
    std::unordered_map<int, sf::Vector2i> cameFrom;
    std::vector<std::vector<bool>> closedSet(height, std::vector<bool>(width, false));

    gScore[start.y][start.x] = 0.0f;
    openSet.push({start.x, start.y, 0.0f, heuristic(start, goal)});

    std::vector<sf::Vector2i> directions = {
            {0, -1}, {-1, 0}, {1, 0}, {0, 1}
    };

    while (!openSet.empty()) {
        Node current = openSet.top();
        openSet.pop();

        if (current.x == goal.x && current.y == goal.y) {
            std::vector<sf::Vector2i> path;
            sf::Vector2i cur = goal;
            while (cur != start) {
                path.push_back(cur);
                cur = cameFrom[cur.y * width + cur.x];
            }
            std::reverse(path.begin(), path.end());
            return path;
        }

        if (closedSet[current.y][current.x]) continue;
        closedSet[current.y][current.x] = true;

        for (auto& dir : directions) {
            int nx = current.x + dir.x;
            int ny = current.y + dir.y;

            if (!map.isWalkable(nx, ny)) continue;

            float tentativeG = gScore[current.y][current.x] + 1.0f;

            if (tentativeG < gScore[ny][nx]) {
                gScore[ny][nx] = tentativeG;
                float h = heuristic({nx, ny}, goal);
                openSet.push({nx, ny, tentativeG, h});
                cameFrom[ny * width + nx] = {current.x, current.y};
            }
        }
    }

    return {}; // Nessun percorso trovato
}
