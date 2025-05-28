#include <cassert>
#include <iostream>
#include "Enemy.h"

// This will test core behavior with only Enemy, using itself as a target.

void testEnemyConstruction() {
    Enemy easy("E-Easy", Difficulty::Easy, AbilityType::NormalAttack, ResistanceType::Physical, 0);
    assert(easy.getDifficulty() == Difficulty::Easy);
    assert(easy.getBaseDamage() == 8);
    assert(easy.getMaxHealth() == 50);
    assert(easy.getRewardExp() == 10);

    Enemy med("E-Med", Difficulty::Medium, AbilityType::NormalAttack, ResistanceType::Magical, 0);
    assert(med.getBaseDamage() == 13);
    assert(med.getMaxHealth() == 75);
    assert(med.getRewardExp() == 25);

    Enemy hard("E-Hard", Difficulty::Hard, AbilityType::NormalAttack, ResistanceType::None, 0);
    assert(hard.getBaseDamage() == 18);
    assert(hard.getMaxHealth() == 100);
    assert(hard.getRewardExp() == 35);

    std::cout << "testEnemyConstruction passed.\n";
}

void testPhysicalAndMagicalResistance() {
    Enemy physical("E-Phys", Difficulty::Easy, AbilityType::NormalAttack, ResistanceType::Physical, 0);
    int start = physical.getMaxHealth();
    physical.takePhysicalDamage(20); // Should be halved: 10
    assert(physical.getHealth() == start - 10);

    Enemy magical("E-Mag", Difficulty::Easy, AbilityType::NormalAttack, ResistanceType::Magical, 0);
    start = magical.getMaxHealth();
    magical.takeMagicalDamage(20); // Should be halved: 10
    assert(magical.getHealth() == start - 10);

    std::cout << "testPhysicalAndMagicalResistance passed.\n";
}

void testUseAbility() {
    // We'll make an Enemy attack itself, since all it needs is a Character&
    Enemy e("SelfAttacker", Difficulty::Easy, AbilityType::NormalAttack, ResistanceType::None, 0);
    int start = e.getHealth();
    e.useAbility(e); // should do damage, stun, or poison, depending on random ability
    assert(e.getHealth() <= start); // health should not increase
    std::cout << "testUseAbility passed.\n";
}

void testRollRewardSkill() {
    Enemy e("Rewarder", Difficulty::Medium, AbilityType::NormalAttack, ResistanceType::None, 0);
    std::string skill = e.rollRewardSkill();
    // Skill may be empty (50% chance), but if not, it should be a valid skill name
    if (!skill.empty()) {
        assert(skill == "Slash" || skill == "Ice Slash" || skill == "Poison Dart" || skill == "Heal");
    }
    std::cout << "testRollRewardSkill passed.\n";
}

int main() {
    testEnemyConstruction();
    testPhysicalAndMagicalResistance();
    testUseAbility();
    testRollRewardSkill();
    std::cout << "All Enemy unit tests passed!\n";
    return 0;
}
