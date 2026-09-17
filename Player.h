#ifndef FOOTBALLCHAMPIONSHIP_PLAYER_H
#define FOOTBALLCHAMPIONSHIP_PLAYER_H
#include <iosfwd>

enum class Position {
    Goalkeeper,
    Defender,
    Midfielder,
    Winger,
    Forward
};

class Player {
public:
    Player(const char* name, unsigned number, Position position, double salary, double transferValue);
    Player(const Player& other);
    Player& operator=(const Player& other);
    ~Player();

    void print() const;
    bool savePlayer(std::ostream& os) const;
    static Player readPlayer(std::istream& is);
    void playMatch();
    void scoreGoal();
    void salaryRegulation(unsigned teamTotalMatches);

    char* getName() const;
    unsigned getNumber() const;
    Position getPosition() const;
    unsigned getScoredGoals() const;
    double getTransferValue() const;
    unsigned getMatchesPlayed() const;
    double getSalary() const;

private:
    char* name;
    unsigned number;
    Position position;
    double salary;
    double transferValue;
    unsigned matchesPlayed;
    unsigned scoredGoals;

    void free();

};

#endif //FOOTBALLCHAMPIONSHIP_PLAYER_H