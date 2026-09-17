#include "Team.h"
#include "AttackingTeam.h"
#include "DefensiveTeam.h"
#include "BalancedTeam.h"
#include "../Helper/Copy.h"
#include <cstring>
#include <iostream>

Team::Team(const char* name, const char* coach, const char* stadium, double budget)
    : teamName(copyString(name)), coachName(nullptr), stadiumName(nullptr), playersCount(0), budget(budget), wins(0), draws(0), losses(0), points(0), goalsScored(0), goalsConceded(0)
{
    try {
        coachName = copyString(coach);
        stadiumName = copyString(stadium);
    }catch (...) {
        free();
        throw;
    }
}

Team::~Team()
{
    free();
}

bool Team::hasPlayerName(const char* name) const
{
    for(unsigned i = 0; i < playersCount; i++)
    {
        if(strcmp(players[i]->getName(), name) == 0)
            return true;
    }

    return false;
}

bool Team::hasPlayerNumber(unsigned number) const
{
    for(unsigned i = 0; i < playersCount; i++)
    {
        if(players[i]->getNumber() == number)
            return true;
    }

    return false;
}

unsigned Team::countPosition(Position position) const
{
    unsigned count = 0;

    for(unsigned i = 0; i < playersCount; i++)
    {
        if(players[i]->getPosition() == position)
            count++;
    }

    return count;
}

bool Team::canBuyPlayer(const Player& player) const
{
    if(playersCount >= MAX_PLAYERS)
        return false;

    if(player.getTransferValue() > budget)
        return false;

    if(hasPlayerName(player.getName()))
        return false;

    if(hasPlayerNumber(player.getNumber()))
        return false;

    if(player.getPosition() == Position::Goalkeeper && countPosition(Position::Goalkeeper) >= 2)
        return false;

    return canBuySpecificPlayer(player);
}

bool Team::addPlayer(const Player& player)
{
    if(!canBuyPlayer(player))
        return false;

    players[playersCount++] = new Player(player);

    budget -= player.getTransferValue();

    return true;
}

bool Team::removePlayer(const char* playerName)
{
    int index = -1;

    for(unsigned i = 0; i < playersCount; i++)
    {
        if(strcmp(players[i]->getName(), playerName) == 0)
        {
            index = i;
            break;
        }
    }

    if(index == -1)
        return false;

    double refund = players[index]->getTransferValue();
    delete players[index];

    for(unsigned i = index; i < playersCount - 1; i++)
        players[i] = players[i + 1];

    playersCount--;
    budget += refund;

    return true;
}

void Team::updateStatistics(unsigned scored, unsigned conceded)
{
    goalsScored += scored;
    goalsConceded += conceded;

    if(scored > conceded)
    {
        wins++;
        points += 3;
    }
    else if(scored == conceded)
    {
        draws++;
        points += 1;
    }
    else
    {
        losses++;
    }
}

void Team::startNewSeason()
{
    wins = 0;
    draws = 0;
    losses = 0;
    points = 0;

    goalsScored = 0;
    goalsConceded = 0;
}

void Team::regulateSalaries()
{
    unsigned totalMatches = wins + draws + losses;

    for (unsigned i = 0; i < playersCount; i++)
        players[i]->salaryRegulation(totalMatches);
}

bool Team::isSquadValid() const
{
    return satisfiesComposition();
}

void Team::print() const
{
    std::cout << "Team: " << teamName << '\n';
    std::cout << "Coach: " << coachName << '\n';
    std::cout << "Stadium: " << stadiumName << '\n';

    std::cout << "Budget: " << budget << '\n';

    std::cout << "Wins: " << wins << '\n';
    std::cout << "Draws: " << draws << '\n';
    std::cout << "Losses: " << losses << '\n';

    std::cout << "Points: " << points << '\n';

    std::cout << "Goals scored: " << goalsScored << '\n';

    std::cout << "Goals conceded: " << goalsConceded << '\n';

    std::cout << "Players:\n";

    for(unsigned i = 0; i < playersCount; i++)
    {
        players[i]->print();
        std::cout << '\n';
    }
}

const Player* Team::getPlayer(const char* name) const {
    for(unsigned i = 0; i < playersCount; i++)
        if(strcmp(players[i]->getName(), name) == 0)
            return players[i];
    return nullptr;
}

Player* Team::getPlayer(const char* name) {
    for(unsigned i = 0; i < playersCount; i++)
        if(strcmp(players[i]->getName(), name) == 0)
            return players[i];
    return nullptr;
}

Player* Team::getPlayerByIndex(unsigned index)
{
    if (index >= playersCount)
        return nullptr;

    return players[index];
}

const Player* Team::getPlayerByIndex(unsigned index) const
{
    if (index >= playersCount)
        return nullptr;

    return players[index];
}

const char* Team::getName() const
{
    return teamName;
}

unsigned Team::getWins() const { return wins; }
unsigned Team::getDraws() const { return draws; }
unsigned Team::getLosses() const { return losses; }
unsigned Team::getPoints() const { return points; }
unsigned Team::getGoalsScored() const { return goalsScored; }
unsigned Team::getGoalsConceded() const { return goalsConceded; }

int Team::getGoalDifference() const
{
    return (int)(goalsScored) - (int)(goalsConceded);
}

void Team::free() {
    delete[] teamName;
    delete[] coachName;
    delete[] stadiumName;

    for(unsigned i = 0; i < playersCount; i++)
        delete players[i];
}

static void writeString(std::ostream& os, const char* s)
{
    size_t length = s ? strlen(s) : 0;
    os.write((const char*)(&length), sizeof(length));

    if (length)
        os.write(s, length);
}

static void readString(std::istream& is, char* buffer, size_t bufferSize)
{
    size_t length;
    is.read((char*)(&length), sizeof(length));

    if (!is)
    {
        buffer[0] = '\0';
        return;
    }

    if (length >= bufferSize)
        length = bufferSize - 1;

    if (length)
        is.read(buffer, length);

    buffer[length] = '\0';
}

bool Team::saveTeam(std::ostream& os) const
{
    char type = getTypeCode();
    os.write(&type, sizeof(type));

    writeString(os, teamName);
    writeString(os, coachName);
    writeString(os, stadiumName);

    os.write((const char*)(&budget), sizeof(budget));

    os.write((const char*)(&wins), sizeof(wins));
    os.write((const char*)(&draws), sizeof(draws));
    os.write((const char*)(&losses), sizeof(losses));
    os.write((const char*)(&points), sizeof(points));
    os.write((const char*)(&goalsScored), sizeof(goalsScored));
    os.write((const char*)(&goalsConceded), sizeof(goalsConceded));

    os.write((const char*)(&playersCount), sizeof(playersCount));

    for (unsigned i = 0; i < playersCount; i++)
        players[i]->savePlayer(os);

    return os.good();
}

Team* Team::loadTeam(std::istream& is)
{
    char type;
    is.read(&type, sizeof(type));

    if (!is)
        return nullptr;

    char name[128];
    char coach[128];
    char stadium[128];

    readString(is, name, sizeof(name));
    readString(is, coach, sizeof(coach));
    readString(is, stadium, sizeof(stadium));

    double budget;
    is.read((char*)(&budget), sizeof(budget));

    if (!is)
        return nullptr;

    Team* team;

    switch (type)
    {
        case 'A': team = new AttackingTeam(name, coach, stadium, budget); break;
        case 'D': team = new DefensiveTeam(name, coach, stadium, budget); break;
        case 'B': team = new BalancedTeam(name, coach, stadium, budget); break;
        default:  return nullptr;
    }

    is.read((char*)(&team->wins), sizeof(team->wins));
    is.read((char*)(&team->draws), sizeof(team->draws));
    is.read((char*)(&team->losses), sizeof(team->losses));
    is.read((char*)(&team->points), sizeof(team->points));
    is.read((char*)(&team->goalsScored), sizeof(team->goalsScored));
    is.read((char*)(&team->goalsConceded), sizeof(team->goalsConceded));

    unsigned savedPlayersCount;
    is.read((char*)(&savedPlayersCount), sizeof(savedPlayersCount));

    if (!is)
    {
        delete team;
        return nullptr;
    }

    unsigned playersToLoad = savedPlayersCount < MAX_PLAYERS ? savedPlayersCount : MAX_PLAYERS;

    for (unsigned i = 0; i < playersToLoad; i++)
        team->players[i] = new Player(Player::readPlayer(is));

    team->playersCount = playersToLoad;

    if (!is)
    {
        delete team;
        return nullptr;
    }

    return team;
}