#ifndef FOOTBALLCHAMPIONSHIP_MENU_H
#define FOOTBALLCHAMPIONSHIP_MENU_H
#include <iostream>
#include "../ChampionshipCollection.h"
#include "../Teams/AttackingTeam.h"
#include "../Teams/DefensiveTeam.h"
#include "../Teams/BalancedTeam.h"
#include "../Player.h"
#include "Input.h"

#include <cstring>
#include <cstdlib>

static unsigned readUnsigned(const char* prompt)
{
    unsigned value;

    while (true)
    {
        std::cout << prompt;
        std::cin >> value;

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
        return value;
    }
}

static double readDouble(const char* prompt)
{
    double value;

    while (true)
    {
        std::cout << prompt;
        std::cin >> value;

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
        return value;
    }
}

static void readLine(const char* prompt, char* buffer, size_t size)
{
    std::cout << prompt;
    readLine(buffer, size);
}

static void readRequiredLine(const char* prompt, char* buffer, size_t size)
{
    while (true)
    {
        readLine(prompt, buffer, size);

        if (buffer[0] != '\0')
            return;

        std::cout << "This field cannot be empty.\n";
    }
}

static bool readBool(const char* prompt)
{
    while (true)
    {
        std::cout << prompt << " (y/n): ";
        char answer = readChar();

        if (answer == 'y' || answer == 'Y') return true;
        if (answer == 'n' || answer == 'N') return false;

        std::cout << "Please answer y or n.\n";
    }
}

static Position readPosition()
{
    char buffer[32];

    while (true)
    {
        readLine(
            "  Position (Goalkeeper/Defender/Midfielder/Winger/Forward): ",
            buffer, sizeof(buffer));

        if (strcmp(buffer, "Goalkeeper") == 0) return Position::Goalkeeper;
        if (strcmp(buffer, "Defender") == 0) return Position::Defender;
        if (strcmp(buffer, "Midfielder") == 0) return Position::Midfielder;
        if (strcmp(buffer, "Winger") == 0) return Position::Winger;
        if (strcmp(buffer, "Forward") == 0) return Position::Forward;

        std::cout << "  Invalid position, try again.\n";
    }
}

static Player readPlayer()
{
    char name[128];
    readRequiredLine("  Player name: ", name, sizeof(name));

    unsigned number = readUnsigned("  Number: ");
    Position position = readPosition();
    double salary = readDouble("  Salary: ");
    double transferValue = readDouble("  Transfer value: ");

    return Player(name, number, position, salary, transferValue);
}

static Team* createTeam(double budget)
{
    std::cout << "\n--- New team ---\n";

    char typeChoice;
    while (true)
    {
        std::cout << "Team type - (A)ttacking / (D)efensive / (B)alanced: ";
        typeChoice = readChar();

        if (typeChoice == 'A' || typeChoice == 'a' ||
            typeChoice == 'D' || typeChoice == 'd' ||
            typeChoice == 'B' || typeChoice == 'b')
            break;

        std::cout << "Invalid choice.\n";
    }

    char name[128];
    char coach[128];
    char stadium[128];

    readRequiredLine("Team name: ", name, sizeof(name));
    readRequiredLine("Coach name: ", coach, sizeof(coach));
    readRequiredLine("Stadium name: ", stadium, sizeof(stadium));

    Team* team;

    if (typeChoice == 'A' || typeChoice == 'a')
        team = new AttackingTeam(name, coach, stadium, budget);
    else if (typeChoice == 'D' || typeChoice == 'd')
        team = new DefensiveTeam(name, coach, stadium, budget);
    else
        team = new BalancedTeam(name, coach, stadium, budget);

    std::cout << "\nAdding players to \"" << name << "\":\n";

    while (true)
    {
        while (readBool("Add a player?"))
        {
            Player p = readPlayer();

            if (team->addPlayer(p))
                std::cout << "  Player added.\n";
            else
                std::cout << "  Could not add player (duplicate name/number, "
                             "budget, or goalkeeper limit reached).\n";
        }

        if (team->isSquadValid())
            break;

        std::cout << "\nThis squad does not yet satisfy the composition "
                     "requirements for this team type. Add more players.\n";
    }

    std::cout << "Team \"" << name << "\" is ready.\n";
    return team;
}

static unsigned selectTeamIndex(Championship& champ, const char* prompt)
{
    std::cout << prompt << "\n";
    for (unsigned i = 0; i < champ.getTeamsCount(); i++)
        std::cout << "  - " << champ.getTeam(i)->getName() << "\n";

    char name[128];

    while (true)
    {
        readLine("Team name: ", name, sizeof(name));

        for (unsigned i = 0; i < champ.getTeamsCount(); i++)
        {
            if (strcmp(champ.getTeam(i)->getName(), name) == 0)
                return i;
        }

        std::cout << "No team with that name.\n";
    }
}

static void teamInfo(Championship& champ)
{
    unsigned idx = selectTeamIndex(champ, "Choose a team to inspect:");
    champ.getTeam(idx)->print();
}

static void buyPlayer(Championship& champ)
{
    unsigned idx = selectTeamIndex(champ, "Choose a team to buy a player for:");
    Team* team = champ.getTeam(idx);

    Player p = readPlayer();

    if (team->addPlayer(p))
        std::cout << "Player bought successfully.\n";
    else
        std::cout << "Could not buy player (budget, duplicate name/number, "
                     "or goalkeeper limit reached).\n";
}

static void removePlayer(Championship& champ)
{
    unsigned idx = selectTeamIndex(champ, "Choose a team to remove a player from:");
    Team* team = champ.getTeam(idx);

    char name[128];
    readLine("Player name to remove: ", name, sizeof(name));

    if (team->removePlayer(name))
        std::cout << "Player removed.\n";
    else
        std::cout << "No such player.\n";
}

static void swapPlayers(Championship& champ)
{
    unsigned idxA = selectTeamIndex(champ, "First team:");
    char nameA[128];
    char promptA[160];
    snprintf(promptA, sizeof(promptA), "Player from %s: ", champ.getTeam(idxA)->getName());
    readLine(promptA, nameA, sizeof(nameA));

    unsigned idxB = selectTeamIndex(champ, "Second team:");
    char nameB[128];
    char promptB[160];
    snprintf(promptB, sizeof(promptB), "Player from %s: ", champ.getTeam(idxB)->getName());
    readLine(promptB, nameB, sizeof(nameB));

    if (champ.swapPlayers(idxA, nameA, idxB, nameB))
        std::cout << "Swap completed.\n";
    else
        std::cout << "Swap failed (missing player, or one of the squads "
                     "would become invalid after the swap).\n";
}

static void regulateSalaries(Championship& champ)
{
    champ.regulateAllSalaries();
    std::cout << "Salaries regulated for all teams based on this season's performance.\n";
}

static void seasonMenu(Championship& champ)
{
    while (true)
    {
        std::cout << "\n===== Championship " << champ.getYear()
                   << (champ.isFinished() ? " [FINISHED]" : " [IN PROGRESS]") << " =====\n"
                   << "1. Play next round\n"
                   << "2. Play all remaining rounds\n"
                   << "3. Print standings\n"
                   << "4. Print top scorer(s)\n"
                   << "5. Print team info\n"
                   << "6. Buy a player\n"
                   << "7. Remove a player\n"
                   << "8. Swap players between two teams\n"
                   << "9. Regulate salaries\n"
                   << "10. Back to main menu\n";

        unsigned choice = readUnsigned("Choice: ");

        switch (choice)
        {
            case 1:
                if (!champ.playRound())
                    std::cout << "The championship has already finished.\n";
                break;

            case 2:
                champ.playAllRounds();
                break;

            case 3:
                champ.printStandings();
                break;

            case 4:
                champ.printTopScorers();
                break;

            case 5:
                teamInfo(champ);
                break;

            case 6:
                buyPlayer(champ);
                break;

            case 7:
                removePlayer(champ);
                break;

            case 8:
                swapPlayers(champ);
                break;

            case 9:
                regulateSalaries(champ);
                break;

            case 10:
                return;

            default:
                std::cout << "Invalid choice.\n";
        }
    }
}

static void newChampionship(ChampionshipCollection& collection)
{
    Championship* champ = collection.startNewSeason(false);

    std::cout << "\n--- New championship: year " << champ->getYear() << " ---\n";

    unsigned teamCount;
    while (true)
    {
        teamCount = readUnsigned("Number of teams (even, at least 4): ");

        if (teamCount >= 4 && teamCount % 2 == 0)
            break;

        std::cout << "Must be even and at least 4.\n";
    }

    double budget = readDouble(
        "Starting budget for every team (same for all teams this season): ");

    for (unsigned i = 0; i < teamCount; i++)
    {
        std::cout << "\n=== Team " << (i + 1) << " of " << teamCount << " ===\n";
        Team* t = createTeam(budget);

        if (!champ->addTeam(t))
        {
            std::cout << "A team with that name already exists in this "
                         "championship. Try again for this slot.\n";
            delete t;
            i--;
        }
    }

    if (!champ->startSeason())
    {
        std::cout << "Failed to start the season.\n";
        delete champ;
        return;
    }

    std::cout << "\nChampionship " << champ->getYear() << " created with " << teamCount
               << " teams and " << champ->getTotalRounds() << " rounds.\n";

    collection.addChampionship(champ);
    seasonMenu(*champ);
}

static void carryOverSeason(ChampionshipCollection& collection)
{
    if (collection.getCount() == 0)
    {
        std::cout << "There is no previous championship to carry teams over from.\n";
        return;
    }

    Championship* previous = collection.getLatest();

    if (!previous->isFinished())
    {
        std::cout << "Championship " << previous->getYear()
                   << " hasn't finished yet - finish it (or play its remaining rounds) "
                     "before starting a new season from its teams.\n";
        return;
    }

    std::cout << "This starts a new season reusing the " << previous->getTeamsCount()
               << " team(s) from championship " << previous->getYear()
               << " (rosters and budgets kept, records reset to 0).\n";

    if (!readBool("Continue?"))
        return;

    Championship* champ = collection.startNewSeason(true);

    if (!champ)
    {
        std::cout << "Could not start the new season "
                     "(the previous championship's teams didn't form a valid schedule).\n";
        return;
    }

    collection.addChampionship(champ);

    std::cout << "\nChampionship " << champ->getYear() << " created with "
               << champ->getTeamsCount() << " teams (carried over) and "
               << champ->getTotalRounds() << " rounds.\n";

    seasonMenu(*champ);
}

static void continueChampionship(ChampionshipCollection& collection)
{
    if (collection.getCount() == 0)
    {
        std::cout << "There are no championships yet.\n";
        return;
    }

    std::cout << "\nChampionships:\n";
    for (unsigned i = 0; i < collection.getCount(); i++)
    {
        Championship* c = collection.getChampionship(i);
        std::cout << "  " << (i + 1) << ". " << c->getYear()
                   << (c->isFinished() ? " [FINISHED]" : " [IN PROGRESS]") << "\n";
    }

    unsigned choice;
    while (true)
    {
        choice = readUnsigned("Choose a championship (number): ");

        if (choice >= 1 && choice <= collection.getCount())
            break;

        std::cout << "Invalid choice.\n";
    }

    seasonMenu(*collection.getChampionship(choice - 1));
}

static Player generateRandomPlayer(unsigned number, Position position, bool boostForwardGoals)
{
    static unsigned counter = 0;

    char name[32];
    snprintf(name, sizeof(name), "Player%u", counter++);

    double salary = 800.0 + (rand() % 1200);
    double transferValue = 200.0 + (rand() % 800);

    Player p(name, number, position, salary, transferValue);

    if (boostForwardGoals && position == Position::Forward)
    {
        for (int i = 0; i < 6; i++)
            p.scoreGoal();
    }

    return p;
}

static void autoFillSquad(Team& team, char typeChoice)
{
    unsigned number = 1;
    bool isAttacking = (typeChoice == 'A' || typeChoice == 'a');

    unsigned gk, def, mid, win, fwd;

    if (typeChoice == 'A' || typeChoice == 'a')
    {
        gk = 2; def = 2; mid = 2; win = 2; fwd = 8;
    }
    else if (typeChoice == 'D' || typeChoice == 'd')
    {
        gk = 2; def = 6; mid = 6; win = 2; fwd = 2;
    }
    else
    {
        gk = 2; def = 4; mid = 4; win = 4; fwd = 4;
    }

    for (unsigned i = 0; i < gk;  i++) team.addPlayer(generateRandomPlayer(number++, Position::Goalkeeper, isAttacking));
    for (unsigned i = 0; i < def; i++) team.addPlayer(generateRandomPlayer(number++, Position::Defender,   isAttacking));
    for (unsigned i = 0; i < mid; i++) team.addPlayer(generateRandomPlayer(number++, Position::Midfielder, isAttacking));
    for (unsigned i = 0; i < win; i++) team.addPlayer(generateRandomPlayer(number++, Position::Winger,     isAttacking));
    for (unsigned i = 0; i < fwd; i++) team.addPlayer(generateRandomPlayer(number++, Position::Forward,    isAttacking));
}

static void quickSimulate(ChampionshipCollection& collection)
{
    const double AUTO_BUDGET = 100000.0;
    const char TYPES[] = { 'A', 'D', 'B' };

    unsigned teamCount;
    while (true)
    {
        teamCount = readUnsigned("Number of teams (even, at least 4): ");

        if (teamCount >= 4 && teamCount % 2 == 0)
            break;

        std::cout << "Must be even and at least 4.\n";
    }

    Championship* champ = collection.startNewSeason(false);

    std::cout << "\nAuto-generating " << teamCount << " teams for championship "
               << champ->getYear() << "...\n";

    for (unsigned i = 0; i < teamCount; i++)
    {
        char typeChoice = TYPES[i % 3];

        char name[32];
        char coach[32];
        char stadium[32];
        snprintf(name, sizeof(name), "Team%u", i + 1);
        snprintf(coach, sizeof(coach), "Coach%u", i + 1);
        snprintf(stadium, sizeof(stadium), "Stadium%u", i + 1);

        Team* t;
        if (typeChoice == 'A')      t = new AttackingTeam(name, coach, stadium, AUTO_BUDGET);
        else if (typeChoice == 'D') t = new DefensiveTeam(name, coach, stadium, AUTO_BUDGET);
        else                        t = new BalancedTeam(name, coach, stadium, AUTO_BUDGET);

        autoFillSquad(*t, typeChoice);
        champ->addTeam(t);
    }

    if (!champ->startSeason())
    {
        std::cout << "Failed to start the auto-generated season.\n";
        delete champ;
        return;
    }

    collection.addChampionship(champ);

    std::cout << "\nSimulating " << champ->getTotalRounds() << " rounds for "
               << teamCount << " auto-generated teams...\n";

    champ->playAllRoundsAuto();

    std::cout << "\n=== Simulation complete ===\n";
    champ->printStandings();
    champ->printTopScorers();

    std::cout << "\nYou can keep exploring this championship below "
                 "(buy/sell/swap players, inspect teams, etc.)\n";

    seasonMenu(*champ);
}

#endif //FOOTBALLCHAMPIONSHIP_MENU_H
