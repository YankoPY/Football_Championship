#include "Match.h"
#include "Teams/Team.h"
#include "Helper/Input.h"

#include <iostream>
#include <cstdlib>

static unsigned readGoals(const char* teamName)
{
    unsigned goals;

    while (true)
    {
        std::cout << teamName << " goals: ";
        std::cin >> goals;

        if (std::cin.eof())
        {
            std::cout << "\n[End of input - exiting.]\n";
            std::exit(0);
        }

        if (std::cin.fail())
        {
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            std::cout << "Invalid number, try again.\n";
            continue;
        }

        std::cin.ignore(10000, '\n');
        return goals;
    }
}

Match::Match(Team& home, Team& away, unsigned round)
    : homeTeam(&home),
      awayTeam(&away),
      homeLineup(home),
      awayLineup(away),
      homeGoals(0),
      awayGoals(0),
      round(round),
      goals(nullptr),
      goalsCount(0),
      goalsCapacity(0)
{}

Match::Match(const Match& other)
    : homeTeam(other.homeTeam),
      awayTeam(other.awayTeam),
      homeLineup(other.homeLineup),
      awayLineup(other.awayLineup),
      homeGoals(other.homeGoals),
      awayGoals(other.awayGoals),
      round(other.round),
      goals(nullptr),
      goalsCount(other.goalsCount),
      goalsCapacity(other.goalsCount)
{
    if (goalsCount > 0)
    {
        goals = new Player*[goalsCount];

        for (unsigned i = 0; i < goalsCount; i++)
            goals[i] = other.goals[i];
    }
}

Match& Match::operator=(const Match& other)
{
    if (this != &other)
    {
        Player** newGoals = nullptr;

        if (other.goalsCount > 0)
        {
            newGoals = new Player*[other.goalsCount];

            for (unsigned i = 0; i < other.goalsCount; i++)
                newGoals[i] = other.goals[i];
        }

        delete[] goals;

        homeTeam = other.homeTeam;
        awayTeam = other.awayTeam;
        homeLineup = other.homeLineup;
        awayLineup = other.awayLineup;
        homeGoals = other.homeGoals;
        awayGoals = other.awayGoals;
        round = other.round;
        goals = newGoals;
        goalsCount = other.goalsCount;
        goalsCapacity = other.goalsCount;
    }

    return *this;
}

Match::~Match()
{
    delete[] goals;
}

void Match::resizeGoals()
{
    unsigned newCapacity = goalsCapacity == 0 ? 4 : goalsCapacity * 2;
    Player** newGoals = new Player*[newCapacity];

    for (unsigned i = 0; i < goalsCount; i++)
        newGoals[i] = goals[i];

    delete[] goals;
    goals = newGoals;
    goalsCapacity = newCapacity;
}

void Match::chooseLineups(unsigned currentRound, unsigned totalRounds)
{
    std::cout << "\n--- Lineup for " << homeTeam->getName() << " (home) ---\n";

    do
    {
        homeLineup.choosePlayers();
    } while (!homeLineup.isValid());

    if (totalRounds > 0 && currentRound + 3 > totalRounds)
    {
        std::cout << "\nChecking for players who still need matches to reach 3 played...\n";
        homeLineup.includePlayersWithFewMatches();
    }

    std::cout << "\n--- Lineup for " << awayTeam->getName() << " (away) ---\n";

    do
    {
        awayLineup.choosePlayers();
    } while (!awayLineup.isValid());

    if (totalRounds > 0 && currentRound + 3 > totalRounds)
    {
        std::cout << "\nChecking for players who still need matches to reach 3 played...\n";
        awayLineup.includePlayersWithFewMatches();
    }
}

void Match::enterResult()
{
    std::cout << "\nEnter result for " << homeTeam->getName()
               << " vs " << awayTeam->getName() << ":\n";

    homeGoals = readGoals(homeTeam->getName());
    awayGoals = readGoals(awayTeam->getName());

    unsigned totalGoals = homeGoals + awayGoals;
    unsigned homeAssigned = 0;
    unsigned awayAssigned = 0;

    for (unsigned i = 0; i < totalGoals; i++)
    {
        std::cout << "\nGoal " << (i + 1) << " of " << totalGoals << ":\n";

        bool homeAvailable = homeAssigned < homeGoals;
        bool awayAvailable = awayAssigned < awayGoals;
        bool isHomeGoal;

        if (homeAvailable && awayAvailable)
        {
            char teamChoice;

            while (true)
            {
                std::cout << "Which team scored - (H)ome " << homeTeam->getName()
                           << " or (A)way " << awayTeam->getName() << "? ";
                teamChoice = readChar();

                if (teamChoice == 'H' || teamChoice == 'h')
                {
                    isHomeGoal = true;
                    break;
                }

                if (teamChoice == 'A' || teamChoice == 'a')
                {
                    isHomeGoal = false;
                    break;
                }

                std::cout << "Invalid choice.\n";
            }
        }
        else
        {
            isHomeGoal = homeAvailable;
        }

        Lineup& scoringLineup = isHomeGoal ? homeLineup : awayLineup;

        Player* scorer = nullptr;

        while (!scorer)
        {
            char name[128];

            std::cout << "Scorer name: ";
            readLine(name, 128);

            scorer = scoringLineup.getPlayer(name);

            if (!scorer)
                std::cout << "This player is not part of the selected lineup.\n";
        }

        scorer->scoreGoal();

        if (goalsCount >= goalsCapacity)
            resizeGoals();

        goals[goalsCount++] = scorer;

        if (isHomeGoal)
            homeAssigned++;
        else
            awayAssigned++;
    }

    homeTeam->updateStatistics(homeGoals, awayGoals);
    awayTeam->updateStatistics(awayGoals, homeGoals);
}

void Match::play(unsigned currentRound, unsigned totalRounds)
{
    std::cout << "\n=========================================\n";
    std::cout << "Round " << round << ": " << homeTeam->getName()
               << " (home) vs " << awayTeam->getName() << " (away)\n";
    std::cout << "=========================================\n";

    chooseLineups(currentRound, totalRounds);
    enterResult();

    homeLineup.playMatch();
    awayLineup.playMatch();

    std::cout << "\nResult: " << homeTeam->getName() << " " << homeGoals
               << " - " << awayGoals << " " << awayTeam->getName() << "\n";
}

unsigned Match::getRound() const { return round; }
unsigned Match::getHomeGoals() const { return homeGoals; }
unsigned Match::getAwayGoals() const { return awayGoals; }
const Team* Match::getHomeTeam() const { return homeTeam; }
const Team* Match::getAwayTeam() const { return awayTeam; }

void Match::playAuto()
{
    homeLineup.chooseAutomatically();
    awayLineup.chooseAutomatically();

    homeGoals = rand() % 5;
    awayGoals = rand() % 5;

    unsigned totalGoals = homeGoals + awayGoals;
    unsigned homeAssigned = 0;
    unsigned awayAssigned = 0;

    for (unsigned i = 0; i < totalGoals; i++)
    {
        bool homeAvailable = homeAssigned < homeGoals;
        bool awayAvailable = awayAssigned < awayGoals;
        bool isHomeGoal = homeAvailable && (!awayAvailable || (rand() % 2 == 0));

        Lineup& scoringLineup = isHomeGoal ? homeLineup : awayLineup;
        unsigned count = scoringLineup.getPlayersCount();

        if (count > 0)
        {
            Player* scorer = scoringLineup.getPlayerByIndex(rand() % count);

            if (scorer)
            {
                scorer->scoreGoal();

                if (goalsCount >= goalsCapacity)
                    resizeGoals();

                goals[goalsCount++] = scorer;
            }
        }

        if (isHomeGoal)
            homeAssigned++;
        else
            awayAssigned++;
    }

    homeLineup.playMatch();
    awayLineup.playMatch();

    homeTeam->updateStatistics(homeGoals, awayGoals);
    awayTeam->updateStatistics(awayGoals, homeGoals);
}