#ifndef GAME_H
#define GAME_H

#include "Deck.h"
#include "Hand.h"

enum class GameAction {
    Hit,
    Stand
};

enum class GameStatus {
    NotStarted,
    InProgress,
    PlayerWins,
    DealerWins,
    Push
};

class Game {
public:
    Game();

    // Starts a new round and deals two cards to each side.
    void startRound();

    // Executes the player's chosen action.
    void playAction(GameAction action);

    // Plays the dealer's turn according to Blackjack rules.
    void playDealerTurn();

    // Returns the current round status.
    GameStatus getStatus() const;

    // Returns the player's and dealer's hands.
    const Hand& getPlayerHand() const;
    const Hand& getDealerHand() const;

    // Returns true when the round has finished.
    bool isGameOver() const;

private:
    Deck deck;
    Hand playerHand;
    Hand dealerHand;
    GameStatus status;

    // Determines the outcome after both turns are complete.
    void determineWinner();

    // Handles natural Blackjack after the initial deal.
    void checkInitialBlackjack();
};

#endif // GAME_H