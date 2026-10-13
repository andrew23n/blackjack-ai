#include "BasicStrategyAgent.h"

GameAction BasicStrategyAgent::chooseAction(
    const Hand& playerHand,
    const Card& dealerUpcard
) {
    const int playerTotal = playerHand.getTotal();
    const int dealerValue = dealerUpcard.getValue();

    // A soft hand contains an Ace currently counted as 11.
    if (playerHand.hasUsableAce()) {
        // Soft 19 or higher: stand.
        if (playerTotal >= 19) {
            return GameAction::Stand;
        }

        // Soft 18: stand against dealer 2-8.
        // Hit against dealer 9, 10, or Ace.
        if (playerTotal == 18) {
            if (dealerValue >= 9) {
                return GameAction::Hit;
            }

            return GameAction::Stand;
        }

        // Soft 17 or lower: hit.
        return GameAction::Hit;
    }

    // Hard hands: no usable Ace counted as 11.
    if (playerTotal >= 17) {
        return GameAction::Stand;
    }

    if (playerTotal <= 11) {
        return GameAction::Hit;
    }

    if (playerTotal == 12) {
        // Stand against dealer 4, 5, or 6.
        if (dealerValue >= 4 && dealerValue <= 6) {
            return GameAction::Stand;
        }

        return GameAction::Hit;
    }

    // Hard 13-16: stand against dealer 2-6.
    if (playerTotal <= 16) {
        if (dealerValue >= 2 && dealerValue <= 6) {
            return GameAction::Stand;
        }

        return GameAction::Hit;
    }

    return GameAction::Stand;
}