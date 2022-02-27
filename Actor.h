#ifndef ACTOR_H_
#define ACTOR_H_


#include "GraphObject.h"
#include <iostream>
using namespace std;
class StudentWorld;
// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp
class Actor: public GraphObject {
public:
    Actor(StudentWorld* studentWorld, int x, int y, int imageID, int startDirection, int depth, int size): GraphObject(imageID, x, y , startDirection, depth, size), m_world(studentWorld),
    isAlive(true) {
        
    }
    virtual void doSomething() = 0;
    virtual void bonk() {
        cerr << "object bonked" << endl;
    }
    StudentWorld* getWorld() {
        return m_world;
    }
    bool getIsAlive() {
        return isAlive;
    }
    void setIsAlive(bool alive) {
        isAlive = alive;
    }
    virtual bool block() {
        return false;
    }
    virtual bool isDamageable() {
        return false;
    }
    virtual void damage() {
        cerr << "object damaged" << endl;
        return;
    }
private:
    StudentWorld* m_world;
    bool isAlive;
};


class Peach: public Actor {
public:
    Peach(StudentWorld* studentWorld, int x, int y): Actor(studentWorld, x, y, IID_PEACH, 0, 0, 1) {
        m_temporaryInvincibility = false;
        m_temporaryInvincibilityFramesLeft = 0;
        m_starPower = false;
        m_invincibilityFramesLeft = 0;
        m_shootPower = false;
        m_jumpPower = false;
        m_remainingJumpDistance = 0;
        m_recharging = false;
        m_rechargeFramesLeft = 0;
        m_hp = 1;
    }
    void doSomething();
    void bonk();
//    void doSomething() {
//        std::cerr << "peach is doing nothing" << std::endl;
//        return;
//    }
    bool getStarPower() {
        return m_starPower;
    }
    bool getShootPower() {
        return m_shootPower;
    }
    bool getJumpPower() {
        return m_jumpPower;
    }
    bool isDamageable() {
        return true;
    }
    void setShootPower() {
        m_shootPower = true;
    }
    void setJumpPower() {
        m_jumpPower = true;
    }
    void setStarPower(int frames) {
        m_starPower = true;
        m_invincibilityFramesLeft = frames;
    }
    void setHP(int hp) {
        m_hp = hp;
    }
    void damage() {
        bonk();
    }
private:
    bool m_temporaryInvincibility;
    int m_temporaryInvincibilityFramesLeft;
    int m_invincibilityFramesLeft;
    bool m_starPower;
    bool m_shootPower;
    bool m_jumpPower;
    int m_remainingJumpDistance;
    bool m_recharging;
    int m_rechargeFramesLeft;
    int m_hp;
};

class Collider : public Actor {
public:
    Collider(StudentWorld* studentWorld, int x, int y, int IID) : Actor(studentWorld, x, y, IID, 0, 2, 1) {
        
    }
    void doSomething();
    bool block() {
        return true;
    }
};
class Block: public Collider {
public:
    Block(StudentWorld* studentWorld, int x, int y, int goodie): Collider(studentWorld, x, y, IID_BLOCK) {
        m_hasBeenBonked = false;
        if(goodie == 0) {
            m_hasBeenBonked = true;
        }
        m_goodie = goodie;
    }
    void bonk();
private:
    bool m_hasBeenBonked;
    int m_goodie;
};

class Pipe: public Collider {
public:
    Pipe(StudentWorld* studentWorld, int x, int y): Collider(studentWorld, x, y, IID_PIPE) {
        
    }
};

class Flag: public Actor {
public:
    Flag(StudentWorld* studentWorld, int x, int y): Actor(studentWorld, x, y, IID_FLAG, 0, 1, 1) {
    }
    void bonk() {
        cerr << "flag bonked" << endl;
    }
    void doSomething();
};

class Mario: public Actor {
public:
    Mario(StudentWorld* studentWorld, int x, int y): Actor(studentWorld, x, y, IID_MARIO, 0, 1, 1) {
    }
    void bonk() {
        cerr << "mario bonked" << endl;
    }
    void doSomething();
};
class Enemy : public Actor {
public:
    Enemy(StudentWorld* studentWorld, int x, int y, int direction, int IID) : Actor(studentWorld, x, y, IID, direction, 0, 1) {
        
    }
};
class Goomba: public Enemy {
public:
    Goomba(StudentWorld* studentWorld, int x, int y) : Enemy(studentWorld, x, y, 180*randInt(0, 1), IID_GOOMBA) {
        
    }
    void bonk();
    void doSomething();
    void damage() {
        setIsAlive(false);
    }
};
class Koopa: public Enemy {
public:
    Koopa(StudentWorld* studentWorld, int x, int y) : Enemy(studentWorld, x, y, 180*randInt(0, 1), IID_KOOPA) {
        
    }
    void bonk();
    void doSomething() {
        if(!getIsAlive()) {
            return;
        }
    }
    void damage() {
        setIsAlive(false);
    }
};
class Projectile : public Actor {
public:
    Projectile(StudentWorld* studentWorld, int x, int y, int direction, int IID) : Actor(studentWorld, x, y, IID, direction, 1, 1) {
        
    }
};
class PeachFireball : public Projectile {
public:
    PeachFireball(StudentWorld* studentWorld, int x, int y, int direction) : Projectile(studentWorld, x, y, direction, IID_PEACH_FIRE) {
        cerr << "new peach fireball made!" << endl;
    }
    void doSomething();
};
class PiranhaFireball : public Projectile {
public:
    PiranhaFireball(StudentWorld* studentWorld, int x, int y, int direction) : Projectile(studentWorld, x, y, direction, IID_PIRANHA_FIRE) {
        cerr << "new piranha fireball made!" << endl;
    }
    void bonk() {
        
    }
    void doSomething() {
        
    }
};
class Shell : public Projectile {
public:
    Shell(StudentWorld* studentWorld, int x, int y, int direction) : Projectile(studentWorld, x, y, direction, IID_SHELL) {
        cerr << "new shell made!" << endl;
    }
    void bonk() {
        
    }
    void doSomething() {
        
    }
};

class Goodie : public Actor {
public:
    Goodie(StudentWorld* studentWorld, int x, int y, int direction, int IID) : Actor(studentWorld, x, y, IID, direction, 1, 1) {
        
    }
    void doSomething();
    virtual void customBuffs() = 0;
};
class Flower : public Goodie {
public:
    Flower(StudentWorld* studentWorld, int x, int y, int direction) : Goodie(studentWorld, x, y, direction, IID_FLOWER) {
        cerr << "new flower made!" << endl;
    }
//    void bonk() {
//        cerr << "flower bonked" << endl;
//    }
    void customBuffs();
};
class Mushroom : public Goodie {
public:
    Mushroom(StudentWorld* studentWorld, int x, int y, int direction) : Goodie(studentWorld, x, y, direction, IID_MUSHROOM) {
        cerr << "new mushroom made!" << endl;
    }
    void bonk() {
        cerr << "mushroom bonked" << endl;
    }
    void customBuffs();
};
class Star : public Goodie {
public:
    Star(StudentWorld* studentWorld, int x, int y, int direction) : Goodie(studentWorld, x, y, direction, IID_STAR) {
        cerr << "new star made!" << endl;
    }
    void bonk() {
        cerr << "star bonked" << endl;
    }
    void customBuffs();
};
#endif // ACTOR_H_


