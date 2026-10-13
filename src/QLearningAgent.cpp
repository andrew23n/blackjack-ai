#include "QLearningAgent.h"

#include <algorithm>
#include <stdexcept>
#include <string>
#include <fstream>

QLearningAgent::QLearningAgent()
    : randomEngine(std::random_device{}()) {
}

int QLearningAgent::actionToIndex(GameAction action) const {
    if (action == GameAction::Hit) {
        return 0;
    }

    if (action == GameAction::Stand) {
        return 1;
    }

    throw std::invalid_argument(
        "QLearningAgent only supports Hit and Stand."
    );
}

int QLearningAgent::encodeState(
    const Hand& playerHand,
    const Card& dealerUpcard
) {
    const int total = std::clamp(
        playerHand.getTotal(), 0, 21
    );

    const int usableAce =
        playerHand.hasUsableAce() ? 1 : 0;

    const int dealerValue = dealerUpcard.getValue();

    // Ace has value 11; face cards have value 10.
    const int dealerIndex =
        dealerValue == 11 ? 9 : dealerValue - 2;

    // State layout:
    // (player total * 2 + usable Ace) * 10 + dealer index
    return (total * 2 + usableAce) * 10
           + std::clamp(dealerIndex, 0, 9);
}

GameAction QLearningAgent::chooseAction(
    const Hand& playerHand,
    const Card& dealerUpcard
) {
    const int state = encodeState(
        playerHand, dealerUpcard
    );

    std::uniform_real_distribution<double> probability(
        0.0, 1.0
    );

    std::uniform_int_distribution<int> randomAction(
        0, NUM_ACTIONS - 1
    );

    // Epsilon-greedy exploration.
    // With probability epsilon, choose a random action.
    if (probability(randomEngine) < epsilon) {
        return randomAction(randomEngine) == 0
            ? GameAction::Hit
            : GameAction::Stand;
    }

    const double hitValue = qTable[state][0];
    const double standValue = qTable[state][1];

    // Randomly break ties to avoid a systematic preference.
    if (hitValue == standValue) {
        return randomAction(randomEngine) == 0
            ? GameAction::Hit
            : GameAction::Stand;
    }

    return hitValue > standValue
        ? GameAction::Hit
        : GameAction::Stand;
}

void QLearningAgent::update(
    int state,
    GameAction action,
    double reward,
    int nextState,
    bool terminal
) {
    const int actionIndex = actionToIndex(action);

    const double currentValue =
        qTable.at(state).at(actionIndex);

    double bestNextValue = 0.0;

    // Terminal states have no future action value.
    if (!terminal) {
        bestNextValue = std::max(
            qTable.at(nextState).at(0),
            qTable.at(nextState).at(1)
        );
    }

    const double target =
        reward + discountFactor * bestNextValue;

    qTable.at(state).at(actionIndex) =
        currentValue
        + learningRate * (target - currentValue);
}

void QLearningAgent::setEpsilon(double value) {
    epsilon = std::clamp(value, 0.0, 1.0);
}

bool QLearningAgent::saveQTable(
    const std::string& filename
) const {
    std::ofstream file(filename);

    if (!file.is_open()) {
        return false;
    }

    // Store a version marker and table dimensions.
    file << "BLACKJACK_Q_TABLE_V1 "
         << NUM_STATES << ' '
         << NUM_ACTIONS << '\n';

    file.precision(17);

    for (const auto& stateValues : qTable) {
        for (double value : stateValues) {
            file << value << ' ';
        }
        file << '\n';
    }

    return file.good();
}

bool QLearningAgent::loadQTable(
    const std::string& filename
) {
    std::ifstream file(filename);

    if (!file.is_open()) {
        return false;
    }

    std::string version;
    int states;
    int actions;

    if (!(file >> version >> states >> actions)) {
        return false;
    }

    if (version != "BLACKJACK_Q_TABLE_V1" ||
        states != NUM_STATES ||
        actions != NUM_ACTIONS) {
        return false;
    }

    // Load into a temporary table so a failed load
    // does not partially overwrite the current Q-table.
    decltype(qTable) loadedTable{};

    for (auto& stateValues : loadedTable) {
        for (double& value : stateValues) {
            if (!(file >> value)) {
                return false;
            }

            if (!std::isfinite(value)) {
                return false;
            }
        }
    }

    qTable = loadedTable;
    return true;
}