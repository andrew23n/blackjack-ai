#include "Game.h"

#include <cmath>
#include <stdexcept>

Game::Game(double startingBankroll)
    : deck(),
      status(GameStatus::NotStarted),
      bankroll(startingBankroll),
      roundStartingBankroll(startingBankroll),
      roundProfit(0.0) {
    if (!std::isfinite(startingBankroll) || startingBankroll < 0.0) {
        throw std::invalid_argument("Invalid starting bankroll.");
    }
}

void Game::startRound(double initialBet) {
    if (status == GameStatus::InProgress) {
        throw std::logic_error("A round is already in progress.");
    }

    if (!std::isfinite(initialBet) ||
        initialBet <= 0.0 ||
        initialBet > bankroll) {
        throw std::invalid_argument("Invalid initial bet.");
    }

    // Reshuffle between rounds, never during a hand.
    if (deck.cardsRemaining() <= 78) {
        deck.reset();
    }

    roundStartingBankroll = bankroll;
    roundProfit = 0.0;
    bankroll -= initialBet;

    playerHands.clear();
    dealerHand.clear();
    activeHandIndex = 0;
    hasSplit = false;
    dealerHasBlackjack = false;
    status = GameStatus::InProgress;

    PlayerHandState initialHand;
    initialHand.wager = initialBet;
    playerHands.push_back(initialHand);

    playerHands[0].hand.addCard(deck.dealCard());
    dealerHand.addCard(deck.dealCard());
    playerHands[0].hand.addCard(deck.dealCard());
    dealerHand.addCard(deck.dealCard());

    checkInitialBlackjack();
}

void Game::checkInitialBlackjack() {
    const bool playerBlackjack = playerHands[0].hand.isBlackjack();
    dealerHasBlackjack = dealerHand.isBlackjack();

    if (playerBlackjack || dealerHasBlackjack) {
        settleInitialBlackjack();
    }
}

void Game::settleInitialBlackjack() {
    PlayerHandState& player = playerHands[0];

    if (player.hand.isBlackjack() && dealerHasBlackjack) {
        // Return the original wager: a push.
        bankroll += player.wager;
    } else if (player.hand.isBlackjack()) {
        // Return the wager plus 3:2 winnings.
        bankroll += player.wager * 2.5;
    }
    // If only the dealer has Blackjack, the player's wager is lost.

    finishRound();
}

bool Game::canDoubleDown() const {
    if (activeHandIndex >= playerHands.size()) {
        return false;
    }

    const PlayerHandState& player = playerHands[activeHandIndex];

    return !player.finished &&
           player.hand.size() == 2 &&
           !player.doubled &&
           !player.splitAces &&
           bankroll >= player.wager;
}

bool Game::canSplit() const {
    if (activeHandIndex >= playerHands.size() || hasSplit) {
        return false;
    }

    const PlayerHandState& player = playerHands[activeHandIndex];
    const auto& cards = player.hand.getCards();

    return !player.finished &&
           cards.size() == 2 &&
           cards[0].getRank() == cards[1].getRank() &&
           bankroll >= player.wager;
}

void Game::playAction(GameAction action) {
    if (status != GameStatus::InProgress) {
        throw std::logic_error("No round is in progress.");
    }

    if (activeHandIndex >= playerHands.size()) {
        throw std::logic_error("No active player hand.");
    }

    PlayerHandState& player = playerHands[activeHandIndex];

    if (player.finished) {
        throw std::logic_error("The active hand is already finished.");
    }

    switch (action) {
        case GameAction::Hit:
            if (player.splitAces) {
                throw std::logic_error(
                    "Split Aces can receive only one additional card."
                );
            }

            player.hand.addCard(deck.dealCard());

            if (player.hand.isBust()) {
                player.finished = true;
                advanceToNextHand();
            }
            break;

        case GameAction::Stand:
            player.finished = true;
            advanceToNextHand();
            break;

        case GameAction::DoubleDown:
            if (!canDoubleDown()) {
                throw std::logic_error("Double down is not allowed.");
            }

            // Match the existing wager, then receive exactly one card.
            bankroll -= player.wager;
            player.wager *= 2.0;
            player.doubled = true;
            player.hand.addCard(deck.dealCard());
            player.finished = true;
            advanceToNextHand();
            break;

        case GameAction::Split: {
            if (!canSplit()) {
                throw std::logic_error("Splitting is not allowed.");
            }

            const double splitWager = player.wager;
            const auto cards = player.hand.getCards();
            const Card firstCard = cards[0];
            const Card secondCard = cards[1];
            const bool splittingAces = firstCard.isAce();

            // Pay for the second hand.
            bankroll -= splitWager;

            PlayerHandState firstHand;
            firstHand.wager = splitWager;
            firstHand.naturalEligible = false;
            firstHand.splitAces = splittingAces;
            firstHand.hand.addCard(firstCard);

            PlayerHandState secondHand;
            secondHand.wager = splitWager;
            secondHand.naturalEligible = false;
            secondHand.splitAces = splittingAces;
            secondHand.hand.addCard(secondCard);

            // Replace the current hand with the first split hand,
            // and insert the second hand immediately after it.
            playerHands[activeHandIndex] = firstHand;
            playerHands.insert(
                playerHands.begin() +
                    static_cast<std::ptrdiff_t>(activeHandIndex + 1),
                secondHand
            );

            hasSplit = true;

            // Deal one new card to each split hand.
            playerHands[activeHandIndex].hand.addCard(deck.dealCard());
            playerHands[activeHandIndex + 1].hand.addCard(deck.dealCard());

            // Simplified split-Ace rule: one card per Ace hand.
            if (splittingAces) {
                playerHands[activeHandIndex].finished = true;
                playerHands[activeHandIndex + 1].finished = true;
                advanceToNextHand();
            }
            break;
        }
    }
}

void Game::advanceToNextHand() {
    while (activeHandIndex < playerHands.size() &&
           playerHands[activeHandIndex].finished) {
        ++activeHandIndex;
    }

    if (activeHandIndex >= playerHands.size()) {
        playDealerTurn();
        determineWinner();
    }
}

void Game::playDealerTurn() {
    if (status != GameStatus::InProgress) {
        return;
    }

    // No dealer draws are needed if every player hand has busted.
    bool anyLiveHand = false;

    for (const PlayerHandState& player : playerHands) {
        if (!player.hand.isBust()) {
            anyLiveHand = true;
            break;
        }
    }

    if (!anyLiveHand) {
        return;
    }

    while (dealerHand.getTotal() < 17) {
        dealerHand.addCard(deck.dealCard());
    }
}

void Game::determineWinner() {
    if (status != GameStatus::InProgress) {
        return;
    }

    settleRound();
}

void Game::settleRound() {
    const int dealerTotal = dealerHand.getTotal();
    const bool dealerBust = dealerHand.isBust();

    for (const PlayerHandState& player : playerHands) {
        const int playerTotal = player.hand.getTotal();

        if (player.hand.isBust()) {
            continue;
        }

        if (dealerHasBlackjack) {
            // Initial naturals were settled before player actions.
            continue;
        }

        if (dealerBust || playerTotal > dealerTotal) {
            bankroll += player.wager * 2.0;
        } else if (playerTotal == dealerTotal) {
            bankroll += player.wager;
        }
        // Otherwise the wager is lost.
    }

    finishRound();
}

void Game::finishRound() {
    roundProfit = bankroll - roundStartingBankroll;

    if (roundProfit > 0.000001) {
        status = GameStatus::PlayerWins;
    } else if (roundProfit < -0.000001) {
        status = GameStatus::DealerWins;
    } else {
        status = GameStatus::Push;
    }
}

GameStatus Game::getStatus() const {
    return status;
}

const Hand& Game::getPlayerHand() const {
    if (playerHands.empty()) {
        throw std::logic_error("No player hand is available.");
    }

    const std::size_t index =
        activeHandIndex < playerHands.size()
            ? activeHandIndex
            : playerHands.size() - 1;

    return playerHands[index].hand;
}

const Hand& Game::getPlayerHand(std::size_t index) const {
    return playerHands.at(index).hand;
}

const Hand& Game::getDealerHand() const {
    return dealerHand;
}

std::size_t Game::getPlayerHandCount() const {
    return playerHands.size();
}

std::size_t Game::getActiveHandIndex() const {
    return activeHandIndex;
}

double Game::getPlayerWager(std::size_t index) const {
    return playerHands.at(index).wager;
}

double Game::getBankroll() const {
    return bankroll;
}

double Game::getRoundProfit() const {
    return roundProfit;
}

bool Game::isGameOver() const {
    return status != GameStatus::InProgress;
}