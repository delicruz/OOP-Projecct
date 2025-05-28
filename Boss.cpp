#include "Boss.h"
using namespace std;

Boss::Boss(string name) :
    Enemy(name, Difficulty::Hard, AbilityType::NormalAttack, ResistanceType::Magical, 0) {
        maxHealth = 200;
        health = maxHealth;
        baseDamage = 40;
    }

void Boss::attack(Character& target) {
    int abilityChoice = rand() % 4;     // Randomly decide which ability to use from available abilities
    switch (abilityChoice)
    {
    case 0:
        target.takeDamage(baseDamage);
        break;
    case 1:
        target.applyPoison(10, 3);
        break;
    case 2:
        target.applyStun();
        target.takeDamage(baseDamage - 30);
        break;
    default: 
        target.takeDamage(baseDamage);
        break;
    }
}