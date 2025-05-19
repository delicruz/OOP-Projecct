#include "StunSkill.h"

StunSkill::StunSkill(string name, int dmg, float min, float max, int sunTime) 
: Skill(name,dmg,min,max), stunTime(stunTime){}