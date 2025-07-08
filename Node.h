//
// Created by elsai on 08/07/2025.
//

#ifndef PATHFINDINGASTARELSAIDREXHA_NODE_H
#define PATHFINDINGASTARELSAIDREXHA_NODE_H

struct Node {
    int x, y; // Coordinates of the node
    float gCost; // Cost from start to this node
    float hCost; // Heuristic cost to goal
    float fCost() const { return gCost + hCost; } // Total cost

    Node(int xCoord, int yCoord, float g = 0.0f, float h = 0.0f)
        : x(xCoord), y(yCoord), gCost(g), hCost(h) {}

    bool operator==(const Node& other) const {
        return x == other.x && y == other.y;
    }

    bool operator>(const Node& other) const {
        return fCost() > other.fCost();
    }
};

#endif //PATHFINDINGASTARELSAIDREXHA_NODE_H
