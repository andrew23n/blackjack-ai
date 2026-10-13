#include "BasicStrategyAgent.h"
#include "QLearningAgent.h"
#include "RandomAgent.h"
#include "Game.h"

#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

namespace {

constexpr int NUM_EVALUATION_ROUNDS = 100'000;

constexpr double INITIAL_BANKROLL = 1'000'000'000.0;
constexpr double INITIAL_BET = 10.0;

struct EvaluationResults {
    int rounds = 0;
    int wins = 0;
    int losses = 0;
    int pushes = 0;

    double totalProfit = 0.0;

    double averageProfit() const {
        return rounds == 0
            ? 0.0
            : totalProfit / rounds;
    }
};

std::string formatMoney(double amount) {
    std::ostringstream output;

    if (amount < 0.0) {
        output << "-$" << std::fixed
               << std::setprecision(2) << -amount;
    } else {
        output << "$" << std::fixed
               << std::setprecision(2) << amount;
    }

    return output.str();
}

EvaluationResults evaluateAgent(
    Agent& agent,
    int numRounds
) {
    EvaluationResults results;
    Game game(INITIAL_BANKROLL);

    for (int round = 0; round < numRounds; ++round) {
        if (game.getBankroll() < INITIAL_BET) {
            std::cout
                << "Evaluation stopped: insufficient bankroll.\n";
            break;
        }

        game.startRound(INITIAL_BET);

        while (!game.isGameOver()) {
            const std::size_t handIndex =
                game.getActiveHandIndex();

            const Hand& hand =
                game.getPlayerHand(handIndex);

            const Card& dealerUpcard =
                game.getDealerHand().getCards().front();

            const GameAction action =
                agent.chooseAction(hand, dealerUpcard);

            // The Q-learning and random agents support
            // only Hit and Stand. Basic strategy may also
            // request DoubleDown or Split.
            GameAction appliedAction = action;

            if (action == GameAction::Split ||
                action == GameAction::DoubleDown) {

                // Use a simple Hit/Stand fallback for agents
                // whose action set is restricted.
                const int total = hand.getTotal();
                const int dealer = dealerUpcard.getValue();

                if (hand.hasUsableAce()) {
                    if (total >= 19) {
                        appliedAction = GameAction::Stand;
                    } else if (total == 18 &&
                               dealer >= 2 && dealer <= 8) {
                        appliedAction = GameAction::Stand;
                    } else {
                        appliedAction = GameAction::Hit;
                    }
                } else if (total >= 17) {
                    appliedAction = GameAction::Stand;
                } else if (total <= 11) {
                    appliedAction = GameAction::Hit;
                } else if (total == 12) {
                    appliedAction =
                        (dealer >= 4 && dealer <= 6)
                        ? GameAction::Stand
                        : GameAction::Hit;
                } else if (total <= 16) {
                    appliedAction =
                        (dealer >= 2 && dealer <= 6)
                        ? GameAction::Stand
                        : GameAction::Hit;
                } else {
                    appliedAction = GameAction::Stand;
                }
            }

            game.playAction(appliedAction);
        }

        const double profit = game.getRoundProfit();

        results.totalProfit += profit;
        ++results.rounds;

        if (profit > 0.0) {
            ++results.wins;
        } else if (profit < 0.0) {
            ++results.losses;
        } else {
            ++results.pushes;
        }
    }

    return results;
}

void printResults(
    const std::string& agentName,
    const EvaluationResults& results
) {
    std::cout << "\n=== " << agentName << " ===\n";
    std::cout << "Rounds played: "
              << results.rounds << '\n';

    if (results.rounds == 0) {
        std::cout << "No rounds completed.\n";
        return;
    }

    const double rounds = results.rounds;

    std::cout << std::fixed << std::setprecision(2);

    std::cout << "Wins: " << results.wins << " ("
              << 100.0 * results.wins / rounds << "%)\n";

    std::cout << "Losses: " << results.losses << " ("
              << 100.0 * results.losses / rounds << "%)\n";

    std::cout << "Pushes: " << results.pushes << " ("
              << 100.0 * results.pushes / rounds << "%)\n";

    std::cout << "Total net profit: "
              << formatMoney(results.totalProfit) << '\n';

    std::cout << "Average profit per round: "
              << formatMoney(results.averageProfit()) << '\n';

    std::cout << "Average profit as % of initial bet: "
              << 100.0 * results.averageProfit() / INITIAL_BET
              << "%\n";
}

} // namespace

int main() {
    QLearningAgent qLearningAgent;

    // Load the trained Q-table. Do not retrain here.
    if (!qLearningAgent.loadQTable("data/q_table.txt")) {
        std::cerr
            << "Error: Could not load data/q_table.txt.\n"
            << "Run ./build/train first to train and save the agent.\n";
        return 1;
    }

    // Greedy policy: always choose the best learned action.
    qLearningAgent.setEpsilon(0.0);

    BasicStrategyAgent basicStrategyAgent;
    RandomAgent randomAgent;

    std::cout << "Evaluating agents over "
              << NUM_EVALUATION_ROUNDS
              << " rounds each...\n";

    const EvaluationResults qResults =
        evaluateAgent(qLearningAgent, NUM_EVALUATION_ROUNDS);

    const EvaluationResults basicResults =
        evaluateAgent(basicStrategyAgent, NUM_EVALUATION_ROUNDS);

    const EvaluationResults randomResults =
        evaluateAgent(randomAgent, NUM_EVALUATION_ROUNDS);

    printResults("Q-Learning Agent", qResults);
    printResults("Basic Strategy Agent", basicResults);
    printResults("Random Agent", randomResults);

    std::cout << "\n=== Average Profit Comparison ===\n";

    std::cout << "Q-learning: "
              << formatMoney(qResults.averageProfit()) << '\n';

    std::cout << "Basic strategy: "
              << formatMoney(basicResults.averageProfit()) << '\n';

    std::cout << "Random: "
              << formatMoney(randomResults.averageProfit()) << '\n';

    return 0;
}