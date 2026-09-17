#pragma once
#include "Team.h"

class BalancedTeam : public Team
{
public:
    using Team::Team;

protected:
    bool satisfiesComposition() const override;
    bool canBuySpecificPlayer(const Player& player) const override;
    char getTypeCode() const override;
};