# Football Championship Simulator

A C++20 console application that simulates a football championship from fixture generation through to final standings, built from scratch with a full object-oriented design.

## Features

- OOP class hierarchy with a `Team` base class and `AttackingTeam`, `DefensiveTeam`, and `BalancedTeam` variants
- Round-robin scheduling for fixture generation
- File-based persistence — save and load tournament state
- Tiebreaker and constraint logic for standings calculation
- Full unit test suite, plus scripted smoke/integration tests
- Compiles cleanly under strict warnings (`-Wall -Wextra -std=c++20`)

## Project structure

```
.
├── Helper/
│   ├── Copy.h
│   ├── Input.h
│   └── Menu.h
├── Teams/
│   ├── Team.h
│   ├── Team.cpp
│   ├── AttackingTeam.h / .cpp
│   ├── DefensiveTeam.h / .cpp
│   └── BalancedTeam.h / .cpp
├── Championship.h / .cpp
├── ChampionshipCollection.h / .cpp
├── Lineup.h / .cpp
├── Match.h / .cpp
├── Player.h / .cpp
├── main.cpp
└── CMakeLists.txt
```

## Build & run

```bash
cmake -S . -B build              # configure the project
cmake --build build               # build the project
./build/FootballChampionship      # run the simulator
```

## Example output

```
Round 12 complete.

Standings:
1. Team A     P12  W9  D2  L1   Pts 29
2. Team B     P12  W8  D3  L1   Pts 27
3. Team C     P12  W6  D4  L2   Pts 22
...
```

> Replace this with real output from an actual run.

## Tech

- **Language:** C++20
- **Build system:** CMake
- **Testing:** custom unit + integration test suite
