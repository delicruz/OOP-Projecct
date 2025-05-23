#include "Skill.h"
using namespace std;

Skill::Skill(string name, int dmg, float min, float max)
{
    this->skillName = name;
    this->skillDamage = dmg;
    this->minDmgMulti = min;
    this->maxDmgMulti = max;
}

    void Skill::Attack(Character& target)
    {
        target.takeDamage(getRandDamage());
    }

    int Skill::getRandDamage() const
    {
        static std::random_device rd;         // Only used once to seed
        static std::mt19937 gen(rd());        // Random-number engine (seeded once)
        std::uniform_int_distribution<> dist(static_cast<float>(getBaseDamage()) * minDmgMulti, static_cast<float>(getBaseDamage()) * maxDmgMulti);  // Inclusive range
        return dist(gen);
    }

    string Skill::getName() const
    {
        return this->skillName;
    }

    int Skill::getBaseDamage() const
    {
        return this->skillDamage;
    }