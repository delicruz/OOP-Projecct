#include "Enemy.h"
using namespace std;

Enemy::Enemy(string name, Difficulty diff, AbilityType abil, ResistanceType res, string reward, int exp) : 
    Character(name, 0), difficulty(diff), ability(abil), resistance(res), rewardSkill(reward), rewardExp(exp) {
        initializeByDifficulty();
        health = maxHealth;
        rewardExp = exp;
    }

void Enemy::initializeByDifficulty() {
    switch (difficulty) {
        case Difficulty::Easy:
        maxHealth = 50;
        baseDamage = 8;
        rewardExp = 10;
        break;
        case Difficulty::Medium:
        maxHealth = 75;
        baseDamage = 13;
        rewardExp = 25;
        break;
        case Difficulty::Hard:
        maxHealth = 100;
        baseDamage = 18;
        rewardExp = 35;
        break;
    }
}

void Enemy::attack(Character& target) {
    int damage = baseDamage;
    target.takeDamage(damage);
}

void Enemy::defend() {
    setDefenseState(true);    
}

Difficulty Enemy::getDifficulty() const {
    return difficulty;
}

AbilityType Enemy::getAbility() const {
    return ability;
}

ResistanceType Enemy::getResistance() const {
    return resistance;
}

string Enemy::getRewardSkill() const {
    return rewardSkill;
}

int Enemy::getBaseDamage() const {
    return baseDamage;
}

int Enemy::getMaxHealth() const {
    return maxHealth;
}

int Enemy::getRewardExp() const {
    return rewardExp;
}

