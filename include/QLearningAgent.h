#ifndef Q_LEARNING_AGENT_H
#define Q_LEARNING_AGENT_H

#include "Agent.h"

#include <array>
#include <random>

class QLearningAgent : public Agent {
public:
    QLearningAgent();

    GameAction chooseAction(
        const Hand& playerHand,
        const Card& dealerUpcard
    ) override;

    // Convert a Blackjack situation into a Q-table state index.
    static int encodeState(
        const Hand& playerHand,
        const Card& dealerUpcard
    );

    // Update the Q-value after an action and its resulting reward.
    void update(
        int state,
        GameAction action,
        double reward,
        int nextState,
        bool terminal
    );

    void setEpsilon(double value);

    bool saveQTable(const std::string& filename) const;
    bool loadQTable(const std::string& filename);

private:
    static constexpr int NUM_STATES = 22 * 2 * 10;
    static constexpr int NUM_ACTIONS = 2;

    // Action 0 = Hit, action 1 = Stand.
    std::array<
        std::array<double, NUM_ACTIONS>,
        NUM_STATES
    > qTable{};

    double learningRate = 0.1;
    double discountFactor = 0.95;
    double epsilon = 1.0;

    std::mt19937 randomEngine;

    int actionToIndex(GameAction action) const;
};

#endif // Q_LEARNING_AGENT_H