#include "RandomAgent.h"

RandomAgent::RandomAgent()
    : randomEngine(std::random_device{}()),
      actionDistribution(0, 1) {}

GameAction RandomAgent::chooseAction(
    const Hand& playerHand,
    const Card& dealerUpcard
) {
    // tells compilers not to use these parameters since the random agent will play blindly
    (void) playerHand;
    (void) dealerUpcard;

    if (actionDistribution(randomEngine) == 0) {
        return GameAction::Hit;
    }

    return GameAction::Stand;
}
