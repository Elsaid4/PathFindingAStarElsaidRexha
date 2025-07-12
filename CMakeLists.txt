cmake_minimum_required(VERSION 3.25)
project(PathFindingAStarElsaidRexha)

set(CMAKE_CXX_STANDARD 23)

set(SFML_DIR "C:/SFML/lib/cmake/SFML")

find_package(SFML 2.6 COMPONENTS graphics window system REQUIRED)

add_subdirectory(test)

add_executable(PathFindingAStarElsaidRexha main.cpp Map.cpp Map.h CellState.h
        Node.h
        AStar.cpp
        AStar.h
        GameCharacter.cpp
        GameCharacter.h)

target_link_libraries(${PROJECT_NAME} PRIVATE sfml-graphics sfml-window sfml-system)
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)

if (WIN32 AND NOT SFML_STATIC_LIBRARIES)
    add_custom_command(TARGET PathFindingAStarElsaidRexha POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_RUNTIME_DLLS:PathFindingAStarElsaidRexha> $<TARGET_FILE_DIR:PathFindingAStarElsaidRexha>
            COMMAND_EXPAND_LISTS
    )
endif()

install(TARGETS ${PROJECT_NAME})