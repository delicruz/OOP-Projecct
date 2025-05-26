#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <string>
#include "Enums.h"
#include "BattleSystem.h"
#include "MapManager.h"
#include "Player.h"
#include "Enemy.h"
#include "Dungeon.h"

// Forward declaration 
class MapManager;   
class Player;

using namespace std;

class GameManager {
private:
    // Window
    sf::RenderWindow window;
    
    // Game state
    GameState currentState;
    
    // Resources
    sf::Font font;
    sf::Texture playerBattleTexture;
    sf::Texture enemyBattleTexture;

    // Battle stats
    bool lastBattleWon = false;
    int lastBattleExp = 0;
    string lastBattleSkill; // Leave empty if none
    
    // Menu variables
    int selectedMenuOption;
    int selectedSlotOption;
    vector<std::string> menuOptions;
    
    // Battle variables
    int selectedBattleAction;
    vector<std::string> battleActions;
    shared_ptr<Enemy> currentEnemy;
    BattleSystem battleSystem;
    int currentDoorIndex;
    Dungeon dungeon;
    
    // Game systems
    MapManager* mapManager;

    
    // Game entities
    Player* player;
    
    // Helper methods
    void loadResources();

    // Inventory UI state
    int inventorySelectionIndex = 0;
    
    // Input handling methods
    void handleMainMenuInput(const sf::Event& event);
    void handleHubWorldInput(const sf::Event& event);
    void handleBeforeFightInput(const sf::Event& event);
    void handleBattleInput(const sf::Event& event);
    void handleInventoryInput(const sf::Event& event);
    void handleSaveSlotMenuInput(const sf::Event& event);
    void handleLoadSlotMenuInput(const sf::Event& event);
    
public:
    GameManager();
    ~GameManager();
    
    // Core game methods
    void run();
    void processEvents();
    void update(float deltaTime);
    void render();
  
    
    // Screen drawing methods
    void drawMainMenu();
    void drawBeforeFightScreen();
    void drawBattleScreen();
    void drawInventoryScreen();
    void drawEndScreen();
    void drawSaveSlotMenu();
    void drawLoadSlotMenu();
    void drawBattleResultPopup();
    
    // Transition methods
    void startTransition(GameState newState);
    void startBattle(int doorIndex);
    void endBattle(bool playerWon);
    
    // Getters
    sf::RenderWindow& getWindow() { return window; }
    sf::Font& getFont() { return font; }
    Player* getPlayer();

    // Save/Load methods
    void saveGame(int slot);
    bool loadGame(int slot);
    vector<int> getExistingSaves() const;
    

};

#endif