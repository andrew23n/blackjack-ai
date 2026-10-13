# Blackjack AI: Reinforcement Learning Agent

An object-oriented C++ Blackjack simulator featuring a Q-learning agent that learns Hit/Stand decisions through reinforcement learning, alongside basic strategy and random baseline agents.

## Features

- **Blackjack Simulator:** Card, deck, hand, and game logic with betting, bankroll tracking, and configurable agent behavior.
- **Q-Learning Agent:** Learns Hit/Stand policies using a Q-table, epsilon-greedy exploration, and reward-based updates.
- **Baseline Agents:** Basic strategy and random agents for performance comparison.
- **Automated Evaluation:** Simulates thousands of rounds and tracks win rates, losses, pushes, and net profit.
- **Q-Table Persistence:** Saves learned Q-values to `data/q_table.txt` for reuse without retraining.

## Tech Stack

- **Language:** C++
- **Algorithms:** Q-Learning, epsilon-greedy exploration, Monte Carlo simulation

## Initial Results

Results from one 100,000-round evaluation per agent:

| Metric | Q-Learning | Basic Strategy | Random |
|---|---:|---:|---:|
| Win rate | 42.60% | 43.38% | 30.06% |
| Loss rate | 48.99% | 48.01% | 65.97% |
| Push rate | 8.41% | 8.61% | 3.97% |
| Average profit/round | -$0.42 | -$0.23 | -$3.36 |

The Q-learning agent achieved a **12.54 percentage-point higher win rate than random play** in this evaluation. Results may vary between runs.

## Project Structure

├── app/             # Playable Blackjack application
├── include/         # Class and agent headers
├── src/             # Game engine and agent implementations
├── training/
│   ├── train.cpp    # Q-learning training
│   └── evaluate.cpp # Agent evaluation
├── data/            # Saved Q-table
└── CMakeLists.txt

## Current Limitations

- The Q-learning agent supports Hit and Stand only.
- The state representation uses player total, usable Ace status, and dealer upcard.
- Performance depends on training and evaluation conditions.
- Basic strategy can use additional actions that the Q-learning agent does not currently learn.

## Future Improvements

- Expand the action space to support doubling and splitting.
- Track state-action visits and training progress.
- Compare multiple training runs and random seeds.
- Improve Q-table persistence and evaluation reproducibility.