#include "GameManager.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

int main() {
    try {
        // Seed the random number generator
        std::srand(static_cast<unsigned int>(std::time(nullptr)));
        
        // Create game manager
        std::cout << "Initializing game..." << std::endl;
        GameManager gameManager;
        
        // Run the game
        std::cout << "Starting game loop..." << std::endl;
        gameManager.run();
        
        // Game has ended normally
        std::cout << "Game ended successfully." << std::endl;
        return 0;
    }
    catch (const std::exception& e) {
        // Handle any exceptions
        std::cerr << "ERROR: " << e.what() << std::endl;
        return 1;
    }
    catch (...) {
        // Catch any other exceptions
        std::cerr << "ERROR: Unknown exception occurred." << std::endl;
        return 1;
    }
}