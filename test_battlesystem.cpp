#include <cassert>
#include <iostream>
#include "BattleSystem.h"
#include "Player.h"
#include "Enemy.h"
#include "Skill.h"

void testStartFightAndTurns() {
    Player player("Hero", 100);
    Enemy enemy("Slime", Difficulty::Easy, AbilityType::NormalAttack, ResistanceType::Physical, 0);

    BattleSystem bs;
    bs.fight(player, enemy);

    // Player's turn: should attack enemy
    int enemyStart = enemy.getHealth();
    bs.runTurn();
    assert(enemy.getHealth() < enemyStart);

    // Enemy's turn: should attack player
    int playerStart = player.getHealth();
    bs.runTurn();
    assert(player.getHealth() < playerStart);

    std::cout << "testStartFightAndTurns passed.\n";
}

void testBattleEnds() {
    Player player("Hero", 100);
    Enemy enemy("Slime", Difficulty::Easy, AbilityType::NormalAttack, ResistanceType::Physical, 0);

    BattleSystem bs;
    bs.fight(player, enemy);

    // Reduce enemy to 0 health
    enemy.setHealth(1);
    bs.runTurn(); // Player's turn, should finish enemy
    assert(bs.isBattleOver());
    assert(bs.isPlayerVictorious());

    std::cout << "testBattleEnds passed.\n";
}

void testTurnSwitching() {
    Player player("Hero", 100);
    Enemy enemy("Slime", Difficulty::Easy, AbilityType::NormalAttack, ResistanceType::Physical, 0);

    BattleSystem bs;
    bs.fight(player, enemy);

    bool playerStarted = true;

    for (int i = 0; i < 6; ++i) {
        if (playerStarted) {
            int enemyStart = enemy.getHealth();
            bs.runTurn();
            assert(enemy.getHealth() <= enemyStart);
        } else {
            int playerStart = player.getHealth();
            bs.runTurn();
            assert(player.getHealth() <= playerStart);
        }
        playerStarted = !playerStarted;
    }

    std::cout << "testTurnSwitching passed.\n";
}

int main() {
    testStartFightAndTurns();
    testBattleEnds();
    testTurnSwitching();
    std::cout << "All BattleSystem unit tests passed!\n";
    return 0;
}
