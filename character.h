#ifndef CHARACTER_H
#define CHARACTER_H

#include <string>
#include <memory>

#include "entity.h"

class Character : public Entity {
    protected:
        int hp;
        int atk;
        int def;
        std::string name;
    public:
        // virtual void move(int x, int y);
        Character();
        Character(int hp, int atk, int def, char symbol, std::string name);
        Character(int hp, int atk, int def, char symbol, std::string name, std::string color);
        virtual void move(int dx, int dy);
        int getHP();
        int getAtk();
        int getDef();
        void setHP(int newHp);
        void setAtk(int newAtk);
        void setDef(int newDef);
        virtual int attack(std::shared_ptr<Character> defender);
        std::string getName();
};
#endif
