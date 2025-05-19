#include "HealSkill.h"

HealSkill::HealSkill(string name, int dmg, float min, float max, int healAmount)
    : Skill(name,dmg,min,max), healAmount(healAmount){}

void HealSkill::Attack(Character& target)
{
    //I simply implemented healthing by making the player take negative damage
    //Ive decided against making the amount healed random for simplicities sake
    target.takeDamage(-healAmount);
}

int HealSkill::getHealAmount() const
{
    return this->healAmount;
}