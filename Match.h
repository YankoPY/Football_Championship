#ifndef FOOTBALLCHAMPIONSHIP_MATCH_H
#define FOOTBALLCHAMPIONSHIP_MATCH_H
#include "Lineup.h"

class Match
{
public:
    Match(Team& home, Team& away, unsigned round);
    Match(const Match& other);
    Match& operator=(const Match& other);
    ~Match();

    void chooseLineups(unsigned currentRound, unsigned totalRounds);

    void enterResult();

    void play(unsigned currentRound, unsigned totalRounds);
    void playAuto();

    unsigned getRound() const;
    unsigned getHomeGoals() const;
    unsigned getAwayGoals() const;
    const Team* getHomeTeam() const;
    const Team* getAwayTeam() const;

private:
    Team* homeTeam;
    Team* awayTeam;

    Lineup homeLineup;
    Lineup awayLineup;

    unsigned homeGoals;
    unsigned awayGoals;

    unsigned round;

    Player** goals;
    unsigned goalsCount;
    unsigned goalsCapacity;

    void resizeGoals();

};

#endif //FOOTBALLCHAMPIONSHIP_MATCH_H