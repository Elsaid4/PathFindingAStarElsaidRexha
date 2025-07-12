//
// Created by elsai on 12/07/2025.
//

#include "gtest/gtest.h"
#include "../AStar.h"
#include "../Map.h"

class AStarTest : public ::testing::Test {
protected:
    Map map;

    AStarTest() : map(10, 10) {
        map.setCellState(0, 0, CellState::Start);
        map.setCellState(9, 9, CellState::Goal);
    }

    void SetUp() override {
        map.reset();
    }
};

TEST_F(AStarTest, FindPath) {
    auto result = AStar::findPath(map, map.getStart(), map.getGoal());
    EXPECT_FALSE(result.first.empty()); // Trova un percorso
    EXPECT_FALSE(result.second.empty()); // Dovrebbe avere delle celle visitate

    map.setCellState(0, 1, CellState::Obstacle);
    map.setCellState(1, 0, CellState::Obstacle);
    result = AStar::findPath(map, map.getStart(), map.getGoal());
    EXPECT_TRUE(result.first.empty()); // Non trova un percorso se ci sono ostacoli bloccanti
    EXPECT_TRUE(result.second.empty()); // Non dovrebbe avere delle celle visitate
}