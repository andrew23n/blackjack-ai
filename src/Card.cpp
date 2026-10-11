#include "Card.h"

#include <stdexcept>
#include <string>

// Constructs a card and validates its rank.
Card::Card(int rank, Suit suit)
    : rank(rank), suit(suit) {

    if (rank < 1 || rank > 13) {
        throw std::invalid_argument(
            "Card rank must be between 1 and 13."
        );
    }

    switch (suit) {
        case Suit::Hearts:
        case Suit::Diamonds:
        case Suit::Clubs:
        case Suit::Spades:
            break;

        default:
            throw std::invalid_argument(
                "Invalid card suit."
            );
    }
}

// Returns the card's rank.
int Card::getRank() const {
    return rank;
}

// Returns the card's suit.
Suit Card::getSuit() const {
    return suit;
}

// Returns the card's Blackjack value.
int Card::getValue() const {
    if (rank == 1) {
        return 11;
    }

    if (rank >= 11) {
        return 10;
    }

    return rank;
}

// Checks whether the card is an Ace.
bool Card::isAce() const {
    return rank == 1;
}

// Converts the rank into a readable string.
std::string Card::rankToString() const {
    switch (rank) {
        case 1:
            return "Ace";
        case 11:
            return "Jack";
        case 12:
            return "Queen";
        case 13:
            return "King";
        default:
            return std::to_string(rank);
    }
}

// Converts the suit into a readable string.
std::string Card::suitToString() const {
    switch (suit) {
        case Suit::Hearts:
            return "Hearts";
        case Suit::Diamonds:
            return "Diamonds";
        case Suit::Clubs:
            return "Clubs";
        case Suit::Spades:
            return "Spades";
        default:
            throw std::logic_error(
                "Invalid card suit."
            );
    }
}

// Returns the complete card name.
std::string Card::toString() const {
    return rankToString() + " of " + suitToString();
}
