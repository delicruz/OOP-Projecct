#include <cassert>
#include <iostream>
#include <vector>
#include <SFML/System.hpp> // Needed for sf::Vector2f
#include "Dungeon.h"
#include "Enemy.h"

void testConstruction() {
    Dungeon d;
    assert(d.getCurrentRoomIndex() == 0);
    assert(d.getTotalRooms() > 0);
    std::cout << "testConstruction passed.\n";
}

void testAssignEnemiesAndGetEnemy() {
    Dungeon d;
    std::vector<sf::Vector2f> doorPositions = { {0,0}, {1,0}, {2,0}, {3,0} };
    d.assignEnemiesToDoors(doorPositions);

    for (int i = 0; i < 4; ++i) {
        auto enemy = d.getEnemyAt(i);
        assert(enemy != nullptr);
    }
    // Final boss should not be null
    auto boss = d.getEnemyAt(999);
    assert(boss != nullptr);
    std::cout << "testAssignEnemiesAndGetEnemy passed.\n";
}


void testRegenerateEnemyAt() {
    Dungeon d;
    std::vector<sf::Vector2f> doorPositions = { {0,0}, {1,0}, {2,0}, {3,0} };
    d.assignEnemiesToDoors(doorPositions);

    auto before = d.getEnemyAt(1);
    d.regenerateEnemyAt(1);
    auto after = d.getEnemyAt(1);

    // Enemy pointer will change (regenerated)
    assert(before != nullptr && after != nullptr && before != after);
    std::cout << "testRegenerateEnemyAt passed.\n";
}

void testResetDungeon() {
    Dungeon d;
    std::vector<sf::Vector2f> doorPositions = { {0,0}, {1,0}, {2,0}, {3,0} };
    d.assignEnemiesToDoors(doorPositions);
    d.advanceRoom();
    d.resetDungeon(doorPositions);
    assert(d.getCurrentRoomIndex() == 0);
    std::cout << "testResetDungeon passed.\n";
}

int main() {
    testConstruction();
    testAssignEnemiesAndGetEnemy();
    testRegenerateEnemyAt();
    testResetDungeon();
    std::cout << "All Dungeon unit tests passed!\n";
    return 0;
}
