#ifndef PLAYER_H
#define PLAYER_H
using namespace std;
#include "Character.h"
#include "Skill.h"
#include <vector>
#include <string>

class Player : public Character {
    private:
    int level;
    //Added variable js to track the player's current mana
    int mana;
    int currentMana;
    //I changed the inventory to be a list of the Skill class, js to simplify stuff
    vector<Skill> skills;
    //I also added a max skills integer just to allow us to change stuff later if we wish
    int maxSkills;

    public:
    Player(string name, int hp);

    void gainMana(int amount);
    void level_Up();
    void resetProgress();
    void physicalAttack(Character& target);
    void magicalAttack(Character& target);

    //Changed the function to use the Skills variable, js to fit in with the new inventory system
    void addToInventory(const Skill& skill);
    void manageEquippedSkills();

    int getLevel() const;
    int getMana() const;
    int getMaxHealth() const;
    

};

#endif