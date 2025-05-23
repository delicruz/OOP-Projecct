#include "DOTSkill.h"

DOTSkill::DOTSkill(string name, int dmg, float min, float max, int dotDamage, int dotTime) 
    : Skill(name,dmg,min,max), dotDamage(dotDamage), dotTime(dotTime){}

void DOTSkill::Attack(Character& target)
{
    Skill::Attack(target);

    target.applyDOT(dotDamage, dotTime);

}
