//
// Created by elsai on 12/07/2025.
//

#ifndef PATHFINDINGASTARELSAIDREXHA_GAMECHARACTER_H
#define PATHFINDINGASTARELSAIDREXHA_GAMECHARACTER_H


class GameCharacter {
public:
    GameCharacter(int posX, int posY);

    void move(int deltaX, int deltaY);

private:
    float posX;
    float posY;
};


#endif //PATHFINDINGASTARELSAIDREXHA_GAMECHARACTER_H
