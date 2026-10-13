#include "BasicStrategyAgent.h"

namespace {

int dealerValue(const Card& card) {
    return card.getValue();
}

bool isPair(const Hand& hand) {
    const auto& cards = hand.getCards();

    return cards.size() == 2 &&
           cards[0].getRank() == cards[1].getRank();
}

bool shouldSplitPair(
    const Hand& hand,
    const Card& dealerUpcard
) {
    if (!isPair(hand)) {
        return false;
    }

    const int rank = hand.getCards()[0].getRank();
    const int dealer = dealerValue(dealerUpcard);

    // Always split Aces and 8s.
    if (rank == 1 || rank == 8) {
        return true;
    }

    // Never split 10-value cards or 5s.
    if (rank >= 10 || rank == 5) {
        return false;
    }

    // Split 2s and 3s against dealer 2-7.
    if (rank == 2 || rank == 3) {
        return dealer >= 2 && dealer <= 7;
    }

    // With no double after split, do not split 4s.
    if (rank == 4) {
        return false;
    }

    // Split 6s against dealer 2-6.
    if (rank == 6) {
        return dealer >= 2 && dealer <= 6;
    }

    // Split 7s against dealer 2-7.
    if (rank == 7) {
        return dealer >= 2 && dealer <= 7;
    }

    // Split 9s against 2-6 and 8-9.
    if (rank == 9) {
        return (dealer >= 2 && dealer <= 6) ||
               dealer == 8 || dealer == 9;
    }

    return false;
}

bool shouldDoubleDown(
    const Hand& hand,
    const Card& dealerUpcard
) {
    if (hand.size() != 2) {
        return false;
    }

    const int total = hand.getTotal();
    const int dealer = dealerValue(dealerUpcard);

    if (hand.hasUsableAce()) {
        // Soft 13-14: double against 5-6.
        if ((total == 13 || total == 14) &&
            dealer >= 5 && dealer <= 6) {
            return true;
        }

        // Soft 15-16: double against 4-6.
        if ((total == 15 || total == 16) &&
            dealer >= 4 && dealer <= 6) {
            return true;
        }

        // Soft 17-18: double against 3-6.
        if ((total == 17 || total == 18) &&
            dealer >= 3 && dealer <= 6) {
            return true;
        }

        return false;
    }

    // Hard 9: double against 3-6.
    if (total == 9) {
        return dealer >= 3 && dealer <= 6;
    }

    // Hard 10: double against 2-9.
    if (total == 10) {
        return dealer >= 2 && dealer <= 9;
    }

    // Hard 11: double against 2-10.
    if (total == 11) {
        return dealer >= 2 && dealer <= 10;
    }

    return false;
}

GameAction chooseHitOrStand(
    const Hand& hand,
    const Card& dealerUpcard
) {
    const int total = hand.getTotal();
    const int dealer = dealerValue(dealerUpcard);

    if (hand.hasUsableAce()) {
        // Soft 19 or higher: stand.
        if (total >= 19) {
            return GameAction::Stand;
        }

        // Soft 18: stand against 2-8, otherwise hit.
        if (total == 18) {
            return (dealer >= 2 && dealer <= 8)
                ? GameAction::Stand
                : GameAction::Hit;
        }

        return GameAction::Hit;
    }

    // Hard hands.
    if (total >= 17) {
        return GameAction::Stand;
    }

    if (total <= 11) {
        return GameAction::Hit;
    }

    // Hard 12: stand against 4-6.
    if (total == 12) {
        return (dealer >= 4 && dealer <= 6)
            ? GameAction::Stand
            : GameAction::Hit;
    }

    // Hard 13-16: stand against 2-6.
    if (total <= 16) {
        return (dealer >= 2 && dealer <= 6)
            ? GameAction::Stand
            : GameAction::Hit;
    }

    return GameAction::Stand;
}

} // namespace

GameAction BasicStrategyAgent::chooseAction(
    const Hand& playerHand,
    const Card& dealerUpcard
) {
    if (shouldSplitPair(playerHand, dealerUpcard)) {
        return GameAction::Split;
    }

    if (shouldDoubleDown(playerHand, dealerUpcard)) {
        return GameAction::DoubleDown;
    }

    return chooseHitOrStand(playerHand, dealerUpcard);
}