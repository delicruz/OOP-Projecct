#ifndef HEALSKILL_H
#define HEALSKILL_H

#include "Skill.h"
#include "Character.h"
#include <string.h>

class HealSkill: public Skill
{
    protected:

    int healAmount;

    public:
    HealSkill(string name, int dmg, float min, float max, int healAmount);
    

    void Attack(Character& target) override;
    int getHealAmount() const;
};

#endif