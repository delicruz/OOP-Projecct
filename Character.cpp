#include "Character.h"
#include <iostream>
using namespace std;

Character::Character(string name, int maxHp) : name(name), maxHealth(maxHp), health(maxHp), isDefending(false), 
    isTakingDOT(false), dotTurns(0), dotDamage(0), isStunned(false) {}

void Character::takeDamage(int amount) {
    if (isDefending) amount = 0;
    health -= amount;
    if (health < 0) health = 0;
}

bool Character::isAlive() const {
    return health > 0;
}

void Character::setDefenseState(bool state) {
    isDefending = state;
}

bool Character::isDefendingFunc() const {
    return isDefending;
}

void Character::applyDOT(int damage, int turns) {
    isTakingDOT = true;
    dotDamage = damage;
    dotTurns = turns;
}

void Character::applyStun() {
    isStunned = true;
}

void Character::processStatusEffect() {
    if (isTakingDOT && dotTurns > 0) {
        health -= dotDamage;
        dotTurns--;
        if (dotTurns == 0) {
            isTakingDOT = false;
        }
    }
    if (isStunned) {
        isStunned = false;
    }
}

int Character::getHealth() const {
    return health;
}

int Character::getMaxHealth() const {
    return maxHealth;
}

string Character::getName() const {
    return name;
}