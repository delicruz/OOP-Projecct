#ifndef DOTSKILL_H
#define DOTSKILL_H

#include "Skill.h"
#include "Character.h"
#include <string.h>

class DOTSkill: public Skill
{
    protected:

    int dotDamage;
    int dotTime;

    public:
    DOTSkill(string name, int dmg, float min, float max, int dotDamage, int dotTime);

    void Attack(Character& target) override;

    int getBaseDOTDamage() const;
    int getDOTTime() const;
};

#endif