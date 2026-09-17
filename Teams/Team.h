#pragma once
#include "../Player.h"
#include <iosfwd>

class Team {
public:
    Team(const char* name, const char* coach, const char* stadium, double budget);
    virtual ~Team();

    Team(const Team&) = delete;
    Team& operator=(const Team&) = delete;

    bool addPlayer(const Player& player);
    bool removePlayer(const char* playerName);
    bool canBuyPlayer(const Player& player) const;
    void updateStatistics(unsigned scored, unsigned conceded);
    void startNewSeason();
    void regulateSalaries();
    bool isSquadValid() const;

    virtual void print() const;
    Player* getPlayer(const char* name);
    const Player* getPlayer(const char* name) const;
    Player* getPlayerByIndex(unsigned index);
    const Player* getPlayerByIndex(unsigned index) const;
    const char* getName() const;

    unsigned getWins() const;
    unsigned getDraws() const;
    unsigned getLosses() const;
    unsigned getPoints() const;
    unsigned getGoalsScored() const;
    unsigned getGoalsConceded() const;
    int getGoalDifference() const;

    bool saveTeam(std::ostream& os) const;
    static Team* loadTeam(std::istream& is);

    static const unsigned MAX_PLAYERS = 20;
protected:

    Player* players[MAX_PLAYERS]{};
    unsigned playersCount;

    char* teamName;
    char* coachName;
    char* stadiumName;

    double budget;

    unsigned wins;
    unsigned draws;
    unsigned losses;
    unsigned points;

    unsigned goalsScored;
    unsigned goalsConceded;

    bool hasPlayerName(const char* name) const;
    bool hasPlayerNumber(unsigned number) const;

    unsigned countPosition(Position position) const;

    virtual bool satisfiesComposition() const = 0;
    virtual bool canBuySpecificPlayer(const Player& player) const = 0;
    virtual char getTypeCode() const = 0;

    void free();
};