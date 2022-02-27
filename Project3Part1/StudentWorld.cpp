#include "StudentWorld.h"
#include "GameConstants.h"
#include "Level.h" // required to use our provided class
#include "Actor.h"
#include <string>
#include <list>
using namespace std;


GameWorld* createStudentWorld(string assetPath)
{
    return new StudentWorld(assetPath);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h, and Actor.cpp

StudentWorld::StudentWorld(string assetPath)
: GameWorld(assetPath)
{
    m_actors.clear();
    m_peach = nullptr;
}

int StudentWorld::init()
{
    Level lev(assetPath());
    int level = getLevel();
    string level_file;
    if(level >= 10) {
        level_file = "level" + to_string(level) + ".txt";
    } else {
        level_file = "level0" + to_string(level) + ".txt";
    }
    
    Level::LoadResult result = lev.loadLevel(level_file);
    if (result == Level::load_fail_file_not_found) {
        cerr << "Could not find level01.txt data file" << endl;
    }
    else if (result == Level::load_fail_bad_format) {
        cerr << "level01.txt is improperly formatted" << endl;
    }
    else if (result == Level::load_success) {
        cerr << "Successfully loaded level" << endl;
        Level::GridEntry ge;
        for(int x = 0; x < 32; ++x) {
            for(int y = 0; y < 32; ++y) {
                ge = lev.getContentsOf(x, y);
                switch (ge) {
                    case Level::empty:
                     //   cout << "Location " << x << "," << y << " is empty" << endl;
                        break;
                    case Level::koopa:
                     //   cout << "Location " << x << "," << y << " starts with a koopa" << endl;
                        break;
                    case Level::goomba:
                     //   cout << "Location " << x << "," << y << " starts with a goomba" << endl;
                        break;
                    case Level::peach:
                     //   cout << "Location " << x << "," << y << " is where Peach starts" << endl;
                        m_peach = new Peach(this, x, y);
                        break;
                    case Level::flag:
                      //  cout << "Location " << x << "," << y << " is where a flag is" << endl;
                        break;
                    case Level::block:
                       // cout << "Location " << x << "," << y << " holds a regular block" << endl;
                        m_actors.push_back(new Block(this, x, y));
                        break;
                    case Level::star_goodie_block:
                     //   cout << "Location " << x << "," << y << " has a star goodie block" << endl;
                        break;
                    case Level::piranha:
                      //  cout << "Location " << x << "," << y << " is where a piranha is" << endl;
                        break;
                    case Level::mushroom_goodie_block:
                      //  cout << "Location " << x << "," << y << " has a mushroom goodie block" << endl;
                        break;
                    case Level::flower_goodie_block:
                       // cout << "Location " << x << "," << y << " has a flower goodie block" << endl;
                        break;
                    case Level::pipe:
                       // cout << "Location " << x << "," << y << " is where a pipe is" << endl;
                        break;
                    case Level::mario:
                      //  cout << "Location " << x << "," << y << " is where Mario is" << endl;
                        break;
                }
            }
        }
    }
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move() {
    m_peach->doSomething();
    for (auto const& actor : m_actors) {
        actor->doSomething();
    }
    // The term "actors" refers to all actors, e.g., Peach, goodies, // enemies, flags, blocks, pipes, fireballs, etc.
    // Give each actor a chance to do something, incl. Peach for each of the actors in the game world
    //{
    //if (that actor is still active/alive)
    //{
    //// tell that actor to do something (e.g. move)
    //that actor -> doSomething();
    //if (Peach died during this tick) { play dying sound
    //} }
    //}
    //return GWSTATUS_FINISHED_LEVEL;
    //}
    //return GWSTATUS_PLAYER_DIED;
    //if (Peach reached Mario) { play game over sound
    //}
    //return GWSTATUS_WON_GAME;
    //if (Peach competed the current level) { play completed level sound
    //// Remove newly-dead actors after each tick remove dead game objects
    //// Update the game status line
    //update display text // update the score/lives/level text at screen top
    //// the player hasn’t completed the current level and hasn’t died, so // continue playing the current level
    //decLives();
    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
    delete m_peach;
    m_peach = nullptr;
    for (auto actor : m_actors) {
        delete actor;
    }
    m_actors.clear();
}

StudentWorld::~StudentWorld() {
    cleanUp();
}

bool StudentWorld::coordIsBlocked(int x, int y) {
    for (auto const& actor : m_actors) {
        if(x + SPRITE_WIDTH - 1 > actor->getX() && x < actor->getX() + SPRITE_WIDTH - 1 && y + SPRITE_HEIGHT - 1 > actor->getY() && y < actor->getY() + SPRITE_HEIGHT - 1) {
            if(actor->block()) {
                return true;
            }
        }
    }
    return false;
}
