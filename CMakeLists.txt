cmake_minimum_required(VERSION 4.3)
project(FootballChampionship)

set(CMAKE_CXX_STANDARD 20)

add_executable(FootballChampionship main.cpp
        Player.h
        Teams/Team.h
        Lineup.h
        Match.h
        Championship.h
        ChampionshipCollection.h
        Player.cpp
        Teams/AttackingTeam.h
        Teams/AttackingTeam.cpp
        Teams/DefensiveTeam.h
        Teams/DefensiveTeam.cpp
        Teams/BalancedTeam.h
        Teams/BalancedTeam.cpp
        Teams/Team.cpp
        Helper/Copy.h
        Helper/Input.h
        Lineup.cpp
        Match.cpp
        Championship.cpp
        ChampionshipCollection.cpp
        Helper/Menu.h
)
