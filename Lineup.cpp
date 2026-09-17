#include "Lineup.h"
#include "Teams/Team.h"
#include "Helper/Input.h"

#include <iostream>
#include <cstring>

Lineup::Lineup(Team& team)
    : team(&team),
      playersCount(0)
{}

bool Lineup::containsPlayer(const char* name) const
{
    for (unsigned i = 0; i < playersCount; i++)
    {
        if (strcmp(players[i]->getName(), name) == 0)
            return true;
    }

    return false;
}

unsigned Lineup::countPosition(Position position) const
{
    unsigned count = 0;

    for (unsigned i = 0; i < playersCount; i++)
    {
        if (players[i]->getPosition() == position)
            count++;
    }

    return count;
}

void Lineup::choosePlayers()
{
    playersCount = 0;

    std::cout << "Choose 11 players:\n";

    while (playersCount < MAX_PLAYERS)
    {
        char name[128];

        std::cout << "Player " << playersCount + 1 << ": ";
        readLine(name, 128);

        if (containsPlayer(name))
        {
            std::cout << "This player is already in the lineup.\n";
            continue;
        }

        Player* player = team->getPlayer(name);

        if (!player)
        {
            std::cout << "Player does not exist in this team.\n";
            continue;
        }

        players[playersCount++] = player;

        std::cout << "Player added.\n";
    }

    if (!isValid())
    {
        std::cout << "The lineup is invalid.\n";
    }
}

bool Lineup::isValid() const
{
    if (playersCount != 11)
        return false;

    if (countPosition(Position::Goalkeeper) != 1)
        return false;

    if (countPosition(Position::Defender) < 2)
        return false;

    if (countPosition(Position::Midfielder) < 2)
        return false;

    if (countPosition(Position::Winger) < 2)
        return false;

    if (countPosition(Position::Forward) < 2)
        return false;

    return true;
}

Player* Lineup::findBestCandidate(bool used[]) const
{
    Player* best = nullptr;
    int bestIndex = -1;

    for (unsigned i = 0; i < Team::MAX_PLAYERS; i++)
    {
        Player* current = team->getPlayerByIndex(i);

        if (!current)
            continue;

        if (used[i])
            continue;

        if (containsPlayer(current->getName()))
            continue;

        if (current->getMatchesPlayed() >= 3)
            continue;

        if (!best ||
            current->getMatchesPlayed() < best->getMatchesPlayed())
        {
            best = current;
            bestIndex = i;
        }
    }

    if (bestIndex != -1)
        used[bestIndex] = true;

    return best;
}

void Lineup::includePlayersWithFewMatches()
{
    bool used[Team::MAX_PLAYERS]{};

    while (true)
    {
        Player* candidate = findBestCandidate(used);

        if (!candidate)
            break;

        bool swapPerformed = false;

        for (unsigned lineupIndex = 0;
             lineupIndex < playersCount;
             lineupIndex++)
        {
            Player* current = players[lineupIndex];

            if (current->getPosition() != candidate->getPosition())
                continue;

            if (!isValidAfterReplacement(lineupIndex, candidate))
                continue;

            std::cout
                << "\nPriority player found:\n"
                << candidate->getName()
                << " (" << candidate->getMatchesPlayed()
                << " matches)\n";

            std::cout
                << "Replace "
                << current->getName()
                << " ("
                << current->getMatchesPlayed()
                << " matches)"
                << " with "
                << candidate->getName()
                << "?\n";

            std::cout << "Confirm (y/n): ";

            char answer = readChar();

            if (answer == 'y' || answer == 'Y')
            {
                players[lineupIndex] = candidate;

                std::cout
                    << "Swap completed.\n";

                swapPerformed = true;
                break;
            }
        }

        if (!swapPerformed)
        {
            std::cout
                << "No accepted swap for "
                << candidate->getName()
                << ".\n";
        }
    }
}

bool Lineup::isValidAfterReplacement(const unsigned lineupIndex, const Player* newPlayer) const
{
    if (lineupIndex >= playersCount || !newPlayer)
        return false;

    unsigned goalkeepers = 0;
    unsigned defenders = 0;
    unsigned midfielders = 0;
    unsigned wingers = 0;
    unsigned forwards = 0;

    for (unsigned i = 0; i < playersCount; i++)
    {
        Position position;

        if (i == lineupIndex)
            position = newPlayer->getPosition();
        else
            position = players[i]->getPosition();

        switch (position)
        {
            case Position::Goalkeeper:
                goalkeepers++;
                break;

            case Position::Defender:
                defenders++;
                break;

            case Position::Midfielder:
                midfielders++;
                break;

            case Position::Winger:
                wingers++;
                break;

            case Position::Forward:
                forwards++;
                break;
        }
    }

    return goalkeepers == 1 &&
           defenders >= 2 &&
           midfielders >= 2 &&
           wingers >= 2 &&
           forwards >= 2;
}

Player* Lineup::getPlayer(const char* name) const
{
    for(unsigned i = 0; i < playersCount; i++)
    {
        if(strcmp(players[i]->getName(), name) == 0)
            return players[i];
    }

    return nullptr;
}

unsigned Lineup::getPlayersCount() const
{
    return playersCount;
}

Player* Lineup::getPlayerByIndex(unsigned index) const
{
    if (index >= playersCount)
        return nullptr;

    return players[index];
}

void Lineup::chooseAutomatically()
{
    playersCount = 0;
    bool used[Team::MAX_PLAYERS]{};

    for (unsigned i = 0; i < Team::MAX_PLAYERS; i++)
    {
        Player* p = team->getPlayerByIndex(i);
        if (!p) break;

        if (p->getPosition() == Position::Goalkeeper)
        {
            players[playersCount++] = p;
            used[i] = true;
            break;
        }
    }

    Position outfield[] = { Position::Defender, Position::Midfielder, Position::Winger, Position::Forward };

    for (Position pos : outfield)
    {
        unsigned added = 0;

        for (unsigned i = 0; i < Team::MAX_PLAYERS && added < 2 && playersCount < MAX_PLAYERS; i++)
        {
            Player* p = team->getPlayerByIndex(i);
            if (!p) break;

            if (!used[i] && p->getPosition() == pos)
            {
                players[playersCount++] = p;
                used[i] = true;
                added++;
            }
        }
    }

    for (unsigned i = 0; i < Team::MAX_PLAYERS && playersCount < MAX_PLAYERS; i++)
    {
        Player* p = team->getPlayerByIndex(i);
        if (!p) break;

        if (!used[i] && p->getPosition() != Position::Goalkeeper)
        {
            players[playersCount++] = p;
            used[i] = true;
        }
    }
}

void Lineup::playMatch(){
    if (!isValid())
    {
        std::cout << "Cannot play match. Invalid lineup.\n";
        return;
    }

    for (unsigned i = 0; i < playersCount; i++)
    {
        players[i]->playMatch();
    }
}