#ifndef FOOTBALLCHAMPIONSHIP_INPUT_H
#define FOOTBALLCHAMPIONSHIP_INPUT_H

#include <iostream>
#include <cstdlib>

inline void readLine(char* buffer, size_t size)
{
    std::cin.getline(buffer, size);

    if (std::cin.eof())
    {
        std::cout << "\n[End of input - exiting.]\n";
        std::exit(0);
    }
}

inline char readChar()
{
    char value;
    std::cin >> value;

    if (std::cin.eof())
    {
        std::cout << "\n[End of input - exiting.]\n";
        std::exit(0);
    }

    std::cin.ignore(10000, '\n');
    return value;
}

#endif //FOOTBALLCHAMPIONSHIP_INPUT_H
