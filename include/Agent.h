#ifndef AGENT_H
#define AGENT_H

#include "Card.h"
#include "Hand.h"
#include "Game.h"

class Agent {
public:
    virtual ~Agent() = default;

    virtual GameAction chooseAction(
        const Hand& playerHand,
        const Card& dealerUpcard
    ) = 0;
};

#endif // AGENT_H
