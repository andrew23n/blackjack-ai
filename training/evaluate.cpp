#include "BasicStrategyAgent.h"
#include "RandomAgent.h"
#include "Game.h"

#include <cmath>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

namespace {

constexpr int NUM_ROUNDS = 100'000;
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
        output << "-$" << std::fixed << std::setprecision(2)
               << std::abs(amount);
    } else {
        output << "$" << std::fixed << std::setprecision(2)
               << amount;
    }

    return output.str();
}

GameAction fallbackHitOrStand(
    const Hand& hand,
    const Card& dealerUpcard
) {
    const int total = hand.getTotal();
    const int dealer = dealerUpcard.getValue();

    if (hand.hasUsableAce()) {
        if (total >= 19) {
            return GameAction::Stand;
        }

        if (total == 18) {
            return (dealer >= 2 && dealer <= 8)
                ? GameAction::Stand
                : GameAction::Hit;
        }

        return GameAction::Hit;
    }

    if (total >= 17) {
        return GameAction::Stand;
    }

    if (total <= 11) {
        return GameAction::Hit;
    }

    if (total == 12) {
        return (dealer >= 4 && dealer <= 6)
            ? GameAction::Stand
            : GameAction::Hit;
    }

    if (total <= 16) {
        return (dealer >= 2 && dealer <= 6)
            ? GameAction::Stand
            : GameAction::Hit;
    }

    return GameAction::Stand;
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

            GameAction action =
                agent.chooseAction(hand, dealerUpcard);

            // Allow only one split per round.
            // Do not allow doubling after splitting.
            if (game.getPlayerHandCount() > 1 &&
                (action == GameAction::Split ||
                 action == GameAction::DoubleDown)) {
                action = fallbackHitOrStand(
                    hand,
                    dealerUpcard
                );
            }

            game.playAction(action);
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
    const char* agentName,
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
              << 100.0 * results.wins / rounds
              << "%)\n";

    std::cout << "Losses: " << results.losses << " ("
              << 100.0 * results.losses / rounds
              << "%)\n";

    std::cout << "Pushes: " << results.pushes << " ("
              << 100.0 * results.pushes / rounds
              << "%)\n";

    std::cout << "Total net profit: "
              << formatMoney(results.totalProfit) << '\n';

    std::cout << "Average profit per round: "
              << formatMoney(results.averageProfit()) << '\n';

    std::cout << "Starting bankroll: "
              << formatMoney(INITIAL_BANKROLL) << '\n';

    std::cout << "Ending bankroll: "
              << formatMoney(
                     INITIAL_BANKROLL + results.totalProfit
                 ) << '\n';
}

} // namespace

int main() {
    BasicStrategyAgent basicStrategy;
    RandomAgent randomAgent;

    const EvaluationResults basicResults =
        evaluateAgent(basicStrategy, NUM_ROUNDS);

    const EvaluationResults randomResults =
        evaluateAgent(randomAgent, NUM_ROUNDS);

    std::cout << "Blackjack Agent Evaluation\n";
    std::cout << "Rounds per agent: " << NUM_ROUNDS << '\n';
    std::cout << "Initial bet: "
              << formatMoney(INITIAL_BET) << '\n';
    std::cout << "Starting bankroll per agent: "
              << formatMoney(INITIAL_BANKROLL) << '\n';

    printResults("Basic Strategy Agent", basicResults);
    printResults("Random Agent", randomResults);

    std::cout << "\n=== Comparison ===\n";

    std::cout << "Basic strategy average profit: "
              << formatMoney(basicResults.averageProfit())
              << '\n';

    std::cout << "Random agent average profit: "
              << formatMoney(randomResults.averageProfit())
              << '\n';

    const double profitDifference =
        basicResults.averageProfit()
        - randomResults.averageProfit();

    std::cout << "Average profit difference: "
              << formatMoney(profitDifference)
              << " per round\n";

    return 0;
}