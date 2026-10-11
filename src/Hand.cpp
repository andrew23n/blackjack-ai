#include "Hand.h"

#include <cstddef>

// Adds a card without modifying the original card.
void Hand::addCard(const Card& card) {
    cards.push_back(card);
}

// Removes every card from the hand.
void Hand::clear() {
    cards.clear();
}

// Calculates the best Blackjack total.
// Aces initially count as 11, then become 1 if necessary.
int Hand::getTotal() const {
    int total = 0;
    int aceCount = 0;

    for (const Card& card : cards) {
        total += card.getValue();

        if (card.isAce()) {
            ++aceCount;
        }
    }

    // Each adjustment reduces the total by 10.
    while (total > 21 && aceCount > 0) {
        total -= 10;
        --aceCount;
    }

    return total;
}

// Checks whether the best total exceeds 21.
bool Hand::isBust() const {
    return getTotal() > 21;
}

// A natural Blackjack must consist of exactly two cards.
bool Hand::isBlackjack() const {
    return cards.size() == 2 && getTotal() == 21;
}

// Checks whether an Ace can still count as 11.
bool Hand::hasUsableAce() const {
    int total = 0;
    int aceCount = 0;

    for (const Card& card : cards) {
        total += card.getValue();

        if (card.isAce()) {
            ++aceCount;
        }
    }

    while (total > 21 && aceCount > 0) {
        total -= 10;
        --aceCount;
    }

    // If an Ace remains valued at 11, it is usable.
    return aceCount > 0;
}

// Returns the number of cards held.
std::size_t Hand::size() const {
    return cards.size();
}

// Checks whether the hand contains no cards.
bool Hand::isEmpty() const {
    return cards.empty();
}

// Returns a read-only reference to the cards.
const std::vector<Card>& Hand::getCards() const {
    return cards;
}