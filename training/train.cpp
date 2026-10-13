// training Q-learning agent

#include "QLearningAgent.h"
#include "Game.h"

#include <algorithm>
#include <iostream>

namespace {

constexpr int NUM_TRAINING_ROUNDS = 300'000;
constexpr double INITIAL_BANKROLL = 1'000'000'000.0;
constexpr double INITIAL_BET = 10.0;

void trainAgent(QLearningAgent& agent, int numRounds) {
    Game game(INITIAL_BANKROLL);

    std::cout << "Training Q-learning agent for "
              << numRounds << " rounds...\n";

    for (int round = 0; round < numRounds; ++round) {
        if (game.getBankroll() < INITIAL_BET) {
            std::cout << "Training stopped: insufficient bankroll.\n";
            break;
        }

        // Gradually reduce exploration from 1.0 to 0.05.
        const double progress =
            static_cast<double>(round) / numRounds;

        const double epsilon =
            std::max(0.05, 1.0 - 0.95 * progress);

        agent.setEpsilon(epsilon);

        game.startRound(INITIAL_BET);

        while (!game.isGameOver()) {
            const std::size_t handIndex =
                game.getActiveHandIndex();

            const Hand& hand =
                game.getPlayerHand(handIndex);

            const Card& dealerUpcard =
                game.getDealerHand().getCards().front();

            // Encode the current game situation.
            const int state = QLearningAgent::encodeState(
                hand, dealerUpcard
            );

            // Choose Hit or Stand using epsilon-greedy policy.
            const GameAction action =
                agent.chooseAction(hand, dealerUpcard);

            // Apply the action to the game.
            game.playAction(action);

            const bool terminal = game.isGameOver();

            // Reward the agent when the round ends.
            const double reward = terminal
                ? game.getRoundProfit() / INITIAL_BET
                : 0.0;

            int nextState = 0;

            if (!terminal) {
                const Hand& nextHand =
                    game.getPlayerHand(
                        game.getActiveHandIndex()
                    );

                nextState = QLearningAgent::encodeState(
                    nextHand, dealerUpcard
                );
            }

            // Update the Q-table.
            agent.update(
                state,
                action,
                reward,
                nextState,
                terminal
            );
        }

        if ((round + 1) % 50'000 == 0) {
            std::cout << "Completed "
                      << round + 1
                      << " / " << numRounds
                      << " rounds\n";
        }
    }

    // Disable exploration after training.
    agent.setEpsilon(0.0);

    std::cout << "Training complete.\n";
}

} // namespace

int main() {
    QLearningAgent agent;

    trainAgent(agent, NUM_TRAINING_ROUNDS);

    if (agent.saveQTable("data/q_table.txt")) {
        std::cout << "Q-table saved successfully.\n";
    } else {
        std::cerr << "Error: Could not save Q-table.\n";
        return 1;
    }

    return 0;
}