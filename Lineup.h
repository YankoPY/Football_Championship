#ifndef FOOTBALLCHAMPIONSHIP_LINEUP_H
#define FOOTBALLCHAMPIONSHIP_LINEUP_H

#include "Player.h"

class Team;

class Lineup
{
public:
    Lineup(Team& team);

    void choosePlayers();
    void chooseAutomatically();
    bool isValid() const;
    void playMatch();
    void includePlayersWithFewMatches();

    Player* getPlayer(const char* name) const;
    Player* getPlayerByIndex(unsigned index) const;

    unsigned getPlayersCount() const;

private:
    static const unsigned MAX_PLAYERS = 11;

    Team* team;
    Player* players[MAX_PLAYERS]{};
    unsigned playersCount;

    bool containsPlayer(const char* name) const;
    bool hasPosition(Position position) const;
    unsigned countPosition(Position position) const;

    Player* findBestCandidate(bool used[]) const;
    bool isValidAfterReplacement(unsigned lineupIndex, const Player* newPlayer) const;
};

#endif