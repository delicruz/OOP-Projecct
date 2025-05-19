#ifndef POISONSKILL_H
#define POISONSKILL_H

#include <Skill.h>

class PoisonSkill: public Skill
{
    public:
    int turnsPoisoned;
    int poisonDamage;
};

#endif