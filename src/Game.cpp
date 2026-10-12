#include "Game.h"

#include <stdexcept>

// Creates a game with a new deck.
Game::Game()
    : deck(),
      playerHand(),
      dealerHand(),
      status(GameStatus::NotStarted) {
}

// Starts a new round.
void Game::startRound() {
    if (status == GameStatus::InProgress) {
        throw std::logic_error(
            "Cannot start a new round while a game is in progress."
        );
    }

    // Reshuffle if there are not enough cards for a new round.
    if (deck.cardsRemaining() < 4) {
        deck.reset();
    }

    // Clear the previous hands.
    playerHand = Hand{};
    dealerHand = Hand{};

    status = GameStatus::InProgress;

    // Deal two cards to each side, alternating.
    playerHand.addCard(deck.dealCard());
    dealerHand.addCard(deck.dealCard());

    playerHand.addCard(deck.dealCard());
    dealerHand.addCard(deck.dealCard());

    checkInitialBlackjack();
}

// Handles natural Blackjack after the initial deal.
void Game::checkInitialBlackjack() {
    bool playerBlackjack = playerHand.isBlackjack();
    bool dealerBlackjack = dealerHand.isBlackjack();

    if (playerBlackjack && dealerBlackjack) {
        status = GameStatus::Push;
    } else if (playerBlackjack) {
        status = GameStatus::PlayerWins;
    } else if (dealerBlackjack) {
        status = GameStatus::DealerWins;
    }
}

// Executes a player action.
void Game::playAction(GameAction action) {
    if (status != GameStatus::InProgress) {
        throw std::logic_error(
            "Cannot play an action when the round is not in progress."
        );
    }

    switch (action) {
        case GameAction::Hit:
            playerHand.addCard(deck.dealCard());

            // The player loses immediately if they bust.
            if (playerHand.isBust()) {
                status = GameStatus::DealerWins;
            }
            break;

        case GameAction::Stand:
            playDealerTurn();
            determineWinner();
            break;

        default:
            throw std::invalid_argument("Invalid game action.");
    }
}

// Plays the dealer's turn.
// This implementation uses the common rule:
// dealer hits below 17 and stands on all 17s.
void Game::playDealerTurn() {
    if (status != GameStatus::InProgress) {
        throw std::logic_error(
            "Dealer cannot play when the round is not in progress."
        );
    }

    while (dealerHand.getTotal() < 17) {
        dealerHand.addCard(deck.dealCard());
    }
}

// Determines the winner after the dealer finishes.
void Game::determineWinner() {
    if (playerHand.isBust()) {
        status = GameStatus::DealerWins;
        return;
    }

    if (dealerHand.isBust()) {
        status = GameStatus::PlayerWins;
        return;
    }

    int playerTotal = playerHand.getTotal();
    int dealerTotal = dealerHand.getTotal();

    if (playerTotal > dealerTotal) {
        status = GameStatus::PlayerWins;
    } else if (playerTotal < dealerTotal) {
        status = GameStatus::DealerWins;
    } else {
        status = GameStatus::Push;
    }
}

// Returns the current round status.
GameStatus Game::getStatus() const {
    return status;
}

// Returns the player's hand.
const Hand& Game::getPlayerHand() const {
    return playerHand;
}

// Returns the dealer's hand.
const Hand& Game::getDealerHand() const {
    return dealerHand;
}

// Checks whether the round is finished.
bool Game::isGameOver() const {
    return status != GameStatus::InProgress;
}