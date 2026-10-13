#include "BasicStrategyAgent.h"
#include "RandomAgent.h"
#include "game.h"

#include <iomanip>
#include <iostream>
#include <string>

struct EvaluationResults {
    int wins = 0;
    int losses = 0;
    int pushes = 0;
    int rounds = 0;

    double averageReward() const {
        if (rounds == 0) {
            return 0.0;
        }

        return static_cast<double>(wins - losses) / rounds;
    }

    double winRate() const {
        if (rounds == 0) {
            return 0.0;
        }

        return 100.0 * wins / rounds;
    }

    double lossRate() const {
        if (rounds == 0) {
            return 0.0;
        }

        return 100.0 * losses / rounds;
    }

    double pushRate() const {
        if (rounds == 0) {
            return 0.0;
        }

        return 100.0 * pushes / rounds;
    }
};

EvaluationResults evaluateAgent(Agent& agent, int numRounds) {
    Game game;
    EvaluationResults results;

    for (int i = 0; i < numRounds; ++i) {
        game.startRound();

        while (!game.isGameOver()) {
            const Hand& playerHand = game.getPlayerHand();
            const Hand& dealerHand = game.getDealerHand();

            const Card& dealerUpcard =
                dealerHand.getCards().front();

            GameAction action =
                agent.chooseAction(playerHand, dealerUpcard);

            game.playAction(action);
        }

        switch (game.getStatus()) {
            case GameStatus::PlayerWins:
                ++results.wins;
                break;

            case GameStatus::DealerWins:
                ++results.losses;
                break;

            case GameStatus::Push:
                ++results.pushes;
                break;

            default:
                break;
        }

        ++results.rounds;
    }

    return results;
}

void printResults(
    const std::string& agentName,
    const EvaluationResults& results
) {
    std::cout << "\n===== " << agentName << " =====\n";

    std::cout << "Rounds: " << results.rounds << '\n';
    std::cout << "Wins: " << results.wins << '\n';
    std::cout << "Losses: " << results.losses << '\n';
    std::cout << "Pushes: " << results.pushes << '\n';

    std::cout << std::fixed << std::setprecision(2);

    std::cout << "Win rate: "
              << results.winRate() << "%\n";

    std::cout << "Loss rate: "
              << results.lossRate() << "%\n";

    std::cout << "Push rate: "
              << results.pushRate() << "%\n";

    std::cout << "Average reward: "
              << results.averageReward() << '\n';
}

int main() {
    constexpr int NUM_ROUNDS = 100000;

    RandomAgent randomAgent;
    BasicStrategyAgent basicStrategyAgent;

    std::cout << "Running " << NUM_ROUNDS
              << " rounds per agent...\n";

    EvaluationResults randomResults =
        evaluateAgent(randomAgent, NUM_ROUNDS);

    EvaluationResults strategyResults =
        evaluateAgent(basicStrategyAgent, NUM_ROUNDS);

    printResults("RandomAgent", randomResults);
    printResults("BasicStrategyAgent", strategyResults);

    return 0;
}