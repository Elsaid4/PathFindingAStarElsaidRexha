//
// Created by elsai on 07/09/2025.
//

#ifndef PATHFINDINGASTARELSAIDREXHA_DIJKSTRA_H
#define PATHFINDINGASTARELSAIDREXHA_DIJKSTRA_H


#include <utility>
#include <vector>
#include <SFML/System/Vector2.hpp>
#include "Map.h"

class Dijkstra {
public:
    static std::pair<std::vector<sf::Vector2i>, std::vector<sf::Vector2i>> findPath(Map& map, const sf::Vector2i& start, const sf::Vector2i& goal);

};


#endif //PATHFINDINGASTARELSAIDREXHA_DIJKSTRA_H
