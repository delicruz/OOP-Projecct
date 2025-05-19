#ifndef SKILL_H
#define SKILL_H

#include <string>
#include <random>
#include <iostream>
#include "Character.h"
using namespace std;

class Skill
{
    protected:
    string skillName;
    int skillDamage;
    float damageRandomMultiplier;

    public:
    Skill(string name, int dmg, float multiplier);

    void Attack(Character toAttack);

    string getName() const;
    int getDamage() const;
};

#endif