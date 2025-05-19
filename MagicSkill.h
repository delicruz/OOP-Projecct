#ifndef MAGICSKILL_H
#define MAGICSKILL_H

#include "Skill.h"
#include "Character.h"
#include <string.h>

class MagicSkill: public Skill
{
    protected:

    int manaUseage;

    public:
    MagicSkill(string name, int dmg, float min, float max, int manaUseage);

    void Attack(Character& target) override;

    int getManaUseage() const;
};

#endif