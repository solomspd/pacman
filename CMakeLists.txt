cmake_minimum_required(VERSION 3.10)

project(pacman VERSION 1.0)

find_package(SFML COMPONENTS graphics window system audio network REQUIRED)

include_directories(${PROJECT_SOURCE_DIR}/headers)

add_executable(pacman src/main.cpp src/blue_ghost.cpp  src/board.cpp  src/coin.cpp  src/ghost.cpp  src/interaction_box.cpp  src/map_editor_screen.cpp  src/map_select_screen.cpp  src/master_element.cpp  src/pac_man.cpp  src/pink_ghost.cpp  src/red_ghost.cpp  src/tile.cpp  src/yellow_ghost.cpp)
target_link_libraries(pacman sfml-graphics sfml-window sfml-system sfml-audio sfml-network)