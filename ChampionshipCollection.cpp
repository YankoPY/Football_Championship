#include "ChampionshipCollection.h"

#include <fstream>
#include <iostream>
#include <ctime>

ChampionshipCollection::ChampionshipCollection()
    : championships(nullptr), count(0), capacity(0)
{}

ChampionshipCollection::~ChampionshipCollection()
{
    free();
}

void ChampionshipCollection::free()
{
    for (unsigned i = 0; i < count; i++)
        delete championships[i];
    delete[] championships;

    championships = nullptr;
    count = 0;
    capacity = 0;
}

void ChampionshipCollection::resize()
{
    unsigned newCapacity = capacity == 0 ? 4 : capacity * 2;
    Championship** newArray = new Championship*[newCapacity];

    for (unsigned i = 0; i < count; i++)
        newArray[i] = championships[i];

    delete[] championships;
    championships = newArray;
    capacity = newCapacity;
}

bool ChampionshipCollection::addChampionship(Championship* champ)
{
    if (!champ)
        return false;

    for (unsigned i = 0; i < count; i++)
        if (championships[i]->getYear() == champ->getYear())
            return false;

    if (count >= capacity)
        resize();

    championships[count++] = champ;
    return true;
}

unsigned ChampionshipCollection::nextAvailableYear() const
{
    if (count == 0)
    {
        time_t now = time(nullptr);
        tm* localTime = localtime(&now);

        return localTime ? (unsigned)(localTime->tm_year + 1900) : 2024u;
    }

    unsigned maxYear = championships[0]->getYear();

    for (unsigned i = 1; i < count; i++)
        if (championships[i]->getYear() > maxYear)
            maxYear = championships[i]->getYear();

    return maxYear + 1;
}

Championship* ChampionshipCollection::startNewSeason(bool carryOverFromLatest)
{
    unsigned year = nextAvailableYear();
    Championship* champ = new Championship(year);

    if (carryOverFromLatest && count > 0)
    {
        Championship* previous = championships[count - 1];

        if (!champ->takeTeamsFrom(*previous) || !champ->startSeason())
        {
            delete champ;
            return nullptr;
        }
    }

    return champ;
}

void ChampionshipCollection::printAll() const
{
    if (count == 0)
    {
        std::cout << "\nNo championships yet.\n";
        return;
    }

    std::cout << "\n--- All championships ---\n";

    for (unsigned i = 0; i < count; i++)
    {
        Championship* c = championships[i];

        std::cout << "  " << c->getYear() << " - ";

        if (!c->isFinished())
        {
            std::cout << "in progress (round " << c->getCurrentRound()
                       << " of " << c->getTotalRounds() << ")\n";
            continue;
        }

        const char* champion = c->getChampionName();
        const char* runnerUp = c->getRunnerUpName();
        const char* third = c->getThirdName();

        std::cout << "champion: " << (champion ? champion : "-")
                   << ", runner-up: " << (runnerUp ? runnerUp : "-")
                   << ", third: " << (third ? third : "-") << "\n";
    }
}

unsigned ChampionshipCollection::getCount() const { return count; }

Championship* ChampionshipCollection::getChampionship(unsigned index) const
{
    if (index >= count)
        return nullptr;

    return championships[index];
}

Championship* ChampionshipCollection::getLatest() const
{
    if (count == 0)
        return nullptr;

    return championships[count - 1];
}

bool ChampionshipCollection::saveAllToFile(const char* filename) const
{
    std::ofstream os(filename, std::ios::binary | std::ios::trunc);

    if (!os)
        return false;

    os.write((const char*)(&count), sizeof(count));

    for (unsigned i = 0; i < count; i++)
    {
        if (!championships[i]->saveToFile(os))
            return false;
    }

    return os.good();
}

bool ChampionshipCollection::loadAllFromFile(const char* filename)
{
    std::ifstream is(filename, std::ios::binary);

    if (!is)
        return false;

    unsigned savedCount;
    is.read((char*)(&savedCount), sizeof(savedCount));

    if (!is)
        return false;

    free();

    for (unsigned i = 0; i < savedCount; i++)
    {
        Championship* champ = Championship::loadFromFile(is);

        if (!champ || !addChampionship(champ))
        {
            delete champ;
            free();
            return false;
        }
    }

    return true;
}
