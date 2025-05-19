#ifndef SKILL_H
#define SKILL_H

#include <string>
#include <random>
#include <iostream>
#include "Character.h"
#include <cmath>
using namespace std;

class Skill
{
    protected:
    string skillName;
    int skillDamage;
    float minDmgMulti;
    float maxDmgMulti;

    public:
    Skill(string name, int dmg, float min, float max);

    virtual void Attack(Character& target);
    int getRandDamage() const;

    string getName() const;
    int getBaseDamage() const;
};

#endif