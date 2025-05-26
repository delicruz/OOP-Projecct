#ifndef ENEMY_H
#define ENEMY_H
#include "Character.h"
#include "Enums.h"
#include "Skill.h"
#include <string>
#include <vector>

class Enemy : public Character {
    private:
    Difficulty difficulty;
    AbilityType ability;
    ResistanceType resistance;
    int rewardExp;
    void initializeByDifficulty();
    void assignAbilities();
    protected:
    int baseDamage;
    int maxHealth;
    std::vector<AbilityType> abilities;
    std::string rewardSkill;
    public:
   Enemy(string name, Difficulty diff, AbilityType abil, ResistanceType res, int exp);   // Helper function to set stats based on difficulty

    void attack(Character& target) override; // Override attack method for enemy
    void defend();
    void useAbility(Character& target);

    Difficulty getDifficulty() const;
    AbilityType getAbility() const;
    ResistanceType getResistance() const;
    
    int calculateDamageWithResistance(int baseDamage, bool isMagical) const;
    int getBaseDamage() const;
    int getMaxHealth() const;
    int getRewardExp() const;
    std::string getRewardSkill() const;

    std::string rollRewardSkill() const;

    void setMaxHealth(int maxHp)  { maxHealth = maxHp; }
    void setRewardSkill(const std::string& skill) { rewardSkill = skill; }



};

#endif
