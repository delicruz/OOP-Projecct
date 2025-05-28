#include "Dungeon.h"
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <iostream>

using namespace std;

Dungeon::Dungeon()
    : currentRoomIndex(0)
{
    srand(static_cast<unsigned int>(time(nullptr)));
}

string Dungeon::skillTypeToString(SkillType skill) {
    switch (skill) {
        case SkillType::Slash:      return "Slash";
        case SkillType::IceSlash:   return "Ice Slash";
        case SkillType::PoisonDart: return "Poison Dart";
        case SkillType::Heal:       return "Heal";
        default:                    return "None";
    }
}

string Dungeon::generateEnemyName() {
    static const vector<string> adjectives = {
        "Sneaky", "Vicious", "Dark", "Wild", "Savage", "Cunning", "Brutal"
    };
    return adjectives[rand() % adjectives.size()];
}

shared_ptr<Enemy> Dungeon::generateRandomEnemy() {
    Difficulty diff = static_cast<Difficulty>(rand() % 3);
    ResistanceType res = static_cast<ResistanceType>(rand() % 3);
    AbilityType abil = static_cast<AbilityType>(rand() % 3);
    string name = generateEnemyName();
    return make_shared<Enemy>(name, diff, abil, res, 0);
}

void Dungeon::assignEnemiesToDoors(const vector<sf::Vector2f>& doorPositions) {
    roomEnemies.clear();
    roomEnemies.reserve(numDoors);
    for (int i = 0; i < numDoors && i < static_cast<int>(doorPositions.size()); ++i) {
        roomEnemies.push_back(generateRandomEnemy());
    }
    // Shuffle to randomize ordering
    random_shuffle(roomEnemies.begin(), roomEnemies.end());

    // Create boss (only name constructor exists)
    finalBoss = make_shared<Boss>("The Dark Overlord");
    currentRoomIndex = 0;
}


shared_ptr<Enemy> Dungeon::getEnemyAt(int doorIndex) const {
    if (doorIndex >= 0 && doorIndex < static_cast<int>(roomEnemies.size())) {
       return roomEnemies[doorIndex];
   }
    // anything else is boss
   return finalBoss;
}

bool Dungeon::advanceRoom() {
    if (currentRoomIndex < numDoors) {
        ++currentRoomIndex;
        return true;
    }
    return false;
}


int Dungeon::getCurrentRoomIndex() const {
    return currentRoomIndex;
}

int Dungeon::getTotalRooms() const {
    return numDoors + 1;
}

void Dungeon::resetDungeon(const vector<sf::Vector2f>& doorPositions) {
    assignEnemiesToDoors(doorPositions);
    currentRoomIndex = 0;
}

void Dungeon::regenerateEnemyAt(int doorIndex) {
    if (doorIndex >= 0 && doorIndex < static_cast<int>(roomEnemies.size())) {
        roomEnemies[doorIndex] = generateRandomEnemy();
    }
}

