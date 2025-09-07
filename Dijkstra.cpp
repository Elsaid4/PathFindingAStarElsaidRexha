//
// Created by elsai on 07/09/2025.
//

#include "Dijkstra.h"
#include <queue>
#include <unordered_map>
#include <vector>
#include <SFML/System/Vector2.hpp>
#include "Map.h"

std::pair<std::vector<sf::Vector2i>, std::vector<sf::Vector2i>> Dijkstra::findPath(Map &map, const sf::Vector2i &start, const sf::Vector2i &goal) {
    std::vector<sf::Vector2i> visited;
    std::unordered_map<int, sf::Vector2i> cameFrom;
    std::unordered_map<int, int> costSoFar;

    auto hash = [&](const sf::Vector2i& v) { return v.x + v.y * map.getWidth(); };

    struct Node {
        sf::Vector2i pos;
        int cost;
        bool operator>(const Node& other) const { return cost > other.cost; }
    };

    std::priority_queue<Node, std::vector<Node>, std::greater<>> frontier;
    frontier.push({start, 0});
    cameFrom[hash(start)] = start;
    costSoFar[hash(start)] = 0;

    while (!frontier.empty()) {
        auto current = frontier.top();
        frontier.pop();

        visited.push_back(current.pos);

        if (current.pos == goal)
            break;

        for (auto& dir : std::vector<sf::Vector2i>{{0,1},{1,0},{0,-1},{-1,0}}) {
            sf::Vector2i next = current.pos + dir;
            if (!map.isWalkable(next.x, next.y))
                continue;

            int newCost = costSoFar[hash(current.pos)] + 1;
            if (!costSoFar.count(hash(next)) || newCost < costSoFar[hash(next)]) {
                costSoFar[hash(next)] = newCost;
                frontier.push({next, newCost});
                cameFrom[hash(next)] = current.pos;
            }
        }
    }

    // Ricostruzione percorso
    std::vector<sf::Vector2i> path;
    sf::Vector2i current = goal;
    if (cameFrom.count(hash(goal))) {
        while (current != start) {
            path.push_back(current);
            current = cameFrom[hash(current)];
        }
        //path.push_back(start);
        std::reverse(path.begin(), path.end());
    }

    return {path, visited};
}