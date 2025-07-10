//
// Created by elsai on 08/07/2025.
//

#ifndef PATHFINDINGASTARELSAIDREXHA_CELLSTATE_H
#define PATHFINDINGASTARELSAIDREXHA_CELLSTATE_H

enum class CellState {
    Walkable,    // 0
    Obstacle,    // 1
    Start,       // 2
    Goal,        // 3
    Path,        // 4
    Visited      // 5
};

static sf::Color GetCellColor(CellState state) {
    switch (state) {
        case CellState::Walkable:
            return sf::Color::Green;
        case CellState::Obstacle:
            return sf::Color::Red;
        case CellState::Start:
            return sf::Color::Blue;
        case CellState::Goal:
            return sf::Color::Magenta;
        case CellState::Path:
            return {128, 0, 128}; // Viola
        case CellState::Visited:
            return {200, 200, 200}; // Grigio
        default:
            return sf::Color::Black;
    }
}


#endif //PATHFINDINGASTARELSAIDREXHA_CELLSTATE_H
