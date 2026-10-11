#ifndef DECK_H
#define DECK_H

#include "Card.h"

#include <cstddef>
#include <random>
#include <vector>

class Deck {
public:
    // Creates a standard 52-card deck and shuffles it.
    Deck();

    // Creates a deck using a specified random seed.
    // Useful for reproducible tests and AI simulations.
    explicit Deck(unsigned int seed);

    // Restores all 52 cards and shuffles the deck.
    void reset();

    // Shuffles the remaining cards.
    void shuffle();

    // Deals one card from the deck.
    // Throws std::out_of_range if the deck is empty.
    Card dealCard();

    // Returns the number of cards remaining.
    std::size_t cardsRemaining() const;

    // Returns true if there are no cards left.
    bool isEmpty() const;

private:
    std::vector<Card> cards;
    std::mt19937 randomEngine;

    // Creates all 52 cards.
    void initialize();
};

#endif // DECK_H