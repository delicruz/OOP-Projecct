#ifndef ENEMY_H
#define ENEMY_H
#include "Character.h"
#include "Enums.h"
#include <string>

class Enemy : public Character {
    private:
    Difficulty difficulty;
    AbilityType ability;
    ResistanceType resistance;
    string rewardSkill;
    int rewardExp;
    void initializeByDifficulty();
    void assignRewardSkill();
    protected:
    int baseDamage;
    int maxHealth;

    public:
    Enemy(string name, Difficulty diff, AbilityType abil, ResistanceType res, string reward, int exp);   // Helper function to set stats based on difficulty

    void attack(Character& target) override; // Override attack method for enemy
    void defend();

    Difficulty getDifficulty() const;
    AbilityType getAbility() const;
    ResistanceType getResistance() const;
    string getRewardSkill() const;

    int getBaseDamage() const;
    int getMaxHealth() const;
    int getRewardExp() const;
};

#endif
