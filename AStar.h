//
// Created by elsai on 09/07/2025.
//

#ifndef PATHFINDINGASTARELSAIDREXHA_ASTAR_H
#define PATHFINDINGASTARELSAIDREXHA_ASTAR_H

#include <vector>
#include <SFML/System/Vector2.hpp>
#include "Map.h"

class AStar {
public:
    static std::vector<sf::Vector2i> findPath(Map& map, const sf::Vector2i& start, const sf::Vector2i& goal);
};

float heuristic(const sf::Vector2i& a, const sf::Vector2i& b);



#endif //PATHFINDINGASTARELSAIDREXHA_ASTAR_H
