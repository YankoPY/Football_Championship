#ifndef FOOTBALLCHAMPIONSHIP_CHAMPIONSHIP_H
#define FOOTBALLCHAMPIONSHIP_CHAMPIONSHIP_H

#include "Teams/Team.h"
#include "Match.h"
#include <iosfwd>

struct Fixture
{
    unsigned homeIndex;
    unsigned awayIndex;
};

class Championship
{
public:
    explicit Championship(unsigned year);

    Championship(const Championship&) = delete;
    Championship& operator=(const Championship&) = delete;

    ~Championship();

    bool addTeam(Team* team);
    bool validateTeamsCount() const;

    bool startSeason();

    bool playRound();
    void playAllRounds();

    bool playRoundAuto();
    void playAllRoundsAuto();

    void printStandings() const;
    Team* getChampion() const;
    Team* getRunnerUp() const;
    Team* getThird() const;

    const char* getChampionName() const;
    const char* getRunnerUpName() const;
    const char* getThirdName() const;

    void printTopScorers() const;
    void regulateAllSalaries();

    bool swapPlayers(unsigned teamIndexA, const char* playerNameA, unsigned teamIndexB, const char* playerNameB);

    bool takeTeamsFrom(Championship& source);

    bool saveToFile(std::ostream& os) const;
    static Championship* loadFromFile(std::istream& is);

    unsigned getYear() const;
    bool isFinished() const;
    unsigned getCurrentRound() const;
    unsigned getTotalRounds() const;
    unsigned getTeamsCount() const;
    Team* getTeam(unsigned index) const;

private:
    unsigned year;

    Team** teams;
    unsigned teamsCount;
    unsigned teamsCapacity;

    Fixture* schedule;
    unsigned matchesPerRound;
    unsigned totalRounds;

    Match** matches;
    unsigned matchesCount;

    unsigned currentRound;
    bool finished;

    char* championName;
    char* runnerUpName;
    char* thirdName;

    void resizeTeams();
    void generateSchedule();
    void captureFinalStandings();

    bool prepareSchedule();

    void buildSortedStandings(Team** outSorted) const;
    void free();

};

#endif //FOOTBALLCHAMPIONSHIP_CHAMPIONSHIP_H