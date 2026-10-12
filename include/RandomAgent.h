// The random agent simulates playing blackjack without knowing your own hand or the dealers hand.
// It is the baseline in which the Q-Learning agent will be compared against.

#ifndef RANDOM_AGENT_H
#define RANDOM_AGENT_H

#include "Agent.h"

#include <random>

class RandomAgent : public Agent {
public:
    RandomAgent();

    GameAction chooseAction(
        const Hand& playerHand,
        const Card& dealerUpcard
    ) override;

private:
    std::mt19937 randomEngine;
    std::uniform_int_distribution<int> actionDistribution;
};

#endif // RANDOM_AGENT_H
