#include "Championship.h"
#include "Helper/Copy.h"

#include <iostream>
#include <cstring>
#include <algorithm>

Championship::Championship(unsigned year)
    : year(year),
      teams(nullptr),
      teamsCount(0),
      teamsCapacity(0),
      schedule(nullptr),
      matchesPerRound(0),
      totalRounds(0),
      matches(nullptr),
      matchesCount(0),
      currentRound(0),
      finished(false),
      championName(nullptr),
      runnerUpName(nullptr),
      thirdName(nullptr)
{}

Championship::~Championship()
{
    free();
}

void Championship::free()
{
    for (unsigned i = 0; i < matchesCount; i++)
        delete matches[i];
    delete[] matches;

    delete[] schedule;

    for (unsigned i = 0; i < teamsCount; i++)
        delete teams[i];
    delete[] teams;

    delete[] championName;
    delete[] runnerUpName;
    delete[] thirdName;
}

void Championship::captureFinalStandings()
{
    delete[] championName; championName = nullptr;
    delete[] runnerUpName; runnerUpName = nullptr;
    delete[] thirdName;    thirdName = nullptr;

    if (Team* t = getChampion()) championName = copyString(t->getName());
    if (Team* t = getRunnerUp()) runnerUpName = copyString(t->getName());
    if (Team* t = getThird())    thirdName    = copyString(t->getName());
}

const char* Championship::getChampionName() const
{
    if (Team* t = getChampion())
        return t->getName();

    return championName;
}

const char* Championship::getRunnerUpName() const
{
    if (Team* t = getRunnerUp())
        return t->getName();

    return runnerUpName;
}

const char* Championship::getThirdName() const
{
    if (Team* t = getThird())
        return t->getName();

    return thirdName;
}

void Championship::resizeTeams()
{
    unsigned newCapacity = teamsCapacity == 0 ? 4 : teamsCapacity * 2;
    Team** newTeams = new Team*[newCapacity];

    for (unsigned i = 0; i < teamsCount; i++)
        newTeams[i] = teams[i];

    delete[] teams;
    teams = newTeams;
    teamsCapacity = newCapacity;
}

bool Championship::addTeam(Team* team)
{
    if (!team)
        return false;

    for (unsigned i = 0; i < teamsCount; i++)
    {
        if (strcmp(teams[i]->getName(), team->getName()) == 0)
            return false;
    }

    if (teamsCount >= teamsCapacity)
        resizeTeams();

    teams[teamsCount++] = team;
    return true;
}

bool Championship::validateTeamsCount() const
{
    return teamsCount >= 4 && teamsCount % 2 == 0;
}

void Championship::generateSchedule()
{
    unsigned n = teamsCount;
    matchesPerRound = n / 2;

    unsigned roundsInLeg = n - 1;
    totalRounds = 2 * roundsInLeg;

    unsigned totalFixtures = totalRounds * matchesPerRound;
    schedule = new Fixture[totalFixtures];

    unsigned* idx = new unsigned[n];
    for (unsigned i = 0; i < n; i++)
        idx[i] = i;

    for (unsigned r = 0; r < roundsInLeg; r++)
    {
        for (unsigned i = 0; i < matchesPerRound; i++)
        {
            unsigned t1 = idx[i];
            unsigned t2 = idx[n - 1 - i];

            Fixture f{};
            if (r % 2 == 0)
            {
                f.homeIndex = t1;
                f.awayIndex = t2;
            }
            else
            {
                f.homeIndex = t2;
                f.awayIndex = t1;
            }

            schedule[r * matchesPerRound + i] = f;
        }

        unsigned last = idx[n - 1];
        for (unsigned i = n - 1; i > 1; i--)
            idx[i] = idx[i - 1];
        idx[1] = last;
    }

    for (unsigned r = 0; r < roundsInLeg; r++)
    {
        for (unsigned i = 0; i < matchesPerRound; i++)
        {
            Fixture original = schedule[r * matchesPerRound + i];

            Fixture mirrored{};
            mirrored.homeIndex = original.awayIndex;
            mirrored.awayIndex = original.homeIndex;

            schedule[(roundsInLeg + r) * matchesPerRound + i] = mirrored;
        }
    }

    delete[] idx;
}

bool Championship::prepareSchedule()
{
    if (!validateTeamsCount())
        return false;

    generateSchedule();

    matches = new Match*[totalRounds * matchesPerRound];
    matchesCount = 0;

    return true;
}

bool Championship::startSeason()
{
    if (!prepareSchedule())
    {
        std::cout << "Cannot start season: need an even number of teams (at least 4), have "
                   << teamsCount << ".\n";
        return false;
    }

    currentRound = 0;
    finished = false;

    return true;
}

bool Championship::playRound()
{
    if (finished || currentRound >= totalRounds)
    {
        finished = true;
        return false;
    }

    std::cout << "\n===================================================\n";
    std::cout << "CHAMPIONSHIP " << year << " - Round " << (currentRound + 1)
               << " of " << totalRounds << "\n";
    std::cout << "===================================================\n";

    for (unsigned i = 0; i < matchesPerRound; i++)
    {
        const Fixture& f = schedule[currentRound * matchesPerRound + i];

        Match* m = new Match(*teams[f.homeIndex], *teams[f.awayIndex], currentRound + 1);
        m->play(currentRound + 1, totalRounds);

        matches[matchesCount++] = m;
    }

    currentRound++;

    if (currentRound >= totalRounds)
    {
        finished = true;
        std::cout << "\n*** Championship " << year << " has finished! ***\n";
        regulateAllSalaries();
        captureFinalStandings();
        printStandings();
        printTopScorers();
    }

    return true;
}

void Championship::playAllRounds()
{
    while (playRound())
    {
    }
}

bool Championship::playRoundAuto()
{
    if (finished || currentRound >= totalRounds)
    {
        finished = true;
        return false;
    }

    for (unsigned i = 0; i < matchesPerRound; i++)
    {
        const Fixture& f = schedule[currentRound * matchesPerRound + i];

        Match* m = new Match(*teams[f.homeIndex], *teams[f.awayIndex], currentRound + 1);
        m->playAuto();

        matches[matchesCount++] = m;
    }

    currentRound++;

    if (currentRound >= totalRounds)
    {
        finished = true;
        regulateAllSalaries();
        captureFinalStandings();
    }

    return true;
}

void Championship::playAllRoundsAuto()
{
    while (playRoundAuto())
    {
    }
}

void Championship::buildSortedStandings(Team** outSorted) const
{
    for (unsigned i = 0; i < teamsCount; i++)
        outSorted[i] = teams[i];

    std::sort(outSorted, outSorted + teamsCount, [](Team* a, Team* b)
    {
        if (a->getPoints() != b->getPoints())
            return a->getPoints() > b->getPoints();

        if (a->getGoalDifference() != b->getGoalDifference())
            return a->getGoalDifference() > b->getGoalDifference();

        return a->getGoalsScored() > b->getGoalsScored();
    });
}

void Championship::printStandings() const
{
    if (teamsCount == 0)
        return;

    Team** sorted = new Team*[teamsCount];
    buildSortedStandings(sorted);

    std::cout << "\n--- Standings (Championship " << year << ") ---\n";

    for (unsigned i = 0; i < teamsCount; i++)
    {
        Team* t = sorted[i];
        std::cout << (i + 1) << ". " << t->getName()
                   << " | Pts: " << t->getPoints()
                   << " | W-D-L: " << t->getWins() << "-" << t->getDraws() << "-" << t->getLosses()
                   << " | GD: " << t->getGoalDifference()
                   << " (" << t->getGoalsScored() << ":" << t->getGoalsConceded() << ")\n";
    }

    delete[] sorted;
}

Team* Championship::getChampion() const
{
    if (teamsCount == 0)
        return nullptr;

    Team** sorted = new Team*[teamsCount];
    buildSortedStandings(sorted);
    Team* result = sorted[0];
    delete[] sorted;
    return result;
}

Team* Championship::getRunnerUp() const
{
    if (teamsCount < 2)
        return nullptr;

    Team** sorted = new Team*[teamsCount];
    buildSortedStandings(sorted);
    Team* result = sorted[1];
    delete[] sorted;
    return result;
}

Team* Championship::getThird() const
{
    if (teamsCount < 3)
        return nullptr;

    Team** sorted = new Team*[teamsCount];
    buildSortedStandings(sorted);
    Team* result = sorted[2];
    delete[] sorted;
    return result;
}

void Championship::printTopScorers() const
{
    struct ScorerCandidate
    {
        Player* player;
        Team* team;
    };

    unsigned capacity = teamsCount * Team::MAX_PLAYERS;
    ScorerCandidate* candidates = new ScorerCandidate[capacity];
    unsigned candidatesCount = 0;

    for (unsigned t = 0; t < teamsCount; t++)
    {
        for (unsigned i = 0; i < Team::MAX_PLAYERS; i++)
        {
            Player* p = teams[t]->getPlayerByIndex(i);

            if (!p)
                break;

            if (p->getMatchesPlayed() >= 7)
                candidates[candidatesCount++] = ScorerCandidate{ p, teams[t] };
        }
    }

    if (candidatesCount == 0)
    {
        std::cout << "\nNo player has played at least 7 matches yet - "
                     "the top scorer cannot be determined.\n";
        delete[] candidates;
        return;
    }

    unsigned maxGoals = 0;
    for (unsigned i = 0; i < candidatesCount; i++)
        if (candidates[i].player->getScoredGoals() > maxGoals)
            maxGoals = candidates[i].player->getScoredGoals();

    unsigned minMatches = 0;
    bool minMatchesSet = false;
    for (unsigned i = 0; i < candidatesCount; i++)
    {
        if (candidates[i].player->getScoredGoals() != maxGoals)
            continue;

        if (!minMatchesSet || candidates[i].player->getMatchesPlayed() < minMatches)
        {
            minMatches = candidates[i].player->getMatchesPlayed();
            minMatchesSet = true;
        }
    }

    std::cout << "\n--- Top scorer(s) of Championship " << year << " ---\n";

    for (unsigned i = 0; i < candidatesCount; i++)
    {
        if (candidates[i].player->getScoredGoals() == maxGoals &&
            candidates[i].player->getMatchesPlayed() == minMatches)
        {
            std::cout << "  " << candidates[i].player->getName()
                       << " (" << candidates[i].team->getName() << ") - "
                       << maxGoals << " goals in " << minMatches << " matches\n";
        }
    }

    delete[] candidates;
}

unsigned Championship::getYear() const { return year; }
bool Championship::isFinished() const { return finished; }
unsigned Championship::getCurrentRound() const { return currentRound; }
unsigned Championship::getTotalRounds() const { return totalRounds; }
unsigned Championship::getTeamsCount() const { return teamsCount; }

Team* Championship::getTeam(unsigned index) const
{
    if (index >= teamsCount)
        return nullptr;

    return teams[index];
}

void Championship::regulateAllSalaries()
{
    for (unsigned i = 0; i < teamsCount; i++)
        teams[i]->regulateSalaries();
}

bool Championship::swapPlayers(unsigned teamIndexA, const char* playerNameA,
                                unsigned teamIndexB, const char* playerNameB)
{
    if (teamIndexA >= teamsCount || teamIndexB >= teamsCount || teamIndexA == teamIndexB)
        return false;

    Team* teamA = teams[teamIndexA];
    Team* teamB = teams[teamIndexB];

    const Player* pa = teamA->getPlayer(playerNameA);
    const Player* pb = teamB->getPlayer(playerNameB);

    if (!pa || !pb)
        return false;

    Player copyA(*pa);
    Player copyB(*pb);

    teamA->removePlayer(playerNameA);
    teamB->removePlayer(playerNameB);

    bool addedToA = teamA->addPlayer(copyB);
    bool addedToB = addedToA && teamB->addPlayer(copyA);

    if (!addedToA || !addedToB)
    {
        if (addedToA)
            teamA->removePlayer(copyB.getName());

        teamA->addPlayer(copyA);
        teamB->addPlayer(copyB);
        return false;
    }

    if (!teamA->isSquadValid() || !teamB->isSquadValid())
    {
        teamA->removePlayer(copyB.getName());
        teamB->removePlayer(copyA.getName());

        teamA->addPlayer(copyA);
        teamB->addPlayer(copyB);
        return false;
    }

    return true;
}

bool Championship::takeTeamsFrom(Championship& source)
{
    if (teamsCount != 0)
        return false;

    source.captureFinalStandings();

    for (unsigned i = 0; i < source.teamsCount; i++)
    {
        source.teams[i]->startNewSeason();

        if (!addTeam(source.teams[i]))
            return false;
    }

    source.teamsCount = 0;

    return true;
}

static void writeOptionalString(std::ostream& os, const char* s)
{
    size_t length = s ? strlen(s) : 0;
    os.write((const char*)(&length), sizeof(length));

    if (length)
        os.write(s, length);
}

static char* readOptionalString(std::istream& is)
{
    size_t length;
    is.read((char*)(&length), sizeof(length));

    if (!is || length == 0)
        return nullptr;

    char* buffer = new char[length + 1];
    is.read(buffer, length);
    buffer[length] = '\0';
    return buffer;
}

bool Championship::saveToFile(std::ostream& os) const
{
    os.write((const char*)(&year), sizeof(year));
    os.write((const char*)(&teamsCount), sizeof(teamsCount));

    for (unsigned i = 0; i < teamsCount; i++)
    {
        if (!teams[i]->saveTeam(os))
            return false;
    }

    os.write((const char*)(&currentRound), sizeof(currentRound));

    char finishedByte = finished ? 1 : 0;
    os.write(&finishedByte, sizeof(finishedByte));

    writeOptionalString(os, getChampionName());
    writeOptionalString(os, getRunnerUpName());
    writeOptionalString(os, getThirdName());

    return os.good();
}

Championship* Championship::loadFromFile(std::istream& is)
{
    unsigned year;
    is.read((char*)(&year), sizeof(year));

    if (!is)
        return nullptr;

    Championship* champ = new Championship(year);

    unsigned teamsCount;
    is.read((char*)(&teamsCount), sizeof(teamsCount));

    if (!is)
    {
        delete champ;
        return nullptr;
    }

    for (unsigned i = 0; i < teamsCount; i++)
    {
        Team* team = Team::loadTeam(is);

        if (!team || !champ->addTeam(team))
        {
            delete team;
            delete champ;
            return nullptr;
        }
    }

    unsigned currentRound;
    char finishedByte;

    is.read((char*)(&currentRound), sizeof(currentRound));
    is.read(&finishedByte, sizeof(finishedByte));

    char* loadedChampionName = readOptionalString(is);
    char* loadedRunnerUpName = readOptionalString(is);
    char* loadedThirdName    = readOptionalString(is);

    bool scheduleOk = (champ->teamsCount == 0) || champ->prepareSchedule();

    if (!is || !scheduleOk)
    {
        delete[] loadedChampionName;
        delete[] loadedRunnerUpName;
        delete[] loadedThirdName;
        delete champ;
        return nullptr;
    }

    champ->currentRound = currentRound;
    champ->finished = (finishedByte != 0);

    champ->championName = loadedChampionName;
    champ->runnerUpName = loadedRunnerUpName;
    champ->thirdName = loadedThirdName;

    return champ;
}