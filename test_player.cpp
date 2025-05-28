#include <cassert>
#include <iostream>
#include <sstream>
#include "Player.h"
#include "Skill.h"

void testConstruction() {
    Player p("Hero", 100);
    assert(p.getName() == "Hero");
    assert(p.getHealth() == 100);
    assert(p.getMaxHealth() == 100);
    assert(p.getLevel() == 1);
    assert(p.getBaseDamage() == 18);
    std::cout << "testConstruction passed.\n";
}

void testInventory() {
    Player p("Hero", 100);
    p.addToInventory(SkillType::Slash);
    assert(p.getSkillInventory().size() == 1);
    std::cout << "testInventory passed.\n";
}

void testEquipSkill() {
    Player p("Hero", 100);
    p.addToInventory(SkillType::Slash);
    p.equipSkill(SkillType::Slash);
    assert(p.getEquippedSkill() == SkillType::Slash);
    std::cout << "testEquipSkill passed.\n";
}

void testHeal() {
    Player p("Hero", 100);
    p.setHealth(50);
    p.heal(30);
    assert(p.getHealth() == 80);
    p.heal(50); // should not go over max
    assert(p.getHealth() == 100);
    std::cout << "testHeal passed.\n";
}

void testExpLevel() {
    Player p("Hero", 100);
    p.gainExp(35);
    p.checkLevelUp();
    assert(p.getLevel() == 2); // Should level up
    std::cout << "testExpLevel passed.\n";
}

void testSaveLoad() {
    Player p("Hero", 100);
    p.addToInventory(SkillType::Slash);
    p.equipSkill(SkillType::Slash);
    p.setHealth(55);

    std::stringstream ss;
    p.saveToStream(ss);

    Player loaded("", 0);
    loaded.loadFromStream(ss);

    assert(loaded.getName() == "Hero");
    assert(loaded.getHealth() == 55);
    assert(loaded.getSkillInventory().size() == 1);
    assert(loaded.getEquippedSkill() == SkillType::Slash);

    std::cout << "testSaveLoad passed.\n";
}

int main() {
    testConstruction();
    testInventory();
    testEquipSkill();
    testHeal();
    testExpLevel();
    testSaveLoad();
    std::cout << "All Player unit tests passed!\n";
    return 0;
}
