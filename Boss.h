#ifndef BOSS_H
#define BOSS_H
#include "Enemy.h"
using namespace std;

class Boss : public Enemy {
    private:
    int ultimateDamage;
    int hp;

    public:
    Boss(string name);
    void attack(Character& target) override;
    int getUltimateDamage() const;

};

#endif