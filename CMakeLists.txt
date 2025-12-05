cmake_minimum_required(VERSION 3.25)
project(PathFindingAStarElsaidRexha)

set(CMAKE_CXX_STANDARD 23)

include(FetchContent)

FetchContent_Declare(
        SFML
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG        2.6.0
)

FetchContent_MakeAvailable(SFML)

add_subdirectory(test)

add_executable(PathFindingAStarElsaidRexha main.cpp Map.cpp Map.h CellState.h
        Node.h
        AStar.cpp
        AStar.h
        GameCharacter.cpp
        GameCharacter.h
        MazeGenerator.cpp
        MazeGenerator.h
        Dijkstra.cpp
        Dijkstra.h
        Utility/Button.cpp
        Utility/Button.h
        Utility/ButtonManager.h)

target_link_libraries(${PROJECT_NAME} PRIVATE sfml-graphics sfml-window sfml-system)
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)