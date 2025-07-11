//
// Created by elsai on 11/07/2025.
//

#include "gtest/gtest.h"
#include "../Map.h"

class MapTest : public ::testing::Test {
protected:
    Map map;

    MapTest() : map(10, 10) {}

    void SetUp() override {
        map.setCellState(0, 0, CellState::Start);
        map.setCellState(9, 9, CellState::Goal);
    }
};

TEST_F(MapTest, Initialization) {
    EXPECT_EQ(map.getWidth(), 10);
    EXPECT_EQ(map.getHeight(), 10);
    EXPECT_EQ(map.getCellState(0, 0), CellState::Start);
    EXPECT_EQ(map.getCellState(9, 9), CellState::Goal);
}

TEST_F(MapTest, CellStateInitialization) {
    EXPECT_EQ(map.getCellState(0, 0), CellState::Start);
    EXPECT_EQ(map.getCellState(9, 9), CellState::Goal);
    EXPECT_EQ(map.getCellState(1, 1), CellState::Walkable);
    map.setCellState(1, 1, CellState::Obstacle);
    EXPECT_EQ(map.getCellState(1, 1), CellState::Obstacle);
}

TEST_F(MapTest, IsWalkable) {
    EXPECT_TRUE(map.isWalkable(0, 0)); // Start cell
    EXPECT_TRUE(map.isWalkable(9, 9)); // Goal cell
    EXPECT_FALSE(map.isWalkable(-1, -1)); // Out of bounds
    EXPECT_FALSE(map.isWalkable(10, 10)); // Out of bounds
    map.setCellState(1, 1, CellState::Obstacle);
    EXPECT_FALSE(map.isWalkable(1, 1)); // Obstacle cell
}

TEST_F(MapTest, CanPlaceObstacle) {
    EXPECT_TRUE(map.canPlaceObstacle(1, 1)); // Walkable cell
    map.setCellState(1, 1, CellState::Obstacle);
    EXPECT_FALSE(map.canPlaceObstacle(1, 1)); // Obstacle cell
    map.setCellState(1, 1, CellState::Visited);
    EXPECT_TRUE(map.canPlaceObstacle(1, 1)); // Visited cell
    map.setCellState(1, 1, CellState::Path);
    EXPECT_TRUE(map.canPlaceObstacle(1, 1)); // Path cell

    EXPECT_FALSE(map.canPlaceObstacle(0, 0)); // Start cell
    EXPECT_FALSE(map.canPlaceObstacle(9, 9)); // Goal cell
}

TEST_F(MapTest, CanPlaceWalkable) {
    EXPECT_FALSE(map.canPlaceWalkable(1, 1));
    map.setCellState(1, 1, CellState::Obstacle);
    EXPECT_TRUE(map.canPlaceWalkable(1, 1)); // Can place walkable on an obstacle

    map.setCellState(1, 1, CellState::Visited);
    EXPECT_FALSE(map.canPlaceWalkable(1, 1)); // Can't place walkable on visited cell

    map.setCellState(1, 1, CellState::Path);
    EXPECT_FALSE(map.canPlaceWalkable(1, 1)); // Can't place walkable on visited cell

    EXPECT_FALSE(map.canPlaceWalkable(0, 0)); // Start cell
    EXPECT_FALSE(map.canPlaceWalkable(9, 9)); // Goal cell
}

TEST_F(MapTest, Reset) {
    map.setCellState(1, 1, CellState::Obstacle);
    map.reset();
    EXPECT_EQ(map.getCellState(1, 1), CellState::Walkable); // Should reset to Walkable
    EXPECT_EQ(map.getCellState(0, 0), CellState::Start); // Start cell should remain
    EXPECT_EQ(map.getCellState(9, 9), CellState::Goal); // Goal cell should remain
}

TEST_F(MapTest, ResetForRecalculation) {
    map.setCellState(1, 1, CellState::Obstacle);
    map.setCellState(2, 2, CellState::Visited);
    map.setCellState(3, 3, CellState::Path);
    map.resetForRecalculation();
    EXPECT_EQ(map.getCellState(1, 1), CellState::Obstacle); // Should reset to Walkable
    EXPECT_EQ(map.getCellState(2, 2), CellState::Walkable); // Visited should reset to Walkable
    EXPECT_EQ(map.getCellState(3, 3), CellState::Walkable); // Path should reset to Walkable

    EXPECT_EQ(map.getCellState(0, 0), CellState::Start); // Start cell should remain
    EXPECT_EQ(map.getCellState(9, 9), CellState::Goal); // Goal cell should remain
}

TEST_F(MapTest, GenerateObstacles) {
    map.generateObstacles(5);
    int obstacleCount = 0;
    for (int x = 0; x < map.getWidth(); ++x) {
        for (int y = 0; y < map.getHeight(); ++y) {
            if (map.getCellState(x, y) == CellState::Obstacle) {
                obstacleCount++;
            }
        }
    }
    EXPECT_GE(obstacleCount, 5); // At least 5 obstacles should be generated
}

TEST_F(MapTest, GettersAndSetters) {
    map.setCellSize(10);
    EXPECT_FLOAT_EQ(map.getCellSize(), 10);
    EXPECT_EQ(map.getCellSize(), 10);
    map.setStart(sf::Vector2i(1, 1));
    EXPECT_EQ(map.getStart(), sf::Vector2i(1, 1));
    EXPECT_FALSE(map.getCellState(0, 0) == CellState::Start); // Start cell should not be at (0, 0) anymore
    EXPECT_TRUE(map.getCellState(0, 0) == CellState::Walkable);
    map.setGoal(sf::Vector2i(8, 8));
    EXPECT_EQ(map.getGoal(), sf::Vector2i(8, 8));
    EXPECT_FALSE(map.getCellState(9, 9) == CellState::Goal); // Goal cell should not be at (9, 9) anymore
    EXPECT_TRUE(map.getCellState(9, 9) == CellState::Walkable);
}

TEST_F(MapTest, ToggleDebugMode) {
    EXPECT_FALSE(map.isDebug());
    map.toggleDebugMode();
    EXPECT_TRUE(map.isDebug());
    map.toggleDebugMode();
    EXPECT_FALSE(map.isDebug());
}