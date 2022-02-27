#ifndef ACTOR_H_
#define ACTOR_H_


#include "GraphObject.h"
#include <iostream>
using namespace std;
class StudentWorld;

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

//base class for all actors
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

// peach class, contains all member variables, setters, getters, etc.
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

//base class for all objects that can collide
class Collider : public Actor {
public:
    Collider(StudentWorld* studentWorld, int x, int y, int IID) : Actor(studentWorld, x, y, IID, 0, 2, 1) {
        
    }
    void doSomething();
    bool block() {
        return true;
    }
};

//block class, also decides goodie info
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

//pipe class, does nothing aside from block movement
class Pipe: public Collider {
public:
    Pipe(StudentWorld* studentWorld, int x, int y): Collider(studentWorld, x, y, IID_PIPE) {
        
    }
};

// base class for ending objectives (flag and mario)
class Goal : public Actor {
public:
    Goal(StudentWorld* studentWorld, int x, int y, int IID): Actor(studentWorld, x, y, IID, 0 , 1, 1) {
        
    }
    void doSomething();
    virtual void endAction() = 0;
};
//flag class
class Flag: public Goal {
public:
    Flag(StudentWorld* studentWorld, int x, int y): Goal(studentWorld, x, y, IID_FLAG) {
    }
    void endAction();
};

//mario class
class Mario: public Goal {
public:
    Mario(StudentWorld* studentWorld, int x, int y): Goal(studentWorld, x, y, IID_MARIO) {
    }
    void endAction();
};

// base class for all enemies
class Enemy : public Actor {
public:
    Enemy(StudentWorld* studentWorld, int x, int y, int direction, int IID) : Actor(studentWorld, x, y, IID, direction, 0, 1) {
        
    }
    bool isDamageable() {
        return true;
    }
    virtual void doSomething();
    void damage();
    void bonk();
    virtual void onHit() {}
};

//goomba class
class Goomba: public Enemy {
public:
    Goomba(StudentWorld* studentWorld, int x, int y) : Enemy(studentWorld, x, y, 180*randInt(0, 1), IID_GOOMBA) {
        
    }
};

//koopa class
class Koopa: public Enemy {
public:
    Koopa(StudentWorld* studentWorld, int x, int y) : Enemy(studentWorld, x, y, 180*randInt(0, 1), IID_KOOPA) {
        
    }
    void onHit();
};

//piranha class, overrides enemy doSomething()
class Piranha : public Enemy {
public:
    Piranha(StudentWorld* studentWorld, int x, int y) : Enemy(studentWorld, x, y, 180*randInt(0, 1), IID_PIRANHA) {
        m_firingDelay = 0;
    }
    void doSomething();
private:
    int m_firingDelay;
};

//projectile base class
class Projectile : public Actor {
public:
    Projectile(StudentWorld* studentWorld, int x, int y, int direction, int IID) : Actor(studentWorld, x, y, IID, direction, 1, 1) {
        
    }
    void doSomething();
    virtual void collision() = 0;
};

// base class for projectiles that don't damage peach and instead damage enemies
class FriendlyProjectile : public Projectile {
public:
    FriendlyProjectile(StudentWorld* studentWorld, int x, int y, int direction, int IID) : Projectile(studentWorld, x, y, direction, IID) {
        cerr << "new enemy projectile made!" << endl;
    }
    void collision();
};

//class for fireballs made by peach
class PeachFireball : public FriendlyProjectile {
public:
    PeachFireball(StudentWorld* studentWorld, int x, int y, int direction) : FriendlyProjectile(studentWorld, x, y, direction, IID_PEACH_FIRE) {
        cerr << "new peach fireball made!" << endl;
    }
};

//shell class for koopa shells
class Shell : public FriendlyProjectile {
public:
    Shell(StudentWorld* studentWorld, int x, int y, int direction) : FriendlyProjectile(studentWorld, x, y, direction, IID_SHELL) {
        cerr << "new shell made!" << endl;
    }
};

// fireball class for piranha fireballs
class PiranhaFireball : public Projectile {
public:
    PiranhaFireball(StudentWorld* studentWorld, int x, int y, int direction) : Projectile(studentWorld, x, y, direction, IID_PIRANHA_FIRE) {
        cerr << "new piranha fireball made!" << endl;
    }
    void collision();
};

// base class for all goodies/powerups
class Goodie : public Actor {
public:
    Goodie(StudentWorld* studentWorld, int x, int y, int direction, int IID) : Actor(studentWorld, x, y, IID, direction, 1, 1) {
        
    }
    void doSomething();
    virtual void customBuffs() = 0;
};

// fire flower class
class Flower : public Goodie {
public:
    Flower(StudentWorld* studentWorld, int x, int y, int direction) : Goodie(studentWorld, x, y, direction, IID_FLOWER) {
    }
    void customBuffs();
};

// mushroom class
class Mushroom : public Goodie {
public:
    Mushroom(StudentWorld* studentWorld, int x, int y, int direction) : Goodie(studentWorld, x, y, direction, IID_MUSHROOM) {
    }
    void customBuffs();
};

//star class
class Star : public Goodie {
public:
    Star(StudentWorld* studentWorld, int x, int y, int direction) : Goodie(studentWorld, x, y, direction, IID_STAR) {
        cerr << "new star made!" << endl;
    }
    void customBuffs();
};
#endif // ACTOR_H_


