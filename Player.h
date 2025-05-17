#ifndef PLAYER_H
#define PLAYER_H
using namespace std;
#include "Character.h"
#include <vector>
#include <string>

class Player : public Character {
    private:
    int level;
    int mana;
    int maxHealth;
    string skills[4];
    vector<string> skillInventory;

    public:
    Player(string name, int hp);

    void gainMana(int amount);
    void level_Up();
    void resetProgress();
    void physicalAttack(Character& target);
    void magicalAttack(Character& target);

    void addToInventory(const string& skill);
    void manageEquippedSkills();

    int getLevel() const;
    int getMana() const;
    int getMaxHealth() const;
    

};

#endif