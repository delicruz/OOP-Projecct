#ifndef STUNSKILL_H
#define STUNSKILL_H

#include "Skill.h"
#include "Character.h"
#include <string.h>

class StunSkill: public Skill
{
    protected:

    int stunTime;

    public:
    StunSkill(string name, int dmg, float min, float max, int sunTime);
    

    void Attack(Character& target) override;
    int getStunTime() const;
};

#endif