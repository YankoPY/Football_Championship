#include "Player.h"
#include <cstring>
#include <iostream>

#include "Helper/Copy.h"

Player::Player(const char *name, unsigned number, Position position, double salary, double transferValue)
: name(copyString(name)), number(number), position(position), salary(salary), transferValue(transferValue), matchesPlayed(0), scoredGoals(0) {}

Player::Player(const Player& other)
    : name(copyString(other.name)), number(other.number), position(other.position), salary(other.salary), transferValue(other.transferValue), matchesPlayed(other.matchesPlayed), scoredGoals(other.scoredGoals)
{}

Player& Player::operator=(const Player& other)
{
    if (this != &other)
    {
        char* newName = copyString(other.name);

        free();

        name = newName;
        number = other.number;
        position = other.position;
        salary = other.salary;
        transferValue = other.transferValue;
        matchesPlayed = other.matchesPlayed;
        scoredGoals = other.scoredGoals;
    }

    return *this;
}

Player::~Player() {
    free();
}

bool Player::savePlayer(std::ostream& os) const {
    size_t nameLength =strlen(name);

    os.write((const char*)(&nameLength), sizeof(nameLength));
    os.write(name, nameLength);

    os.write((const char*)(&number), sizeof(number));
    os.write((const char*)(&position), sizeof(position));
    os.write((const char*)(&salary), sizeof(salary));
    os.write((const char*)(&transferValue), sizeof(transferValue));
    os.write((const char*)(&matchesPlayed), sizeof(matchesPlayed));
    os.write((const char*)(&scoredGoals), sizeof(scoredGoals));

    return os.good();
}

Player Player::readPlayer(std::istream& is)
{
    size_t nameLength;

    is.read((char*)(&nameLength),sizeof(nameLength));

    char* buffer = nullptr;

    if (nameLength)
    {
        buffer = new char[nameLength + 1];
        is.read(buffer, nameLength);
        buffer[nameLength] = '\0';
    }

    unsigned number;
    Position position;
    double salary;
    double transferValue;
    unsigned matchesPlayed;
    unsigned scoredGoals;

    is.read((char*)(&number),
            sizeof(number));

    is.read((char*)(&position),
            sizeof(position));

    is.read((char*)(&salary),
            sizeof(salary));

    is.read((char*)(&transferValue),
            sizeof(transferValue));

    is.read((char*)(&matchesPlayed),
            sizeof(matchesPlayed));

    is.read((char*)(&scoredGoals),
            sizeof(scoredGoals));

    Player result(buffer, number, position, salary, transferValue);

    result.matchesPlayed = matchesPlayed;
    result.scoredGoals = scoredGoals;

    delete[] buffer;

    return result;
}

void Player::print() const {
    static const char* positionNames[] = {
        "Goalkeeper", "Defender", "Midfielder", "Winger", "Forward"
    };

    std::cout << "  #" << number << " " << name
               << " [" << positionNames[(int)(position)] << "]"
               << " | Salary: " << salary
               << " | Transfer value: " << transferValue
               << " | Matches: " << matchesPlayed
               << " | Goals: " << scoredGoals;
}

void Player::salaryRegulation(unsigned teamTotalMatches) {

    if (teamTotalMatches > 0 &&
        matchesPlayed > teamTotalMatches / 2.0)
    {
        salary *= 1.20;
    }

    if (matchesPlayed < 7)
    {
        salary *= 0.90;
    }

    if (scoredGoals > 5)
    {
        salary *= 1.10;
    }

    if (scoredGoals > 10)
    {
        salary *= 1.20;
    }

    if (scoredGoals < 5 && position == Position::Forward)
    {
        salary *= 0.85;
    }
}

void Player::playMatch() {
    matchesPlayed++;
}

void Player::scoreGoal() {
    scoredGoals++;
}

char *Player::getName() const {return name;}
unsigned Player::getNumber() const {return number;}
Position Player::getPosition() const {return position;}
unsigned Player::getScoredGoals() const {return scoredGoals;}
unsigned Player::getMatchesPlayed() const {return matchesPlayed;}
double Player::getTransferValue() const {return transferValue;}
double Player::getSalary() const {return salary;}

void Player::free() {
    delete[] name;
}