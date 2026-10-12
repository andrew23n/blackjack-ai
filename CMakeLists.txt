
cmake_minimum_required(VERSION 3.20)

project(BlackjackAI LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Core game library
add_library(blackjack_core
    src/Card.cpp
    src/Deck.cpp
    src/Hand.cpp
    src/Game.cpp
)

target_include_directories(blackjack_core
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Playable terminal application
add_executable(blackjack
    app/main.cpp
)

target_link_libraries(blackjack
    PRIVATE blackjack_core
)