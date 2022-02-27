#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "Level.h"
#include "Actor.h"
#include <string>
#include <list>

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
public:
  StudentWorld(std::string assetPath);
  virtual int init();
  virtual int move();
  virtual void cleanUp();
    ~StudentWorld();
  bool coordIsBlocked(int x, int y, bool canBonk, bool canDamage,Actor* caller);
    void addActor(Actor* newActor);
    bool overlappingPeach(int x, int y, bool canBonk);
    void setLevelFinished();
    void setGameWon();
    void giveFlowerBuff();
    void giveMushroomBuff();
    void giveInvincibility(int frames);
    void setPeachHP(int hp);
    bool getPeachIsInvincible() const;
    int getPeachX();
    int getPeachY();

private:
    std::list<Actor*> m_actors;
    Peach* m_peach;
    bool m_levelFinished;
    bool m_gameWon;
};

#endif // STUDENTWORLD_H_
