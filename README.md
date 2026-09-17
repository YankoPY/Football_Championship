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
--- Standings (Championship 2030) ---
1. Team1 | Pts: 15 | W-D-L: 5-0-1 | GD: 10 (15:5)
2. Team3 | Pts: 11 | W-D-L: 3-2-1 | GD: 5 (17:12)
3. Team4 | Pts: 4 | W-D-L: 1-1-4 | GD: -8 (11:19)
4. Team2 | Pts: 3 | W-D-L: 0-3-3 | GD: -7 (15:22)
```

## Tech

- **Language:** C++20
- **Build system:** CMake
- **Testing:** custom unit + integration test suite
