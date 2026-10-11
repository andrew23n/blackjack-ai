#include "Deck.h"

#include <algorithm>
#include <numeric>
#include <stdexcept>

// Creates and shuffles a deck using a random seed.
Deck::Deck()
    : randomEngine(std::random_device{}()) {
    initialize();
    shuffle();
}

// Creates a deck with a specified seed.
Deck::Deck(unsigned int seed)
    : randomEngine(seed) {
    initialize();
    shuffle();
}

// Creates all 52 standard playing cards.
void Deck::initialize() {
    cards.clear();
    cards.reserve(52);

    for (int suit = 0; suit < 4; ++suit) {
        for (int rank = 1; rank <= 13; ++rank) {
            cards.emplace_back(
                rank,
                static_cast<Suit>(suit)
            );
        }
    }
}

// Restores the deck and shuffles it.
void Deck::reset() {
    initialize();
    shuffle();
}

// Randomizes the order of the cards.
void Deck::shuffle() {
    std::shuffle(
        cards.begin(),
        cards.end(),
        randomEngine
    );
}

// Deals one card from the top of the deck.
Card Deck::dealCard() {
    if (cards.empty()) {
        throw std::out_of_range(
            "Cannot deal a card from an empty deck."
        );
    }

    Card card = cards.back();
    cards.pop_back();

    return card;
}

// Returns the number of cards remaining.
std::size_t Deck::cardsRemaining() const {
    return cards.size();
}

// Checks whether the deck is empty.
bool Deck::isEmpty() const {
    return cards.empty();
}