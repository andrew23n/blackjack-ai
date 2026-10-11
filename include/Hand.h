#ifndef HAND_H
#define HAND_H

#include "Card.h"

#include <vector>

class Hand {
public:
    // Adds a card to the hand.
    void addCard(const Card& card);

    // Removes all cards from the hand.
    void clear();

    // Returns the best valid Blackjack total.
    int getTotal() const;

    // Returns true if the total exceeds 21.
    bool isBust() const;

    // Returns true for a two-card natural Blackjack.
    bool isBlackjack() const;

    // Returns true if an Ace currently counts as 11.
    bool hasUsableAce() const;

    // Returns the number of cards in the hand.
    std::size_t size() const;

    // Returns true if the hand has no cards.
    bool isEmpty() const;

    // Provides read-only access to the cards.
    const std::vector<Card>& getCards() const;

private:
    std::vector<Card> cards;
};

#endif // HAND_H