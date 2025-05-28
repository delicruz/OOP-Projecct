#ifndef BOSS_H
#define BOSS_H
#include "Enemy.h"
using namespace std;

class Boss : public Enemy {
    private:
    int hp;

    public:
    Boss(string name);
    void attack(Character& target) override;


};

#endif