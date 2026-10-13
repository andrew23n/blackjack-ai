#ifndef BASIC_STRATEGY_AGENT_H
#define BASIC_STRATEGY_AGENT_H

#include "Agent.h"

class BasicStrategyAgent : public Agent {
public:
    GameAction chooseAction(
        const Hand& playerHand,
        const Card& dealerUpcard
    ) override;
};

#endif // BASIC_STRATEGY_AGENT_H