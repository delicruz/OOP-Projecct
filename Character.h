#ifndef CHARACTER_H
#define CHARACTER_H
using namespace std;
#include <string>

class Character {
    protected:
    string name;
    bool isDefending;
    bool isPoisoned;
    int poisonTurns;
    int poisonDamages;
    bool isStunned;

    public:
    Character (string name, int hp);
    virtual ~Character() = default;

    virtual void attack(Character& target) = 0;
    virtual void takeDamage(int amount);
    bool isAlive() const;

    void setDefenseState(bool state);
    bool isDefendingFunc() const;

    void applyPoison(int damage, int turns);
    void applyStun();
    void ProcessStatusEffect();

    int getHealth() const;
    string getName() const;
 
};

#endif 