#include "AttackingTeam.h"

bool AttackingTeam::satisfiesComposition() const
{
    return countPosition(Position::Goalkeeper) == 2 &&
           countPosition(Position::Forward) >= 8 &&
           countPosition(Position::Defender) >= 2 &&
           countPosition(Position::Midfielder) >= 2 &&
           countPosition(Position::Winger) >= 2;
}

bool AttackingTeam::canBuySpecificPlayer(const Player& player) const
{
    if(player.getPosition() == Position::Forward)
        return player.getScoredGoals() >= 6;

    return true;
}

char AttackingTeam::getTypeCode() const
{
    return 'A';
}