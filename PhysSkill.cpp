#include "PhysSkill.h"

PhysSkill::PhysSkill(string name, int dmg, float min, float max, int critMultiplier, int critChance) :
    Skill(name,dmg,min,max), critMultiplier(critMultiplier), critChance(critChance){}

    void PhysSkill::Attack(Character& target)
    {
        //Checks whether the player critted
        if(!checkChance)
        {
            Skill::Attack(target);
        }
        else
        {
            target.takeDamage(getRandCritDamage());
        }
    }

    int PhysSkill::getRandCritDamage() const
    {
        //Set the base damage value to be the damage multiplier by the crit multiplier
        float baseCritDmg = skillDamage * critMultiplier;
        //Generate a random number between the base damage multiplied by the minimum and maximum multipliers
        static std::random_device rd;
        static std::mt19937 gen(rd());
        std::uniform_real_distribution<> dis(baseCritDmg * minDmgMulti, baseCritDmg * maxDmgMulti);

        return dis(gen);
    }

    bool PhysSkill::checkChance() const
    {
        //Generate a random number between 1 and 100
        static std::random_device rd;
        static std::mt19937 gen(rd());
        std::uniform_real_distribution<> dis(0.0, 100.0);

        double roll = dis(gen);
        //Check if it is bellow or above the chance for a critical strike
        return roll < getCritChance();
    }

    int PhysSkill::getCritMultiplier() const
    {
        return this->critMultiplier;
    }

    int PhysSkill::getCritChance() const
    {
        return this->critChance;
    }