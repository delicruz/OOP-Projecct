#include "Player.h"
#include "Skill.h"
#include <cstdlib>  // for rand()
#include <algorithm>
#include <iostream>
#include <limits>
using namespace std;


Player::Player(const string& name, int hp)
    : Character(name, hp), level(1), exp(0), baseDamage(18), equippedSkill(SkillType::None), inBattle(false) {
    // Initialize equipped skills empty (no extra skills at start)
}

void Player::heal(int amount) {
    health += amount;
    if (health > maxHealth)
        health = maxHealth;
}

void Player::physicalAttack(Character& target) {
    int damage = baseDamage;
    target.takeDamage(damage);
}

void Player::magicalAttack(Character& target) {
    int damage = baseDamage;
    target.takeDamage(damage);
}

void Player::defend() {
    setDefenseState(true);
}

void Player::attack(Character& target) {
    int choice = 1;
    switch (choice) {
        case 1:
            physicalAttack(target);
            break;
        case 2:
            magicalAttack(target);
            break;
        case 3:
            defend();
            break;
        case 4:
            if (equippedSkill != SkillType::None)
                useAbility(target);
            break;
    }   
}


void Player::useAbility(Character& target) {
    if (equippedSkill == SkillType::None) {
        return;
    }
    const SkillData& data = getSkillData(equippedSkill);

    if (equippedSkill == SkillType::Heal) {
        this->heal(25);
            cout << getName() << " heals for 25 HP! Health: " 
            << getHealth() << "/" << getMaxHealth() << endl;
        return;
    }

    int damage = data.baseDamage;
    if (equippedSkill == SkillType::Slash) {
        int roll = rand() % 100;
        if (roll < data.critChancePercent) {
            damage *= 3;
        }
        target.takeDamage(damage);
    }

    if (!data.statusEffect.empty()) {
        if (data.statusEffect == "Poison") {
            target.applyPoison(damage, data.effectDuration);
        }
        else if (data.statusEffect == "Stun") {
            target.applyStun();
            target.takeDamage(damage - 10);
        }
    }
}

void Player::addToInventory(SkillType newSkill) {
    if (newSkill == SkillType::None) return;

    if(std::find(skillInventory.begin(), skillInventory.end(), newSkill) == skillInventory.end()) {
        skillInventory.push_back(newSkill);
    }
}


void Player::equipSkill(SkillType skill) {
    auto it = find(skillInventory.begin(), skillInventory.end(), skill);
    if (it != skillInventory.end()) {
        equippedSkill = skill;
        const SkillData& data = getSkillData(skill);
        cout << "Equipped skill: " << data.name << endl;
    } else {
        cout << "Cannot equip skill: not in inventory" << endl;
    }
}

SkillType Player::getEquippedSkill() const {
    return equippedSkill;
}

const vector<SkillType>& Player::getSkillInventory() const {
    return skillInventory;
}

void Player::gainExp(int amount) {
    exp += amount;
}

void Player::checkLevelUp() {
    while (exp >= expToLevel) {
        exp -= expToLevel;
        level_Up();
    }
}

void Player::level_Up() {
    level++;
    maxHealth += 35;
}

void Player::resetProgress() {
    health = maxHealth;
    exp = 0;
    level = 1;
    skillInventory.clear();
    equippedSkill = SkillType::None;
    cout << "Progress reset!" << endl;
}

int Player::getLevel() const {
    return level;
}

int Player::getExp() const {
    return exp;
}

int Player::getMaxHealth() const {
    return maxHealth;
}

int Player::getBaseDamage() const {
    return baseDamage;
}

void Player::setInBattle(bool battleState) {
    inBattle = battleState;
}

void Player::saveToStream(std::ostream& out) const {
    out << name << "\n";
    out << getHealth() << " " << getMaxHealth() << " " << exp << " " << level << "\n";
    out << skillInventory.size() << "\n";
    for (SkillType skill : skillInventory)
        out << static_cast<int>(skill) << " ";
    out << "\n";
    out << static_cast<int>(equippedSkill) << "\n";
}

void Player::loadFromStream(istream& in) {
    int hp, maxHp;
    getline(in, name);
    in >> hp >> maxHp >> exp >> level;
    setHealth(hp);
    this->maxHealth = maxHp;
    size_t invCount;
    in >> invCount;
    skillInventory.clear();
    for (size_t i = 0; i < invCount; ++i) {
        int skillInt;
        in >> skillInt;
        skillInventory.push_back(static_cast<SkillType>(skillInt));
    }
    int eqSkill;
    in >> eqSkill;
    equippedSkill = static_cast<SkillType>(eqSkill);
    // eat leftover newline
    in.ignore(numeric_limits<streamsize>::max(), '\n');
}