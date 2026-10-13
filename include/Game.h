#ifndef GAME_H
#define GAME_H

#include "Deck.h"
#include "Hand.h"

#include <cstddef>
#include <vector>

enum class GameAction {
    Hit,
    Stand,
    DoubleDown,
    Split
};

enum class GameStatus {
    NotStarted,
    InProgress,
    PlayerWins,
    DealerWins,
    Push
};

struct PlayerHandState {
    Hand hand;
    double wager = 0.0;
    bool finished = false;
    bool doubled = false;
    bool splitAces = false;
    bool naturalEligible = true;
};

class Game {
public:
    explicit Game(double startingBankroll = 1000.0);

    void startRound(double initialBet = 10.0);
    void playAction(GameAction action);
    void playDealerTurn();

    GameStatus getStatus() const;
    const Hand& getPlayerHand() const;
    const Hand& getPlayerHand(std::size_t index) const;
    const Hand& getDealerHand() const;

    std::size_t getPlayerHandCount() const;
    std::size_t getActiveHandIndex() const;
    double getPlayerWager(std::size_t index) const;

    double getBankroll() const;
    double getRoundProfit() const;

    bool isGameOver() const;

private:
    Deck deck;
    std::vector<PlayerHandState> playerHands;
    Hand dealerHand;
    std::size_t activeHandIndex = 0;

    GameStatus status;
    double bankroll;
    double roundStartingBankroll;
    double roundProfit;
    bool hasSplit = false;
    bool dealerHasBlackjack = false;

    void checkInitialBlackjack();
    void advanceToNextHand();
    void determineWinner();
    void settleInitialBlackjack();
    void settleRound();
    void finishRound();
    bool canDoubleDown() const;
    bool canSplit() const;
};

#endif // GAME_H