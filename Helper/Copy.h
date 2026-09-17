#pragma once
#include <cstring>

inline char* copyString(const char* string) {
    if (!string)
        string = "";

    char* copy = new char[strlen(string) + 1];
    strcpy(copy, string);
    return copy;
}
