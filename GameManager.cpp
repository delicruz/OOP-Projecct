#include "GameManager.h"

#include <cstdlib>
#include <ctime>
#include <iostream>

#include "Skill.h"  // for SkillType

using namespace std;

static SkillType stringToSkillType(const std::string& s) {
  if (s == "Slash") return SkillType::Slash;
  if (s == "Ice Slash") return SkillType::IceSlash;
  if (s == "Poison Dart") return SkillType::PoisonDart;
  if (s == "Heal") return SkillType::Heal;
  return SkillType::None;
}

GameManager::GameManager()
    : window(sf::VideoMode(1280, 720), "Turn-Based Strategy Game"),
      currentState(GameState::MainMenu),
      selectedMenuOption(0),
      selectedBattleAction(0),
      currentEnemy(nullptr),
      currentDoorIndex(-1) {
  // Seed random number generator
  srand(static_cast<unsigned int>(time(nullptr)));

  // Configure window
  window.setFramerateLimit(60);

  // Set up menu options
  menuOptions = {"Play Game", "Save Game", "Load Game", "Exit"};

  // Load resources
  loadResources();

  // Create player
  player = new Player("Hero", 100);

  // Create and initialize map manager
  mapManager = new MapManager(this, &window, &font);
  mapManager->initialize();

  dungeon.resetDungeon(mapManager->getDoorPositions());
}

GameManager::~GameManager() {
  // Clean up allocated objects
  delete mapManager;
  delete player;
}

void GameManager::loadResources() {
  if (!font.loadFromFile(
          "/home/nguyenvu343/sfml/Image and fonts/Roboto-Black.ttf")) {
    cerr << "Failed to load font from backup path!" << endl;
    throw std::runtime_error("Font loading failed");
  }

  if (!playerBattleTexture.loadFromFile("/home/nguyenvu343/sfml/Image and "
                                        "fonts/0_Fallen_Angels_Idle_008.png")) {
    cerr << "Failed to load player battle texture" << endl;
  } else {
    cout << "Successfully loaded player battle texture" << endl;
  }

  // Load enemy battle texture - using one texture for all enemies
  if (!enemyBattleTexture.loadFromFile(
          "Image and fonts/0_Dark_Oracle_Idle_012.png")) {
    cerr << "Failed to load enemy battle texture" << endl;
  } else {
    cout << "Successfully loaded enemy battle texture" << endl;
  }
}

void GameManager::run() {
  sf::Clock clock;

  // Main game loop
  while (window.isOpen()) {
    float deltaTime = clock.restart().asSeconds();

    processEvents();
    update(deltaTime);
    render();
  }
}

void GameManager::processEvents() {
  sf::Event event;
  while (window.pollEvent(event)) {
    if (event.type == sf::Event::Closed) {
      window.close();
    }

    if (event.type == sf::Event::KeyPressed) {
      // Handle input based on current game state
      switch (currentState) {
        case GameState::MainMenu:
          handleMainMenuInput(event);
          break;

        case GameState::SaveMenu:
          handleSaveSlotMenuInput(event);
          break;

        case GameState::LoadMenu:
          handleLoadSlotMenuInput(event);
          break;

        case GameState::HubWorld:
          handleHubWorldInput(event);
          break;
        case GameState::BeforeFight:
          handleBeforeFightInput(event);
          break;

        case GameState::Battle:
          battleSystem.handleInput(event);
          break;

        case GameState::Inventory:
          handleInventoryInput(event);
          break;

        default:
          break;
      }
    }

    if (currentState == GameState::EndGame &&
        event.type == sf::Event::KeyPressed) {
      startTransition(
          GameState::MainMenu);  // Go back to main menu on any key press
    }

    if (currentState == GameState::PopupResult &&
        event.type == sf::Event::KeyPressed) {
      startTransition(GameState::HubWorld);
    }
  }

  if (currentState == GameState::HubWorld) {
    const float moveSpeed = 5.0f;  // Movement in pixels per frame

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
      mapManager->movePlayer(0, -moveSpeed);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
      mapManager->movePlayer(0, moveSpeed);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
      mapManager->movePlayer(-moveSpeed, 0);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
      mapManager->movePlayer(moveSpeed, 0);
    }
  }
}

void GameManager::handleMainMenuInput(const sf::Event& event) {
  if (event.key.code == sf::Keyboard::Up) {
    if (selectedMenuOption > 0) {
      selectedMenuOption--;
      cout << "Selected: " << menuOptions[selectedMenuOption] << endl;
    }
  } else if (event.key.code == sf::Keyboard::Down) {
    if (selectedMenuOption < static_cast<int>(menuOptions.size() - 1)) {
      selectedMenuOption++;
      cout << "Selected: " << menuOptions[selectedMenuOption] << endl;
    }
  } else if (event.key.code == sf::Keyboard::Return) {
    if (selectedMenuOption == 0) {
      // Play Game
      cout << "Play selected!" << endl;
      startTransition(GameState::HubWorld);
    } else if (selectedMenuOption == 1) {
      currentState = GameState::SaveMenu;
      selectedSlotOption = 0;
    } else if (selectedMenuOption == 2) {
      currentState = GameState::LoadMenu;
      selectedSlotOption = 0;
    } else if (selectedMenuOption == 3) {
      // Exit Game
      cout << "Exiting game..." << endl;
      window.close();
    }
  }
}

void GameManager::handleHubWorldInput(const sf::Event& event) {
  if (event.key.code == sf::Keyboard::Escape) {
    cout << "Returning to main menu..." << endl;
    startTransition(GameState::MainMenu);
  } else if (event.key.code == sf::Keyboard::I) {
    cout << "Opening inventory..." << endl;
    startTransition(GameState::Inventory);
  } else if (event.key.code == sf::Keyboard::Space) {
    // Check if player is near a door
    if (mapManager->isPlayerNearDoor()) {
      cout << "Entering gate..." << endl;
      currentDoorIndex = mapManager->getNearestDoorIndex();
      currentEnemy = dungeon.getEnemyAt(currentDoorIndex);
      startTransition(GameState::BeforeFight);
    }
    // Check if player is near a heal orb
    else if (mapManager->isPlayerNearHealOrb()) {
      cout << "Found a healing orb!" << endl;

      // Heal the player
      int healAmount =
          player->getMaxHealth() / 4;   // Heal for 25% of max health
      player->takeDamage(-healAmount);  // Negative damage for healing
      cout << "Healed for " << healAmount << " health!" << endl;
      if (player->getHealth() > player->getMaxHealth()) {
        player->setHealth(player->getMaxHealth());  // Cap at max health
      }

      // Remove the orb
      mapManager->removeHealOrb();
    }
  }
}

void GameManager::handleInventoryInput(const sf::Event& event) {
  auto& inv = player->getSkillInventory();
  int count = static_cast<int>(inv.size());
  if (event.key.code == sf::Keyboard::Escape ||
      event.key.code == sf::Keyboard::I) {
    // close inventory
    startTransition(GameState::HubWorld);
  } else if (count > 0) {
    if (event.key.code == sf::Keyboard::Up) {
      inventorySelectionIndex = (inventorySelectionIndex - 1 + count) % count;
    } else if (event.key.code == sf::Keyboard::Down) {
      inventorySelectionIndex = (inventorySelectionIndex + 1) % count;
    } else if (event.key.code == sf::Keyboard::Return) {
      // equip the selected skill
      SkillType toEquip = inv[inventorySelectionIndex];
      player->equipSkill(toEquip);
    }
  }
}

void GameManager::handleBeforeFightInput(const sf::Event& event) {
  // In the demo version, any key press returns to hub world
  if (event.key.code == sf::Keyboard::F) {
    startBattle(currentDoorIndex);
    startTransition(GameState::Battle);
    cout << "Starting battle..." << endl;
    // Reset the selected battle action
    selectedBattleAction = 0;
    startBattle(currentDoorIndex);

    // Print enemy stats for debugging
    cout << "Enemy HP: " << currentEnemy->getHealth() << "/"
         << currentEnemy->getMaxHealth() << endl;
    cout << "Enemy Damage: " << currentEnemy->getBaseDamage() << std::endl;

  } else if (event.key.code == sf::Keyboard::R ||
             event.key.code == sf::Keyboard::Escape) {
    cout << "Retreating from battle..." << endl;
    startTransition(GameState::HubWorld);
  }
}

void GameManager::handleSaveSlotMenuInput(const sf::Event& event) {
  if (event.key.code == sf::Keyboard::Up)
    selectedSlotOption = (selectedSlotOption + 2) % 3;
  else if (event.key.code == sf::Keyboard::Down)
    selectedSlotOption = (selectedSlotOption + 1) % 3;
  else if (event.key.code == sf::Keyboard::Escape)
    currentState = GameState::MainMenu;
  else if (event.key.code == sf::Keyboard::Enter) {
    saveGame(selectedSlotOption + 1);
    currentState = GameState::MainMenu;
  }
}

void GameManager::handleLoadSlotMenuInput(const sf::Event& event) {
  auto slots = getExistingSaves();
  if (event.key.code == sf::Keyboard::Up)
    selectedSlotOption = (selectedSlotOption + 2) % 3;
  else if (event.key.code == sf::Keyboard::Down)
    selectedSlotOption = (selectedSlotOption + 1) % 3;
  else if (event.key.code == sf::Keyboard::Escape)
    currentState = GameState::MainMenu;
  else if (event.key.code == sf::Keyboard::Enter) {
    int slot = selectedSlotOption + 1;
    if (std::find(slots.begin(), slots.end(), slot) != slots.end()) {
      loadGame(slot);
      currentState = GameState::MainMenu;  // Or to HubWorld, if you prefer!
    }
  }
}

void GameManager::update(float deltaTime) {
  // Handle battle logic
  if (currentState == GameState::Battle) {
    // Check if battle is over
    if (battleSystem.isBattleOver()) {
      if (battleSystem.isPlayerVictorious()) {
        // Player won the battle
        cout << "Enemy defeated!" << endl;

        // Award experience
        auto enemy = currentEnemy;
        string skillName = "";
        if (enemy) {
          int expGained = enemy->getRewardExp();
          player->gainExp(expGained);
          cout << "Gained " << expGained << " experience points!" << endl;

          // Check for level up
          player->checkLevelUp();

          skillName = enemy->rollRewardSkill();
          cout << "Skill string from enemy: " << skillName << endl;
          if (!skillName.empty() && skillName != "None") {
            SkillType type = stringToSkillType(skillName);
            cout << "[DEBUG] SkillType enum value: " << static_cast<int>(type)
                 << endl;
            player->addToInventory(type);
            cout << "You learned a new skill: " << skillName << "!" << endl;
          }
        }

        // Set battle result variables for popup
        lastBattleWon = true;
        lastBattleExp = enemy->getRewardExp();
        lastBattleSkill = skillName;

        std::cout << "[DEBUG] isFinalBossRoom = " << dungeon.getBossDoorIndex()
                  << std::endl;
        if (currentDoorIndex == dungeon.getBossDoorIndex()) {
          endBattle(true);
          startTransition(GameState::EndGame);  // <--- TRIGGER END SCREEN!
          return;
        }

        // End battle with victory and return to hub world
        endBattle(true);
        startTransition(GameState::PopupResult);
        return;
      } else {
        // Player was defeated
        cout << "Player defeated!" << endl;

        // Restore player's health
        player->takeDamage(-player->getMaxHealth());  // Full heal
        if (currentEnemy) {
          currentEnemy->setHealth(currentEnemy->getMaxHealth());
        }

        if (currentDoorIndex == dungeon.getBossDoorIndex()) {
          if (auto bossPtr = dynamic_pointer_cast<Boss>(currentEnemy)) {
            bossPtr->setHealth(bossPtr->getMaxHealth());
            cout << "You have been defeated by the final boss!" << std::endl;
          }
        }

        // Set battle result variables for popup
        lastBattleWon = false;
        lastBattleExp = 0;
        lastBattleSkill = "";

        // Go to popup
        startTransition(GameState::PopupResult);
        return;
      }
    }
  }

  // Run the battle system turn if it's not the player's turn
  if (!battleSystem.isPlayerTurn()) {
    battleSystem.runTurn();
  }
}

void GameManager::render() {
  // Clear with appropriate background color
  if (currentState == GameState::MainMenu) {
    window.clear(sf::Color(33, 33, 33));  // Dark gray for main menu
  } else if (currentState == GameState::BeforeFight) {
    window.clear(sf::Color(20, 30, 50));  // Dark blue for before fight screen
  } else if (currentState == GameState::Battle) {
    window.clear(sf::Color(40, 20, 60));  // Purple for battle screen
  } else {
    window.clear(sf::Color(20, 20, 30));  // Dark blue for hub world
  }

  // Render based on current state
  switch (currentState) {
    case GameState::MainMenu:
      drawMainMenu();
      break;

    case GameState::SaveMenu:
      drawSaveSlotMenu();  // Draw the save slot menu
      break;

    case GameState::LoadMenu:
      drawLoadSlotMenu();  // Draw the load slot menu
      break;

    case GameState::HubWorld:
      mapManager->drawMap();                // Draw the hub world
      mapManager->drawInteractionPrompt();  // Draw interaction prompts
      break;

    case GameState::BeforeFight:
      drawBeforeFightScreen();  // Draw the enemy stats screen
      break;

    case GameState::Inventory:
      drawInventoryScreen();  // Draw the inventory screen
      break;

    case GameState::Battle:
      drawBattleScreen();  // Draw the battle screen
      break;

    case GameState::PopupResult:
      drawBattleResultPopup();  // Draw the battle result popup
      break;

    case GameState::EndGame:
      drawEndScreen();  // Draw the end game screen
    default:
      break;
  }

  window.display();
}

void GameManager::drawMainMenu() {
  // Title
  sf::Text titleText("TURN-BASED STRATEGY GAME", font, 60);
  titleText.setStyle(sf::Text::Bold);
  titleText.setFillColor(sf::Color::White);
  titleText.setPosition(
      (window.getSize().x - titleText.getGlobalBounds().width) / 2, 100);
  window.draw(titleText);

  // Menu option: New Game
  sf::Text newGameText(menuOptions[0], font, 30);
  newGameText.setFillColor(selectedMenuOption == 0 ? sf::Color(255, 165, 0)
                                                   : sf::Color::White);
  newGameText.setPosition(
      (window.getSize().x - newGameText.getGlobalBounds().width) / 2, 300);

  // Menu option: Save Game
  sf::Text saveText(menuOptions[1], font, 30);
  saveText.setFillColor(selectedMenuOption == 1 ? sf::Color(255, 165, 0)
                                                : sf::Color::White);
  saveText.setPosition(
      (window.getSize().x - saveText.getGlobalBounds().width) / 2, 360);

  // Menu option: Load Game
  sf::Text loadText(menuOptions[2], font, 30);
  loadText.setFillColor(selectedMenuOption == 2 ? sf::Color(255, 165, 0)
                                                : sf::Color::White);
  loadText.setPosition(
      (window.getSize().x - loadText.getGlobalBounds().width) / 2, 420);

  // Menu option: Exit
  sf::Text exitText(menuOptions[3], font, 30);
  exitText.setFillColor(selectedMenuOption == 3 ? sf::Color(255, 165, 0)
                                                : sf::Color::White);
  exitText.setPosition(
      (window.getSize().x - exitText.getGlobalBounds().width) / 2, 480);

  // Button backgrounds
  sf::RectangleShape newGameBg(
      sf::Vector2f(newGameText.getGlobalBounds().width + 40, 50));
  newGameBg.setFillColor(sf::Color(70, 70, 70, 200));
  newGameBg.setPosition(newGameText.getPosition().x - 20,
                        newGameText.getPosition().y - 10);

  sf::RectangleShape saveBg(
      sf::Vector2f(saveText.getGlobalBounds().width + 40, 50));
  saveBg.setFillColor(sf::Color(70, 70, 70, 200));
  saveBg.setPosition(saveText.getPosition().x - 20,
                     saveText.getPosition().y - 10);

  sf::RectangleShape loadBg(
      sf::Vector2f(loadText.getGlobalBounds().width + 40, 50));
  loadBg.setFillColor(sf::Color(70, 70, 70, 200));
  loadBg.setPosition(loadText.getPosition().x - 20,
                     loadText.getPosition().y - 10);

  sf::RectangleShape exitBg(
      sf::Vector2f(exitText.getGlobalBounds().width + 40, 50));
  exitBg.setFillColor(sf::Color(70, 70, 70, 200));
  exitBg.setPosition(exitText.getPosition().x - 20,
                     exitText.getPosition().y - 10);

  // Draw button backgrounds first
  window.draw(newGameBg);
  window.draw(saveBg);
  window.draw(loadBg);
  window.draw(exitBg);

  // Then draw text
  window.draw(newGameText);
  window.draw(saveText);
  window.draw(loadText);
  window.draw(exitText);

  // Navigation instructions
  sf::Text instructionsText("Use Arrow Keys to Navigate", font, 20);
  instructionsText.setFillColor(sf::Color(200, 200, 200));
  instructionsText.setPosition(
      (window.getSize().x - instructionsText.getGlobalBounds().width) / 2, 600);
  window.draw(instructionsText);

  sf::Text enterText("Press Enter to Select", font, 20);
  enterText.setFillColor(sf::Color(200, 200, 200));
  enterText.setPosition(
      (window.getSize().x - enterText.getGlobalBounds().width) / 2, 630);
  window.draw(enterText);
}

void GameManager::drawBeforeFightScreen() {
  // Draw a dark background
  sf::RectangleShape background(
      sf::Vector2f(window.getSize().x, window.getSize().y));
  background.setFillColor(
      sf::Color(20, 30, 50, 255));  // Dark blue for before fight screen
  window.draw(background);

  // Draw title
  sf::Text titleText("Enemy Encountered!", font, 40);
  titleText.setStyle(sf::Text::Bold);
  titleText.setFillColor(sf::Color::White);
  titleText.setPosition(
      (window.getSize().x - titleText.getGlobalBounds().width) / 2, 80);
  window.draw(titleText);

  // Create a placeholder enemy if we don't have one yet
  if (!currentEnemy) {
    return;
  }

  // Draw enemy stats in a box
  sf::RectangleShape statsBox(sf::Vector2f(500, 300));
  statsBox.setFillColor(sf::Color(50, 50, 80, 230));
  statsBox.setOutlineColor(sf::Color::White);
  statsBox.setOutlineThickness(2);
  statsBox.setPosition((window.getSize().x - 500) / 2, 150);
  window.draw(statsBox);

  // Draw enemy image
  sf::Sprite enemySprite(enemyBattleTexture);

  // Scale enemy sprite to fit nicely in the box
  float enemyScale = 100.0f / max(enemyBattleTexture.getSize().x,
                                       enemyBattleTexture.getSize().y);
  enemySprite.setScale(enemyScale, enemyScale);

  // Position the sprite in the top portion of the box
  enemySprite.setPosition(
      statsBox.getPosition().x +
          (statsBox.getSize().x - enemySprite.getGlobalBounds().width) / 2,
      statsBox.getPosition().y + 30);

  window.draw(enemySprite);

  // Enemy difficulty color
  sf::Color difficultyColor;
  string difficultyText;

  switch (currentEnemy->getDifficulty()) {
    case Difficulty::Easy:
      difficultyColor = sf::Color::Green;
      difficultyText = "Easy";
      break;
    case Difficulty::Medium:
      difficultyColor = sf::Color::Yellow;
      difficultyText = "Medium";
      break;
    case Difficulty::Hard:
      difficultyColor = sf::Color::Red;
      difficultyText = "Hard";
      break;
    default:
      difficultyColor = sf::Color::White;
      difficultyText = "Unknown";
      break;
  }

  // Draw enemy info
  string enemyInfo = "Enemy: " + currentEnemy->getName() + "\n";
  enemyInfo += "Difficulty: " + difficultyText + "\n";
  enemyInfo += "HP: " + to_string(currentEnemy->getHealth()) + "/" +
               to_string(currentEnemy->getMaxHealth()) + "\n";
  enemyInfo += "Damage: " + to_string(currentEnemy->getBaseDamage()) + "\n";

  // Convert resistance type to string
  string resistanceText;
  switch (currentEnemy->getResistance()) {
    case ResistanceType::Physical:
      resistanceText = "Physical";
      break;
    case ResistanceType::Magical:
      resistanceText = "Magical";
      break;
    case ResistanceType::None:
      resistanceText = "None";
      break;
    default:
      resistanceText = "Unknown";
      break;
  }
  enemyInfo += "Resistance: " + resistanceText + "\n";

  sf::Text demoInfo(enemyInfo, font, 22);
  demoInfo.setFillColor(sf::Color::White);
  demoInfo.setPosition(
      statsBox.getPosition().x +
          (statsBox.getSize().x - demoInfo.getGlobalBounds().width) / 2,
      statsBox.getPosition().y + 150);
  window.draw(demoInfo);

  // Draw choice buttons
  sf::RectangleShape fightButton(sf::Vector2f(150, 60));
  fightButton.setFillColor(sf::Color(200, 50, 50, 200));
  fightButton.setPosition((window.getSize().x / 2) - 170, 500);
  window.draw(fightButton);

  sf::Text fightText("FIGHT", font, 30);
  fightText.setFillColor(sf::Color::White);
  fightText.setPosition(
      fightButton.getPosition().x +
          (fightButton.getSize().x - fightText.getGlobalBounds().width) / 2,
      fightButton.getPosition().y +
          (fightButton.getSize().y - fightText.getGlobalBounds().height) / 2 -
          5);
  window.draw(fightText);

  sf::RectangleShape fleeButton(sf::Vector2f(150, 60));
  fleeButton.setFillColor(sf::Color(50, 50, 200, 200));
  fleeButton.setPosition((window.getSize().x / 2) + 20, 500);
  window.draw(fleeButton);

  sf::Text fleeText("Exit", font, 30);
  fleeText.setFillColor(sf::Color::White);
  fleeText.setPosition(
      fleeButton.getPosition().x +
          (fleeButton.getSize().x - fleeText.getGlobalBounds().width) / 2,
      fleeButton.getPosition().y +
          (fleeButton.getSize().y - fleeText.getGlobalBounds().height) / 2 - 5);
  window.draw(fleeText);

  // Draw instructions
  sf::Text instructions("Press F to FIGHT or R to RETREAT", font, 20);
  instructions.setFillColor(sf::Color(200, 200, 200));
  instructions.setPosition(
      (window.getSize().x - instructions.getGlobalBounds().width) / 2, 580);
  window.draw(instructions);
}

void GameManager::drawInventoryScreen() {
  window.clear(sf::Color(40, 40, 60));  // Dark bluish for inventory

  // Title
  sf::Text title("INVENTORY", font, 40);
  title.setFillColor(sf::Color::White);
  title.setPosition(50, 30);
  window.draw(title);

  // Show currently equipped skill
  sf::Text eqLabel("Equipped: ", font, 24);
  eqLabel.setFillColor(sf::Color::White);
  eqLabel.setPosition(50, 100);
  window.draw(eqLabel);

  SkillType eq = player->getEquippedSkill();
  string eqName = (eq != SkillType::None) ? getSkillData(eq).name : "None";
  sf::Text eqText(eqName, font, 24);
  eqText.setFillColor(sf::Color::Yellow);
  eqText.setPosition(200, 100);
  window.draw(eqText);

  const auto& inv = player->getSkillInventory();
  float startY = 160.f;
  for (int i = 0; i < (int)inv.size(); ++i) {
    const auto& name = getSkillData(inv[i]).name;
    sf::Text skillEntry(name, font, 28);
    skillEntry.setPosition(80, startY + i * 40);

    // Highlight: show background for selected skill
    if (i == inventorySelectionIndex) {
      sf::RectangleShape highlightBg(
          sf::Vector2f(300, 36));  // Adjust size as needed
      highlightBg.setFillColor(
          sf::Color(255, 215, 0, 180));  // Gold, semi-transparent
      highlightBg.setPosition(75, startY + i * 40 - 2);  // Align with text
      window.draw(highlightBg);
    }

    skillEntry.setFillColor(sf::Color::White);
    window.draw(skillEntry);

    // Optional: draw an arrow or '>' marker next to selected
    if (i == inventorySelectionIndex) {
      sf::Text marker(">", font, 28);
      marker.setFillColor(sf::Color::Yellow);
      marker.setPosition(55, startY + i * 40);
      window.draw(marker);
    }
  }

  // Instructions
  sf::Text instr("Up/Down to navigate, Press ESC or I to return", font, 18);
  instr.setFillColor(sf::Color(200, 200, 200));
  instr.setPosition(50, window.getSize().y - 40);
  window.draw(instr);

  window.display();
}

void GameManager::drawBattleScreen() {
  // Get window dimensions
  float windowWidth = window.getSize().x;
  float windowHeight = window.getSize().y;

  // Main background
  sf::RectangleShape background(sf::Vector2f(windowWidth, windowHeight));
  background.setFillColor(sf::Color(40, 20, 60));  // Dark purple for battle
  window.draw(background);

  // Battle title
  sf::Text battleTitle("BATTLE", font, 50);
  battleTitle.setStyle(sf::Text::Bold);
  battleTitle.setFillColor(sf::Color::White);
  battleTitle.setPosition(
      (windowWidth - battleTitle.getGlobalBounds().width) / 2,
      windowHeight * 0.05f  // 5% from top
  );
  window.draw(battleTitle);

  // Calculate box dimensions relative to screen size
  float boxWidth = windowWidth * 0.35f;   // 35% of screen width
  float boxHeight = windowHeight * 0.4f;  // 40% of screen height
  float leftBoxX = windowWidth * 0.05f;   // 5% from left
  float rightBoxX = windowWidth * 0.6f;   // 60% from left
  float boxY = windowHeight * 0.15f;      // 15% from top

  // Draw player box on the left
  sf::RectangleShape playerBox(sf::Vector2f(boxWidth, boxHeight));
  playerBox.setFillColor(sf::Color(50, 80, 100, 230));
  playerBox.setOutlineColor(sf::Color::White);
  playerBox.setOutlineThickness(2);
  playerBox.setPosition(leftBoxX, boxY);
  window.draw(playerBox);

  // Draw player image using texture
  sf::Sprite playerSprite(playerBattleTexture);

  // Scale player sprite to fit nicely in the box
  float playerScale = boxHeight * 0.5f /
                      max(playerBattleTexture.getSize().x,
                               playerBattleTexture.getSize().y);
  playerSprite.setScale(playerScale, playerScale);

  // Position the sprite in the left portion of the box
  playerSprite.setPosition(
      playerBox.getPosition().x + boxWidth * 0.1f,  // 10% from left edge of box
      playerBox.getPosition().y +
          (boxHeight - playerSprite.getGlobalBounds().height) /
              2  // Vertical center
  );

  window.draw(playerSprite);

  // Draw player stats in the right portion of the box
  string playerStats = player->getName() + "\n";
  playerStats += "HP: " + to_string(player->getHealth()) + "/" +
                 std::to_string(player->getMaxHealth()) + "\n";
  playerStats += "Exp: " + to_string(player->getExp()) + "/35\n";
  playerStats += "Level: " + to_string(player->getLevel());

  sf::Text playerText(playerStats, font, 24);
  playerText.setFillColor(sf::Color::White);
  playerText.setPosition(
      playerBox.getPosition().x + boxWidth * 0.5f,   // 50% of box width
      playerBox.getPosition().y + boxHeight * 0.25f  // 25% from top of box
  );
  window.draw(playerText);

  // Draw enemy box on the right
  sf::RectangleShape enemyBox(sf::Vector2f(boxWidth, boxHeight));
  enemyBox.setFillColor(sf::Color(100, 50, 80, 230));
  enemyBox.setOutlineColor(sf::Color::White);
  enemyBox.setOutlineThickness(2);
  enemyBox.setPosition(rightBoxX, boxY);
  window.draw(enemyBox);

  // Get enemy from battle system
  Enemy* battleEnemy = battleSystem.getEnemy();

  // Draw enemy stats and image if there is an enemy
  if (battleEnemy) {
    // Draw enemy image
    sf::Sprite enemySprite(enemyBattleTexture);

    // Scale enemy sprite to fit nicely in the box
    float enemyScale = boxHeight * 0.5f /
                       max(enemyBattleTexture.getSize().x,
                                enemyBattleTexture.getSize().y);
    enemySprite.setScale(enemyScale, enemyScale);

    // Position the sprite in the left portion of the box
    enemySprite.setPosition(
        enemyBox.getPosition().x +
            boxWidth * 0.1f,  // 10% from left edge of box
        enemyBox.getPosition().y +
            (boxHeight - enemySprite.getGlobalBounds().height) /
                2  // Vertical center
    );

    window.draw(enemySprite);

    // Draw enemy stats
    std::string enemyStats = battleEnemy->getName() + "\n";
    enemyStats += "HP: " + to_string(battleEnemy->getHealth()) + "/" +
                  to_string(battleEnemy->getMaxHealth()) + "\n";
    enemyStats +=
        "Damage: " + to_string(battleEnemy->getBaseDamage()) + "\n";

    sf::Text enemyText(enemyStats, font, 24);
    enemyText.setFillColor(sf::Color::White);
    enemyText.setPosition(
        enemyBox.getPosition().x + boxWidth * 0.5f,   // 50% of box width
        enemyBox.getPosition().y + boxHeight * 0.25f  // 25% from top of box
    );
    window.draw(enemyText);
  }

  // Draw action menu at the bottom
  float actionBoxWidth = windowWidth * 0.5f;    // 50% of screen width
  float actionBoxHeight = windowHeight * 0.3f;  // 30% of screen height

  sf::RectangleShape actionBox(sf::Vector2f(actionBoxWidth, actionBoxHeight));
  actionBox.setFillColor(sf::Color(60, 60, 80, 230));
  actionBox.setOutlineColor(sf::Color::White);
  actionBox.setOutlineThickness(2);
  actionBox.setPosition(
      (windowWidth - actionBoxWidth) / 2,  // Center horizontally
      windowHeight * 0.6f                  // 60% from top
  );
  window.draw(actionBox);

  // Draw action options - using action names from BattleSystem
  const vector<string>& actionNames = battleSystem.getActionNames();
  float actionTextX = actionBox.getPosition().x +
                      actionBoxWidth * 0.1f;  // 10% from left of action box
  float actionTextY = actionBox.getPosition().y +
                      actionBoxHeight * 0.1f;     // 10% from top of action box
  float actionSpacing = actionBoxHeight * 0.15f;  // Spacing between actions

  for (int i = 0; i < static_cast<int>(actionNames.size()); i++) {
    sf::Text actionText(to_string(i + 1) + ". " + actionNames[i], font,
                        24);

    // Highlight selected action
    if (i == battleSystem.getSelectedAction()) {
      actionText.setFillColor(sf::Color::Yellow);

      // Draw selection indicator
      sf::CircleShape selector(8);
      selector.setFillColor(sf::Color::Yellow);
      selector.setPosition(actionTextX - 20,
                           actionTextY + i * actionSpacing + 8);
      window.draw(selector);
    } else {
      actionText.setFillColor(sf::Color::White);
    }

    actionText.setPosition(actionTextX, actionTextY + i * actionSpacing);
    window.draw(actionText);
  }

  // Display turn information
  string turnText = battleSystem.isPlayerTurn()
                             ? "Your turn! Select an action."
                             : "Enemy's turn...";
  sf::Text turnInfo(turnText, font, 20);
  turnInfo.setFillColor(sf::Color::White);
  turnInfo.setPosition((windowWidth - turnInfo.getGlobalBounds().width) / 2,
                       actionBox.getPosition().y - 30);
  window.draw(turnInfo);

  // Instructions
  sf::Text instructions("Use arrow keys to select action, Enter to confirm",
                        font, 20);
  instructions.setFillColor(sf::Color(200, 200, 200));
  instructions.setPosition(
      (windowWidth - instructions.getGlobalBounds().width) / 2,
      windowHeight - 40);
  window.draw(instructions);
}

void GameManager::drawEndScreen() {
  window.clear(sf::Color(30, 10, 40));  // Any background color you like

  sf::Text congrats("CONGRATULATIONS!", font, 60);
  congrats.setFillColor(sf::Color::Yellow);
  congrats.setStyle(sf::Text::Bold);
  congrats.setPosition(
      (window.getSize().x - congrats.getGlobalBounds().width) / 2, 200);
  window.draw(congrats);

  sf::Text info("You have defeated the boss and finished the game!", font, 32);
  info.setFillColor(sf::Color::White);
  info.setPosition((window.getSize().x - info.getGlobalBounds().width) / 2,
                   320);
  window.draw(info);

  sf::Text instruction("Press any key to return to the Main Menu", font, 28);
  instruction.setFillColor(sf::Color(200, 200, 200));
  instruction.setPosition(
      (window.getSize().x - instruction.getGlobalBounds().width) / 2, 500);
  window.draw(instruction);

  window.display();
}

// Draw Save Slot Selection
void GameManager::drawSaveSlotMenu() {
  sf::Text title("Select a Slot to Save", font, 40);
  title.setFillColor(sf::Color::White);
  title.setPosition((window.getSize().x - title.getGlobalBounds().width) / 2,
                    150);
  window.draw(title);

  for (int i = 0; i < 3; ++i) {
    string label = "Slot " + std::to_string(i + 1);
    sf::Text slotText(label, font, 32);
    slotText.setFillColor(selectedSlotOption == i ? sf::Color(255, 165, 0)
                                                  : sf::Color::White);
    slotText.setPosition(
        (window.getSize().x - slotText.getGlobalBounds().width) / 2,
        270 + i * 70);
    window.draw(slotText);
  }
  sf::Text instr("Up/Down: Select Slot, Enter: Confirm, Esc: Back", font, 20);
  instr.setFillColor(sf::Color(200, 200, 200));
  instr.setPosition((window.getSize().x - instr.getGlobalBounds().width) / 2,
                    550);
  window.draw(instr);
}

// Draw Load Slot Selection
void GameManager::drawLoadSlotMenu() {
  sf::Text title("Select a Slot to Load", font, 40);
  title.setFillColor(sf::Color::White);
  title.setPosition((window.getSize().x - title.getGlobalBounds().width) / 2,
                    150);
  window.draw(title);

  auto slots = getExistingSaves();  // e.g., {1,3}
  for (int i = 0; i < 3; ++i) {
    bool exists = find(slots.begin(), slots.end(), i + 1) != slots.end();
    std::string label =
        "Slot " + to_string(i + 1) + (exists ? "" : " (empty)");
    sf::Text slotText(label, font, 32);
    if (!exists)
      slotText.setFillColor(sf::Color(100, 100, 100));  // grey out
    else
      slotText.setFillColor(selectedSlotOption == i ? sf::Color(255, 165, 0)
                                                    : sf::Color::White);
    slotText.setPosition(
        (window.getSize().x - slotText.getGlobalBounds().width) / 2,
        270 + i * 70);
    window.draw(slotText);
  }
  sf::Text instr("Up/Down: Select Slot, Enter: Confirm, Esc: Back", font, 20);
  instr.setFillColor(sf::Color(200, 200, 200));
  instr.setPosition((window.getSize().x - instr.getGlobalBounds().width) / 2,
                    550);
  window.draw(instr);
}

void GameManager::drawBattleResultPopup() {
  // Draw semi-transparent background
  sf::RectangleShape bg(sf::Vector2f(window.getSize().x, window.getSize().y));
  bg.setFillColor(sf::Color(0, 0, 0, 180));
  window.draw(bg);

  // Popup box
  sf::RectangleShape box(sf::Vector2f(500, 300));
  box.setFillColor(sf::Color(70, 70, 120, 230));
  box.setOutlineColor(sf::Color::White);
  box.setOutlineThickness(3);
  box.setPosition((window.getSize().x - 500) / 2,
                  (window.getSize().y - 300) / 2);
  window.draw(box);

  float x = box.getPosition().x + 40;
  float y = box.getPosition().y + 30;

  sf::Text title("", font, 34);
  title.setStyle(sf::Text::Bold);
  title.setPosition(x, y);

  sf::Text content("", font, 26);
  content.setPosition(x, y + 60);

  if (lastBattleWon) {
    title.setString("Victory!");
    title.setFillColor(sf::Color::Yellow);
    window.draw(title);

    string txt = "Congratulations! You won the battle.\n";
    txt += "You gained " + to_string(lastBattleExp) + " EXP.\n";
    if (!lastBattleSkill.empty() && lastBattleSkill != "None") {
      txt += "You learned a new skill: " + lastBattleSkill + "!";
    }
    content.setString(txt);
    content.setFillColor(sf::Color::White);
    window.draw(content);
  } else {
    title.setString("Defeat");
    title.setFillColor(sf::Color::Red);
    window.draw(title);

    content.setString("You lost the battle.\nBetter luck next time!");
    content.setFillColor(sf::Color::White);
    window.draw(content);
  }

  // Continue hint
  sf::Text continueText("Press any key to continue...", font, 20);
  continueText.setFillColor(sf::Color(200, 200, 200));
  continueText.setPosition(
      (window.getSize().x - continueText.getGlobalBounds().width) / 2,
      box.getPosition().y + box.getSize().y - 50);
  window.draw(continueText);
}

void GameManager::startTransition(GameState newState) {
  cout << "Transitioning from ";
  // Print the current state
  switch (currentState) {
    case GameState::MainMenu:
      cout << "MAIN_MENU";
      break;
    case GameState::HubWorld:
      cout << "HUB_WORLD";
      break;
    case GameState::BeforeFight:
      cout << "BEFORE_FIGHT";
      break;
    case GameState::Battle:
      cout << "BATTLE";
      break;
    case GameState::Inventory:
      cout << "INVENTORY";
      break;
    case GameState::GameOver:
      cout << "GAME_OVER";
      break;
    case GameState::EndGame:
      cout << "END_GAME";
      break;
    default:
      cout << "UNKNOWN";
      break;
  }

  cout << " to ";

  // Print the new state
  switch (newState) {
    case GameState::MainMenu:
      cout << "MAIN_MENU";
      break;
    case GameState::HubWorld:
      cout << "HUB_WORLD";
      break;
    case GameState::BeforeFight:
      cout << "BEFORE_FIGHT";
      break;
    case GameState::Battle:
      cout << "BATTLE";
      break;
    case GameState::Inventory:
      cout << "INVENTORY";
      break;
    case GameState::GameOver:
      cout << "GAME_OVER";
      break;

    case GameState::EndGame:
      cout << "END_GAME";
      break;
    default:
      cout << "UNKNOWN";
      break;
  }

  cout << endl;

  // State-specific setup when transitioning
  if (newState == GameState::HubWorld && currentState == GameState::MainMenu) {
    // Reset player position when entering hub world from main menu
    const float tileSize = mapManager->getTileSize();
    const float gridOffsetX = mapManager->getGridOffsetX();
    const float gridOffsetY = mapManager->getGridOffsetY();

    float centerX = gridOffsetX + 5 * tileSize + tileSize / 4;
    float centerY = gridOffsetY + 5 * tileSize + tileSize / 4;

    mapManager->setPlayerPosition(centerX, centerY);
  }

  // Change state
  currentState = newState;
}

Player* GameManager::getPlayer() { return player; }

void GameManager::startBattle(int doorIndex) {
  cout << "Starting battle with door: " << doorIndex << endl;

  //Reset all effects from the last battles
  player->resetStatusEffects(); 
  currentEnemy->resetStatusEffects();
  
  // Initialize battle system with player and enemy
  battleSystem.fight(*player, *currentEnemy);

  // Keep local tracking variable in sync
  selectedBattleAction = battleSystem.getSelectedAction();
}

void GameManager::endBattle(bool playerWon) {
  // Set player out of battle
  player->setInBattle(false);

  // Reset battle system
  battleSystem.reset();
  if (!(currentDoorIndex == dungeon.getBossDoorIndex())) {
    // regenerate just this door’s enemy
    dungeon.regenerateEnemyAt(currentDoorIndex);
    cout << "A new enemy now guards door " << currentDoorIndex << endl;
  } else {
    // boss defeated
    dungeon.advanceRoom();
  }

  currentEnemy.reset();
}

#include <fstream>

void GameManager::saveGame(int slot) {
  std::ofstream file("save" + to_string(slot) + ".txt");
  player->saveToStream(file);
  file << currentDoorIndex << "\n";
  // Add more state as needed!
}

bool GameManager::loadGame(int slot) {
  ifstream file("save" + to_string(slot) + ".txt");
  if (!file) return false;
  player->loadFromStream(file);
  file >> currentDoorIndex;
  // Add more state as needed!
  return true;
}

// Utility: return slots that exist (1-based, up to 3 for this example)
vector<int> GameManager::getExistingSaves() const {
  vector<int> slots;
  for (int i = 1; i <= 3; ++i) {
    ifstream file("save" + to_string(i) + ".txt");
    if (file.good()) slots.push_back(i);
  }
  return slots;
}