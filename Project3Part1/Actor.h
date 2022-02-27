#ifndef ACTOR_H_
#define ACTOR_H_


#include "GraphObject.h"

class StudentWorld;
// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp
class Actor: public GraphObject {
public:
    Actor(StudentWorld* studentWorld, int x, int y, int imageID, int startDirection, int depth, int size): GraphObject(imageID, x* SPRITE_WIDTH, y * SPRITE_HEIGHT, startDirection, depth, size), m_world(studentWorld),
    isAlive(true) {
        
    }
    virtual void doSomething() = 0;
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
private:
    StudentWorld* m_world;
    bool isAlive;
};

class MovingActor: public Actor {
public:
    MovingActor(StudentWorld* studentWorld, int x, int y, int imageID, int startDirection, int depth, int size) :
        Actor(studentWorld, x, y, imageID, startDirection, depth, size) {
            
        }
    virtual void die() {
        setIsAlive(false);
        return;
        
    }

};


class Peach: public MovingActor {
public:
    Peach(StudentWorld* studentWorld, int x, int y): MovingActor(studentWorld, x, y, IID_PEACH, 0, 0, 1) {
        
    }
    void doSomething();
//    void doSomething() {
//        std::cerr << "peach is doing nothing" << std::endl;
//        return;
//    }
};

class Block: public MovingActor {
public:
    Block(StudentWorld* studentWorld, int x, int y): MovingActor(studentWorld, x, y, IID_BLOCK, 0, 2, 1) {
        
    }
    void doSomething() {
        //std::cerr << "block is doing nothing" << std::endl;
        return;
    }
    bool block() {
        return true;
    }
private:
    bool isBonkable;
    int goodie;
};
#endif // ACTOR_H_
