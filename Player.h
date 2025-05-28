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
    int exp;
    int baseDamage;
    static const int expToLevel = 35;  // EXP threshold
    SkillType equippedSkill;
    vector<SkillType> skillInventory;

    bool inBattle = false;

    public:
    Player(const string& name, int hp);

    void gainExp(int amount);
    void checkLevelUp();
    void level_Up();
    void resetProgress();
    
    void heal(int amount);
    void physicalAttack(Character& target);
    void magicalAttack(Character& target);
    void attack(Character& target) override;
    void defend();



    // Inventory management
    void addToInventory(SkillType skill);
    void equipSkill(SkillType skill);
    SkillType getEquippedSkill() const;
    const std::vector<SkillType>& getSkillInventory() const;
    void useAbility(Character& target) override;


    int getLevel() const;
    int getExp() const;
    int getMaxHealth() const;
    int getBaseDamage() const;
    
    void setInBattle(bool battleState);
 
    // Save/Load functionality
    void saveToStream(ostream& out) const;
    void loadFromStream(istream& in);

    
};

#endif