// TODO: check max hp
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <memory>
#include <cstdlib>
#include <sstream>
#include "color.h"
#include "player.h"
#include "textObserver.h"
#include "graphicalObserver.h"

#include "cc3k.h"

using namespace std;

int main(int argc, char *argv[])
{
    // Set seed to be random by default
    int seed = time(nullptr); 
    srand(seed);

    // Work with STL containers for command line arguments
    vector<string> args(argv, argv + argc);

    cout << "Welcome to CC3K! Collect as much gold as possible and navigate all 5 floors." << endl;

    // Game logic
    auto game = make_shared<CC3K>();



    // Ask the user to set a starting race
    cout << "Please select a starting race:" << endl;
    cout << Color::RED << "s: Shade" << Color::RESET << endl;
    cout << Color::BLUE << "d: Drow" << Color::RESET << endl;
    cout << Color::MAGENTA << "v: Vampire" << Color::RESET << endl;
    cout << Color::YELLOW << "g: Goblin" << Color::RESET << endl;
    cout << Color::BOLDCYAN << "t: Troll" << Color::RESET<< endl;
    cout << Color::BOLDBLUE << "(Bonus) n: Dragonslayer" << Color::RESET<< endl;

    cout << "q: Exit game"  << Color::RESET << endl;

    while (true)
    {
        string cmd;
        cin >> cmd;
        if (cmd == "s")
        {
            game->setStartingRace(Player::RaceTypes::SHADE);
            break;
        }
        else if (cmd == "d")
        {
            game->setStartingRace(Player::RaceTypes::DROW);
            break;
        }
        else if (cmd == "v")
        {
            game->setStartingRace(Player::RaceTypes::VAMPIRE);
            break;

        }
        else if (cmd == "g")
        {
            game->setStartingRace(Player::RaceTypes::GOBLIN);
            break;

        }
        else if (cmd == "t")
        {
            game->setStartingRace(Player::RaceTypes::TROLL);
            break;
        }
        // Quit
        else if (cmd == "q")
        {
            return 0;
        }
        else if (cmd == "n")
        {
            game->setStartingRace(Player::RaceTypes::DRAGONSLAYER);
            break;
        }

    }


    // Text display
    auto textObserver = make_shared<TextObserver>(game.get(), 79, 25);

    game->attach(textObserver.get());

    shared_ptr<GraphicalObserver> graphicalObserver = nullptr;

    // Parse command line args
    for (int i = 0; i < args.size(); i++)
    {
        if (i == 0)
        {
            // Ignore the "./cc3k"
            continue;
        }
        if (args[i] == "-graphics")
        {
            graphicalObserver = make_shared<GraphicalObserver>(game.get(), 79, 25);
            game->attach(graphicalObserver.get());
            game->addMessage("Enabled graphics. If it's not working, sure you have libxpm-dev installed in your Linux environment.\n", Color::BLUE);
        }
        else if (args[i] == "-fog")
        {
            game->setFog(true);
            game->addMessage("Enabled fog.\n", Color::BLUE);

        }
        else if (args[i] == "-dlc")
        {
            game->setDLC(true);
            game->addMessage("\n \n Enabled the CC3K++ content DLC pack! Here's some extra features in CC3K++: \n", Color::BLUE);
            game->addMessage("- Gain XP and level-up when you slay enemies! \n", Color::BLUE);
            game->addMessage("- Buy from (non-hostile) Merchants with 'b [direction]' command! \n", Color::BLUE);
            game->addMessage("- New enemy Pathfinder: Avoids obstacles and approaches you! \n", Color::BLUE);
            game->addMessage("- New player Dragonslayer: Deals immense damage to Dragons and can steal their hoards without slaying them! \n", Color::BLUE);
        }
        else if (args[i] == "-seed")
        {
            // Read the next argument which we assume is a number
            try {
                seed = stoi(args[i+1]);
                srand(seed);
                i += 1;
            }
            catch (const std::invalid_argument &e)
            {
                cerr << "Error: Specified seed is not an integer" << endl;
            }

        }
        // Otherwise it is a filename to a file containing five levels
        else
        {
            game->parseCustomLevels(args[i]);
        }
    }



    game->newGame();

    string cmdLine;

    // Create the display




    game->render();

    // Main game loop
    while (getline(cin, cmdLine))
    {


        istringstream iss(cmdLine);
        string cmd;



        while (iss >> cmd)
        {

        if (game->isGameOver || game->isGameComplete)
        {
            if (cmd == "r")
            {
                game->newGame();
            }
            else
            {
                return 0;
            }

        }


            // Player movement
            if (cmd == "no" || cmd == "so" || cmd == "ea" || cmd == "we" || cmd == "ne" || cmd == "nw" || cmd == "se" || cmd == "sw")
            {
                if (iss.eof())
                {
                    game->movePlayer(cmd);
                }
                // BONUS FEATURE: specify a number to repeatedly move
                else
                {
                    string numRepeat;
                    iss >> numRepeat;
                    try
                    {
                        for (int i = 0; i < stoi(numRepeat); i++)
                        {
                            game->movePlayer(cmd);
                        }
                    }
                    // Exception handling (if numRepeat cannot cast to an int)
                    catch (const std::invalid_argument &e)
                    {
                        game->movePlayer(cmd);
                    }
                }
            }

            // u direction: uses the potion indicated by the direction (e.g. no, so, ea).
            else if (cmd == "u")
            {
                string dir;
                // Read the direction
                // TODO: check invalid dir
                iss >> dir;
                game->usePotion(dir);
            }

            // a direction: attacks the enemy in the specified direction, if the monster is in the immediately specified block (e.g. must
            // be one block north of the @).
            else if (cmd == "a")
            {
                string dir;
                // Read the direction
                // TODO: check invalid dir
                iss >> dir;
                game->playerAttack(dir);
            }

            // Specify starting race
            else if (cmd == "s")
            {
                game->setStartingRace(Player::RaceTypes::SHADE);
                game->addMessage("You will play as a Shade in the next life.", Color::CYAN);
            }
            else if (cmd == "d")
            {
                game->setStartingRace(Player::RaceTypes::DROW);
                game->addMessage("You will play as a Drow in the next life.", Color::CYAN);
            }
            else if (cmd == "v")
            {
                game->setStartingRace(Player::RaceTypes::VAMPIRE);
                game->addMessage("You will play as a Vampire in the next life.", Color::CYAN);
            }
            else if (cmd == "g")
            {
                game->setStartingRace(Player::RaceTypes::GOBLIN);
                game->addMessage("You will play as a Goblin in the next life.", Color::CYAN);
            }
            else if (cmd == "t")
            {
                game->setStartingRace(Player::RaceTypes::TROLL);
                game->addMessage("You will play as a Troll in the next life.", Color::CYAN);
            }
            else if (cmd == "n")
            {
                game->setStartingRace(Player::RaceTypes::DRAGONSLAYER);
                game->addMessage("You will play as a Dragonslayer in the next life.", Color::CYAN);
            }

            // f: stops enemies from moving until this key is pressed again.
            else if (cmd == "f")
            {
                game->toggleStopEnemies();
            }

            // r: restarts the game-> All stats, inventory, and gold are reset. A new race should be selected.
            else if (cmd == "r")
            {
                game->newGame();
            }

            // q: allows the player to admit defeat and exit the game

            else if (cmd == "q")
            {
                cout << "Exiting." << endl;
                return 0;
            }

            else if (cmd == "b")
            {
                string dir;
                // Read the direction
                iss >> dir;

                if (game->getDLC())
                {
                    game->useMerchant(dir);
                }
                else 
                {
                    game->addMessage("This is a DLC feature. Please set -dlc as a flag first.", Color::MAGENTA);
                }
            }

            // Render the display after every command
            game->render();
        }
    }
}
