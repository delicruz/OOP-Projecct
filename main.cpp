#include "GameManager.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

using namespace std;

int main() {
    try {
        // Seed the random number generator
        srand(static_cast<unsigned int>(time(nullptr)));
        
        // Create game manager
        cout << "Initializing game..." << endl;
        GameManager gameManager;
        
        // Run the game
        cout << "Starting game loop..." << endl;
        gameManager.run();
        
        // Game has ended normally
        cout << "Game ended successfully." << endl;
        return 0;
    }
    catch (const std::exception& e) {
        // Handle any exceptions
        cerr << "ERROR: " << e.what() << endl;
        return 1;
    }
    catch (...) {
        // Catch any other exceptions
        cerr << "ERROR: Unknown exception occurred." << endl;
        return 1;
    }
}