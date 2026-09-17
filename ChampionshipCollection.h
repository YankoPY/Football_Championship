#ifndef FOOTBALLCHAMPIONSHIP_CHAMPIONSHIPCOLLECTION_H
#define FOOTBALLCHAMPIONSHIP_CHAMPIONSHIPCOLLECTION_H

#include "Championship.h"

class ChampionshipCollection
{
public:
    ChampionshipCollection();
    ~ChampionshipCollection();

    ChampionshipCollection(const ChampionshipCollection&) = delete;
    ChampionshipCollection& operator=(const ChampionshipCollection&) = delete;

    bool addChampionship(Championship* champ);

    unsigned nextAvailableYear() const;

    Championship* startNewSeason(bool carryOverFromLatest);

    void printAll() const;

    unsigned getCount() const;
    Championship* getChampionship(unsigned index) const;
    Championship* getLatest() const;

    bool saveAllToFile(const char* filename) const;
    bool loadAllFromFile(const char* filename);

private:
    Championship** championships;
    unsigned count;
    unsigned capacity;

    void resize();
    void free();
};

#endif //FOOTBALLCHAMPIONSHIP_CHAMPIONSHIPCOLLECTION_H
