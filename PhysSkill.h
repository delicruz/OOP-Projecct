#ifndef PHYSSKILL_H
#define PHYSSKILL_H

#include "Skill.h"
#include "Character.h"
#include <string.h>

class PhysSkill: public Skill
{
    protected:

    int critMultiplier;
    int critChance; //Out of 100

    public:
    PhysSkill(string name, int dmg, float min, float max, int critMultiplier, int critChance);

    void Attack(Character& target) override;
    int getRandCritDamage() const;
    bool checkChance() const;


    int getCritMultiplier() const;
    int getCritChance() const;
};

#endif