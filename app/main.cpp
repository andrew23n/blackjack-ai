#include "Game.h"

#include <iomanip>
#include <iostream>
#include <limits>

void displayPlayerHand(const Game& game, std::size_t index) {
    const Hand& hand = game.getPlayerHand(index);

    std::cout << "Player hand: ";

    const auto& cards = hand.getCards();

    for (std::size_t i = 0; i < cards.size(); ++i) {
        if (i > 0) {
            std::cout << " | ";
        }

        std::cout << cards[i].toString();
    }

    std::cout << "\nTotal: " << hand.getTotal();

    if (hand.isBust()) {
        std::cout << " (BUST)";
    }

    std::cout << '\n';
}

void displayDealerHand(const Hand& hand, bool hideHoleCard) {
    const auto& cards = hand.getCards();

    std::cout << "\nDealer hand: ";

    for (std::size_t i = 0; i < cards.size(); ++i) {
        if (i > 0) {
            std::cout << " | ";
        }

        if (hideHoleCard && i == 1) {
            std::cout << "[Hidden card]";
        } else {
            std::cout << cards[i].toString();
        }
    }

    std::cout << '\n';

    if (hideHoleCard) {
        std::cout << "Total: ?\n";
    } else {
        std::cout << "Total: " << hand.getTotal();

        if (hand.isBust()) {
            std::cout << " (BUST)";
        }

        std::cout << '\n';
    }
}

bool isPair(const Hand& hand) {
    const auto& cards = hand.getCards();

    return cards.size() == 2 &&
           cards[0].getRank() == cards[1].getRank();
}

bool canDoubleDown(const Game& game, std::size_t index) {
    const Hand& hand = game.getPlayerHand(index);

    return hand.size() == 2 &&
           game.getBankroll() >= game.getPlayerWager(index);
}

int main() {
    Game game(1000.0);

    constexpr double INITIAL_BET = 10.0;
    char playAgain = 'y';

    std::cout << "===== Blackjack =====\n";
    std::cout << "Starting bankroll: $"
              << std::fixed << std::setprecision(2)
              << game.getBankroll() << "\n";

    while (playAgain == 'y' || playAgain == 'Y') {
        if (game.getBankroll() < INITIAL_BET) {
            std::cout << "\nInsufficient funds to place the initial bet.\n";
            break;
        }

        game.startRound(INITIAL_BET);

        while (!game.isGameOver()) {
            const std::size_t index = game.getActiveHandIndex();
            const Hand& playerHand = game.getPlayerHand(index);

            std::cout << "\n------------------------------\n";

            if (game.getPlayerHandCount() > 1) {
                std::cout << "Playing hand " << index + 1
                          << " of " << game.getPlayerHandCount() << '\n';
            }

            displayPlayerHand(game, index);

            std::cout << "Bet: $" << game.getPlayerWager(index) << '\n';
            displayDealerHand(game.getDealerHand(), true);

            std::cout << "\nBankroll: $" << game.getBankroll() << '\n';

            std::cout << "\nAvailable actions:\n";
            std::cout << "  1. Hit\n";
            std::cout << "  2. Stand\n";

            // Doubling is allowed only on the initial two-card hand.
            if (canDoubleDown(game, index)) {
                std::cout << "  3. Double down\n";
            }

            // Splitting requires a pair and sufficient funds.
            if (isPair(playerHand) &&
                game.getPlayerHandCount() == 1 &&
                game.getBankroll() >= game.getPlayerWager(index)) {
                std::cout << "  4. Split\n";
            }

            std::cout << "Choose an action: ";

            int choice;

            if (!(std::cin >> choice)) {
                if (std::cin.eof()) {
                    return 0;
                }

                std::cin.clear();
                std::cin.ignore(
                    std::numeric_limits<std::streamsize>::max(), '\n'
                );

                std::cout << "Please enter a valid number.\n";
                continue;
            }

            GameAction action;

            switch (choice) {
                case 1:
                    action = GameAction::Hit;
                    break;

                case 2:
                    action = GameAction::Stand;
                    break;

                case 3:
                    if (!canDoubleDown(game, index)) {
                        std::cout << "Doubling down is not available.\n";
                        continue;
                    }
                    action = GameAction::DoubleDown;
                    break;

                case 4:
                    if (!isPair(playerHand) ||
                        game.getPlayerHandCount() != 1 ||
                        game.getBankroll() < game.getPlayerWager(index)) {
                        std::cout << "Splitting is not available.\n";
                        continue;
                    }
                    action = GameAction::Split;
                    break;

                default:
                    std::cout << "Choose one of the displayed options.\n";
                    continue;
            }

            try {
                game.playAction(action);
            } catch (const std::exception& error) {
                std::cout << "Action unavailable: "
                          << error.what() << '\n';
            }
        }

        std::cout << "\n========== Final Result ==========\n";

        for (std::size_t i = 0;
             i < game.getPlayerHandCount();
             ++i) {
            std::cout << "\n";

            if (game.getPlayerHandCount() > 1) {
                std::cout << "Hand " << i + 1 << ":\n";
            }

            displayPlayerHand(game, i);
            std::cout << "Bet: $" << game.getPlayerWager(i) << '\n';
        }

        displayDealerHand(game.getDealerHand(), false);

        std::cout << "\nRound profit: $" << game.getRoundProfit() << '\n';
        std::cout << "Bankroll: $" << game.getBankroll() << '\n';

        std::cout << "\nPlay another round? (y/n): ";
        std::cin >> playAgain;
    }

    std::cout << "\nFinal bankroll: $" << game.getBankroll() << '\n';

    return 0;
}