#include "MagicSkill.h"

MagicSkill::MagicSkill(string name, int dmg, float min, float max, int manaUseage):
    Skill(name,dmg,min,max), manaUseage(manaUseage){}