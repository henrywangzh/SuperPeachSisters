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
  bool coordIsBlocked(int x, int y);

private:
    std::list<Actor*> m_actors;
    Peach* m_peach;
};

#endif // STUDENTWORLD_H_
