#include "Card.h"
#include "Hand.h"
#include "Game.h"

#include <iostream>
#include <limits>
#include <string>

void displayHand(const std::string& name, const Hand& hand) {
    std::cout << name << ": ";

    for (const Card& card : hand.getCards()) {
        std::cout << card.toString() << " | ";
    }

    std::cout << "\nTotal: " << hand.getTotal() << "\n";
}

void displayDealerHand(const Hand& hand, bool hideSecondCard) {
    std::cout << "Dealer: ";

    const auto& cards = hand.getCards();

    for (std::size_t i = 0; i < cards.size(); ++i) {
        if (hideSecondCard && i == 1) {
            std::cout << "[Hidden card] | ";
        } else {
            std::cout << cards[i].toString() << " | ";
        }
    }

    std::cout << '\n';

    if (!hideSecondCard) {
        std::cout << "Dealer total: " << hand.getTotal() << '\n';
    }
}

void displayResult(GameStatus status) {
    std::cout << "\n========== RESULT ==========\n";

    switch (status) {
        case GameStatus::PlayerWins:
            std::cout << "You win!\n";
            break;

        case GameStatus::DealerWins:
            std::cout << "Dealer wins.\n";
            break;

        case GameStatus::Push:
            std::cout << "Push! It's a tie.\n";
            break;

        default:
            std::cout << "Round is still in progress.\n";
            break;
    }
}

int main() {
    Game game;
    char playAgain = 'y';

    std::cout << "============================\n";
    std::cout << "      BLACKJACK GAME\n";
    std::cout << "============================\n";

    while (playAgain == 'y' || playAgain == 'Y') {
        try {
            game.startRound();

            std::cout << "\n--- New Round ---\n";
            displayHand("Your hand", game.getPlayerHand());

            // Show the dealer's upcard while hiding the hole card.
            displayDealerHand(game.getDealerHand(), true);

            if (game.isGameOver()) {
                std::cout << "\nBlackjack detected!\n";
                displayHand("Your final hand", game.getPlayerHand());
                displayDealerHand(game.getDealerHand(), false);
                displayResult(game.getStatus());
            } else {
                while (!game.isGameOver()) {
                    std::cout << "\nChoose an action:\n";
                    std::cout << "1. Hit\n";
                    std::cout << "2. Stand\n";
                    std::cout << "Enter choice: ";

                    int choice;

                    if (!(std::cin >> choice)) {
                        if (std::cin.eof()) {
                            std::cout << "\nExiting game.\n";
                            return 0;
                        }

                        std::cin.clear();
                        std::cin.ignore(
                            std::numeric_limits<std::streamsize>::max(),
                            '\n'
                        );

                        std::cout << "Invalid input. Enter 1 or 2.\n";
                        continue;
                    }

                    if (choice != 1 && choice != 2) {
                        std::cout << "Invalid choice. Enter 1 or 2.\n";
                        continue;
                    }

                    if (choice == 1) {
                        game.playAction(GameAction::Hit);

                        std::cout << "\nYou hit!\n";
                        displayHand("Your hand", game.getPlayerHand());

                        if (game.isGameOver()) {
                            std::cout << "You busted!\n";
                        }
                    } else {
                        game.playAction(GameAction::Stand);

                        std::cout << "\nYou stand.\n";
                        break;
                    }

                    if (!game.isGameOver()) {
                        displayDealerHand(game.getDealerHand(), true);
                    }
                }

                displayHand("Your final hand", game.getPlayerHand());
                displayDealerHand(game.getDealerHand(), false);
                displayResult(game.getStatus());
            }
        } catch (const std::exception& e) {
            std::cerr << "Game error: " << e.what() << '\n';
            return 1;
        }

        std::cout << "\nPlay another round? (y/n): ";

        if (!(std::cin >> playAgain)) {
            break;
        }
    }

    std::cout << "\nThanks for playing!\n";
    return 0;
}