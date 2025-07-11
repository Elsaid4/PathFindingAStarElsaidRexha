cmake_minimum_required(VERSION 3.25)

add_subdirectory(./lib/googletest)
set(gtest_SOURCE_DIR, ./lib/googletest/)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

set(SOURCE_FILES ../Map.cpp)
set(TEST_SOURCE_FILES runAllTests.cpp MapFixture.cpp)
add_executable(runMapTests ${SOURCE_FILES} ${TEST_SOURCE_FILES})
target_link_libraries(runMapTests gtest gtest_main sfml-graphics sfml-window sfml-system)