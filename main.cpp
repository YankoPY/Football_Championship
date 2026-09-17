#include "Helper/Menu.h"
#include <ctime>
#include <cstdlib>
#include <fstream>

static const char* SAVE_FILE = "championships.dat";

int main()
{
    srand((unsigned)(time(nullptr)));

    std::cout << "==== Football Championship ====\n";

    ChampionshipCollection collection;

    bool saveFileExists = std::ifstream(SAVE_FILE, std::ios::binary).good();

    if (collection.loadAllFromFile(SAVE_FILE))
    {
        std::cout << "Loaded " << collection.getCount()
                   << " championship(s) from \"" << SAVE_FILE << "\".\n";
    }
    else if (saveFileExists)
    {
        std::cout << "Warning: \"" << SAVE_FILE << "\" exists but could not be read "
                     "(it may be corrupted). Starting with an empty history.\n";
    }
    else
    {
        std::cout << "No saved championships found - starting fresh.\n";
    }

    while (true)
    {
        std::cout << "\n========== MAIN MENU ==========\n"
                   << "1. Create a new championship (new teams)\n"
                   << "2. Start a new season with last season's teams\n"
                   << "3. Quick-simulate a championship\n"
                   << "4. Continue an existing championship\n"
                   << "5. List all championships\n"
                   << "6. Save now\n"
                   << "7. Exit\n";

        unsigned choice = readUnsigned("Choice: ");
        bool autosave = true;

        switch (choice)
        {
            case 1:
                newChampionship(collection);
                break;

            case 2:
                carryOverSeason(collection);
                break;

            case 3:
                quickSimulate(collection);
                break;

            case 4:
                continueChampionship(collection);
                break;

            case 5:
                collection.printAll();
                autosave = false;
                break;

            case 6:
                if (collection.saveAllToFile(SAVE_FILE))
                    std::cout << "Saved.\n";
                else
                    std::cout << "Save failed.\n";
                autosave = false;
                break;

            case 7:
                collection.saveAllToFile(SAVE_FILE);
                std::cout << "Saved. Goodbye!\n";
                return 0;

            default:
                std::cout << "Invalid choice.\n";
                autosave = false;
        }

        if (autosave)
            collection.saveAllToFile(SAVE_FILE);
    }
}
