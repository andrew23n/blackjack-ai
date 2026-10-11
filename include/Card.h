#ifndef CARD_H
#define CARD_H

#include <string>

enum class Suit {
    Hearts,
    Diamonds,
    Clubs,
    Spades
};

class Card {
public:
    // Constructs a card with a rank and suit.
    // Rank: 1 = Ace, 2-10 = number cards,
    //       11 = Jack, 12 = Queen, 13 = King.
    Card(int rank, Suit suit);

    // Returns the card's rank and suit.
    int getRank() const;
    Suit getSuit() const;

    // Returns the card's Blackjack value.
    // Ace initially counts as 11.
    // Jack, Queen, and King count as 10.
    int getValue() const;

    // Returns true if the card is an Ace.
    bool isAce() const;

    // Returns a readable card name.
    // Example: "Ace of Spades".
    std::string toString() const;

private:
    int rank;
    Suit suit;

    // Converts rank and suit into readable strings.
    std::string rankToString() const;
    std::string suitToString() const;
};

#endif // CARD_H