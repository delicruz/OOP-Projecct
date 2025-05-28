#include "Enemy.h"
using namespace std;
#include <iostream>
#include <cstdlib>


Enemy::Enemy(string name, Difficulty diff, AbilityType abil, ResistanceType res, int exp) : 
    Character(name, 0), difficulty(diff), ability(abil), resistance(res), rewardExp(exp) {
        initializeByDifficulty();
        health = maxHealth;
        assignAbilities();
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

void Enemy::assignAbilities() {
    if (difficulty == Difficulty::Easy) {
        abilities.push_back(AbilityType::NormalAttack);
    }
    else if (difficulty == Difficulty::Medium) {
        abilities.push_back(AbilityType::NormalAttack);
        abilities.push_back(AbilityType::Poison);
    }
    else if (difficulty == Difficulty::Hard) {
        abilities.push_back(AbilityType::NormalAttack);
        abilities.push_back(AbilityType::Poison);
        abilities.push_back(AbilityType::Stun);
    }

}

void Enemy::useAbility(Character& target) {
    int randomIndex = rand() % abilities.size();
    AbilityType selectedAbility = abilities[randomIndex];
    cout << getName() << " uses ability on " << target.getName() << endl;
    cout << "Enemy attacking target at address: " << &target << endl;
    switch (selectedAbility)
    {
    case AbilityType::NormalAttack:
    cout << "Dealing damage: " << baseDamage << endl;    
    target.takeDamage(baseDamage);
        break;
    case AbilityType::Stun:
        target.applyStun();
        target.takeDamage(baseDamage - 10);
        cout << "Target is stunned!" << endl;
        break;
    case AbilityType::Poison:
        target.applyPoison(5, 3);
        cout << "Target is poisoned for 5 damage over 3 turns!" << endl;
        break;
    case AbilityType::Defend:
        setDefenseState(true); 
        cout << "Enemy is defending!" << endl;
        break;
    }
}

void Enemy::takePhysicalDamage(int amount) {
    if (resistance == ResistanceType::Physical)
        amount /= 2;
    Character::takeDamage(amount);
    cout << getName() << " takes " << amount << " damage after resistance! Health: "
              << health << "/" << maxHealth << std::endl;
}
void Enemy::takeMagicalDamage(int amount) {
    if (resistance == ResistanceType::Magical)
        amount /= 2;
    Character::takeDamage(amount);
    cout << getName() << " takes " << amount << " damage after resistance! Health: "
              << health << "/" << maxHealth << std::endl;
}


void Enemy::attack(Character& target) {
    useAbility(target);
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

std::string Enemy::getRewardSkill() const {
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

string Enemy::rollRewardSkill() const {
    static const vector<SkillType> pool = {
        SkillType::Slash,
        SkillType::IceSlash,
        SkillType::PoisonDart,
        SkillType::Heal
    };
    // 50% drop chance
    if (rand() % 2 == 0) {
        SkillType picked = pool[rand() % pool.size()];
        // convert to string via the global helper
        switch (picked) {
            case SkillType::Slash:      return "Slash";
            case SkillType::IceSlash:   return "Ice Slash";
            case SkillType::PoisonDart: return "Poison Dart";
            case SkillType::Heal:       return "Heal";
            default:                    return string();
        }
    }
    return string();
}

