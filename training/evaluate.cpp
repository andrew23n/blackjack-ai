#include "RandomAgent.h"
#include "Game.h"

#include <iomanip>
#include <iostream>

int main() {
    constexpr int NUM_ROUNDS = 100000;

    Game game;
    RandomAgent agent;

    int wins = 0;
    int losses = 0;
    int pushes = 0;

    for (int i = 0; i < NUM_ROUNDS; ++i) {
        game.startRound();

        // Play automatically until the round ends.
        while (!game.isGameOver()) {
            const Hand& playerHand = game.getPlayerHand();
            const Hand& dealerHand = game.getDealerHand();

            // The dealer's first card is the visible upcard.
            const Card& dealerUpcard =
                dealerHand.getCards().front();

            GameAction action = agent.chooseAction(
                playerHand,
                dealerUpcard
            );

            game.playAction(action);
        }

        // Record the result of this round.
        switch (game.getStatus()) {
            case GameStatus::PlayerWins:
                ++wins;
                break;

            case GameStatus::DealerWins:
                ++losses;
                break;

            case GameStatus::Push:
                ++pushes;
                break;

            default:
                break;
        }
    }

    const double winRate =
        100.0 * wins / NUM_ROUNDS;

    const double lossRate =
        100.0 * losses / NUM_ROUNDS;

    const double pushRate =
        100.0 * pushes / NUM_ROUNDS;

    // signifies the percentage of wins over losses
    const double averageReward =
        static_cast<double>(wins - losses) / NUM_ROUNDS;

    std::cout << std::fixed << std::setprecision(2);

    std::cout << "\n===== Blackjack Simulation =====\n";
    std::cout << "Agent: RandomAgent\n";
    std::cout << "Total rounds: " << NUM_ROUNDS << "\n\n";

    std::cout << "Wins: " << wins << '\n';
    std::cout << "Losses: " << losses << '\n';
    std::cout << "Pushes: " << pushes << "\n\n";

    std::cout << "Win rate: " << winRate << "%\n";
    std::cout << "Loss rate: " << lossRate << "%\n";
    std::cout << "Push rate: " << pushRate << "%\n";
    std::cout << "Average reward: " << averageReward << '\n';

    return 0;
}