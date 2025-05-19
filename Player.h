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
    //I changed the inventory to be a list of the integers, because the skills are stored in seperate txt files and the int represents which line said skill is on
    vector<int> skills;
    //I also added a max skills integer just to allow us to change stuff later if we wish
    int maxSkills;

    public:
    Player(string name, int hp);

    void gainMana(int amount);
    void level_Up();
    void resetProgress();
    void physicalAttack(Character& target);
    void magicalAttack(Character& target);

    //Changed the function to use integers, js to fit in with the new inventory system
    //Also the skillType variable is an ingeter used to identify what type of skill to give the player, as each skill type is 
    //stored in a document of that particular skill type
    void addToInventory(int skillType, const int& skill);
    void manageEquippedSkills();

    int getLevel() const;
    int getMana() const;
    int getMaxHealth() const;
    

};

#endif