#include "BalancedTeam.h"

bool BalancedTeam::satisfiesComposition() const
{
    return countPosition(Position::Goalkeeper) == 2 &&
           countPosition(Position::Defender) >= 4 &&
           countPosition(Position::Midfielder) >= 4 &&
           countPosition(Position::Winger) >= 4 &&
           countPosition(Position::Forward) >= 4;
}

bool BalancedTeam::canBuySpecificPlayer(const Player&) const
{
    return true;
}

char BalancedTeam::getTypeCode() const
{
    return 'B';
}
