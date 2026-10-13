cmake_minimum_required(VERSION 3.20)

project(BlackjackAI LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Game engine and agent implementations
add_library(blackjack_core
    src/Card.cpp
    src/Deck.cpp
    src/Hand.cpp
    src/game.cpp
    src/Agent.cpp
    src/RandomAgent.cpp
    src/BasicStrategyAgent.cpp
    src/QLearningAgent.cpp
)

target_include_directories(blackjack_core
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Playable Blackjack game
add_executable(blackjack
    app/main.cpp
)

target_link_libraries(blackjack PRIVATE blackjack_core)

# Automated simulation / evaluation
add_executable(evaluate
    training/evaluate.cpp
)

target_link_libraries(evaluate PRIVATE blackjack_core)

add_executable(train
    training/train.cpp
)

target_link_libraries(train PRIVATE blackjack_core)