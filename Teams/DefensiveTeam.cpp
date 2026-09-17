#include "DefensiveTeam.h"

bool DefensiveTeam::satisfiesComposition() const
{
    return countPosition(Position::Goalkeeper) == 2 &&
           countPosition(Position::Defender) >= 6 &&
           countPosition(Position::Midfielder) >= 6 &&
           countPosition(Position::Winger) >= 2 &&
           countPosition(Position::Forward) >= 2;
}

bool DefensiveTeam::canBuySpecificPlayer(const Player&) const
{
    return true;
}

char DefensiveTeam::getTypeCode() const
{
    return 'D';
}
