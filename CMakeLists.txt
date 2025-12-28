cmake_minimum_required(VERSION 3.16)
project(SnakeGame)

set(CMAKE_CXX_STANDARD 17)

# Add audio to the list of SFML components
find_package(SFML 2.5 COMPONENTS graphics window system audio REQUIRED)

add_executable(SnakeGame main.cpp)

# Link audio along with the other SFML libraries
target_link_libraries(SnakeGame sfml-graphics sfml-window sfml-system sfml-audio)

