// Dungeon.h
#ifndef DUNGEON_H
#define DUNGEON_H

#include <vector>
#include <memory>
#include <string>
#include "Enemy.h"
#include "Boss.h"
#include "Enums.h"
#include "Skill.h"
#include <SFML/Graphics.hpp>

using namespace std;

class Dungeon {
private:
    static constexpr int numDoors = 4;
    vector<shared_ptr<Enemy>> roomEnemies;    // enemies for each door
    shared_ptr<Boss> finalBoss;               // boss enemy
    int currentRoomIndex;

    // Utility: generate a randomized name
    string generateEnemyName();
    // Map SkillType to string
    static string skillTypeToString(SkillType skill);
    // Create a new random enemy; Enemy ctor sets stats internally
    shared_ptr<Enemy> generateRandomEnemy();

public:
    Dungeon();

    // Setup dungeon with one enemy per door
    void assignEnemiesToDoors(const vector<sf::Vector2f>& doorPositions);
    // Get the enemy for the current room, or boss if at final
    shared_ptr<Enemy> getEnemyAt(int doorIndex) const;
    
    void setCurrentRoomIndex(int idx) { currentRoomIndex = idx; }

    // Advance room index; false if already at boss
    int getBossDoorIndex() const { return numDoors; }
    bool advanceRoom();
    int getCurrentRoomIndex() const;
    int getTotalRooms() const;

    // Reset dungeon (shuffle and reset index)
    void resetDungeon(const vector<sf::Vector2f>& doorPositions);
    void regenerateEnemyAt(int doorIndex);


};

#endif