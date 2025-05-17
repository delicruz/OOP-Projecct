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
    int health;
    string skills[4]; // Equipped skills (max 4)
    vector<string> skillInventory;  // Inventory of acquired but unequipped skills

    public:
    Player(string name, int hp);

    void gainMana(int amount);
    void level_Up();
    void resetProgress();   // Reset the player stats when defeated
    void physicalAttack(Character& target);
    void magicalAttack(Character& target);

    void addToInventory(const string& skill);   // Add a new skill to the inventory
    void manageEquippedSkills();

    int getLevel() const;
    int getMana() const;
    int getMaxHealth() const;
    

};

#endif