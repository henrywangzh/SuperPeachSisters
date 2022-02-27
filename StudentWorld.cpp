#include "StudentWorld.h"
#include "GameConstants.h"
#include "Level.h" // required to use our provided class
#include "Actor.h"
#include <string>
#include <list>
#include <sstream>
using namespace std;


GameWorld* createStudentWorld(string assetPath)
{
    return new StudentWorld(assetPath);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h, and Actor.cpp

StudentWorld::StudentWorld(string assetPath)
: GameWorld(assetPath)
{
    //initialize values
    m_levelFinished = false;
    m_gameWon = false;
    m_actors.clear();
    m_peach = nullptr;
}

int StudentWorld::init()
{
    //level file name calculations
    m_levelFinished = false;
    Level lev(assetPath());
    int level = getLevel();
    ostringstream levelName;
    levelName.fill('0');
    levelName << setw(2) << level;
    string level_file = "level" + levelName.str() + ".txt";
    
    //check for errors
    Level::LoadResult result = lev.loadLevel(level_file);
    if (result == Level::load_fail_file_not_found) {
        return GWSTATUS_LEVEL_ERROR;
    }
    else if (result == Level::load_fail_bad_format) {
        return GWSTATUS_LEVEL_ERROR;
    }
    //load level
    else if (result == Level::load_success) {
        Level::GridEntry ge;
        for(int x = 0; x < 32; ++x) {
            for(int y = 0; y < 32; ++y) {
                ge = lev.getContentsOf(x, y);
                //account for sprite width and height
                int xCoord = x * SPRITE_WIDTH;
                int yCoord = y * SPRITE_HEIGHT;
                
                //load in each individual actor
                switch (ge) {
                    case Level::empty:
                        break;
                    case Level::koopa:
                        m_actors.push_back(new Koopa(this, xCoord, yCoord));
                        break;
                    case Level::goomba:
                        m_actors.push_back(new Goomba(this, xCoord, yCoord));
                        break;
                    case Level::peach:
                        m_peach = new Peach(this, xCoord, yCoord);
                        break;
                    case Level::flag:
                        m_actors.push_back(new Flag(this, xCoord, yCoord));
                        break;
                    case Level::block:
                        m_actors.push_back(new Block(this, xCoord, yCoord, 0));
                        break;
                    case Level::mushroom_goodie_block:
                        m_actors.push_back(new Block(this, xCoord, yCoord, 1));
                        break;
                    case Level::flower_goodie_block:
                        m_actors.push_back(new Block(this, xCoord, yCoord, 2));
                        break;
                    case Level::star_goodie_block:
                        m_actors.push_back(new Block(this, xCoord, yCoord, 3));
                        break;
                    case Level::piranha:
                        m_actors.push_back(new Piranha(this, xCoord, yCoord));
                        break;
                    case Level::pipe:
                        m_actors.push_back(new Pipe(this, xCoord, yCoord));
                        break;
                    case Level::mario:
                        m_actors.push_back(new Mario(this, xCoord, yCoord));
                        break;
                }
            }
        }
    }
    return GWSTATUS_CONTINUE_GAME;
}

//loop through all actors, move them, check for various conditions, delete dead actors, continue game
int StudentWorld::move() {
    m_peach->doSomething();
    for (auto const& actor : m_actors) {
        if(actor->getIsAlive()) {
            actor->doSomething();
            if(!m_peach->getIsAlive()) {
                playSound(SOUND_PLAYER_DIE);
                decLives();
                return GWSTATUS_PLAYER_DIED;
            }
            if(m_levelFinished) {
                playSound(SOUND_FINISHED_LEVEL);
                return GWSTATUS_FINISHED_LEVEL;
            }
            if(m_gameWon) {
                playSound(SOUND_GAME_OVER);
                return GWSTATUS_PLAYER_WON;
            }
        }
    }
    auto actor = m_actors.begin();
    while(actor != m_actors.end()) {
        if(!((*actor)->getIsAlive())) {
            delete *actor;
            m_actors.erase(actor);
        }
        actor++;
    }
    //display header text
    string text = "Lives: " + to_string(getLives()) + "  Level: " + to_string(getLevel()) + "  Points: " + to_string(getScore());
    if(m_peach->getStarPower()) {
        text += " StarPower!";
    }
    if(m_peach->getShootPower()) {
        text += " ShootPower!";
    }
    if(m_peach->getJumpPower()) {
        text += " JumpPower!";
    }
    setGameStatText(text);
    
    
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

//responsible for checking if a coordinate is blocked and for bonking actors and for damaging actors
//caller parameter ensures an actor skips itself while looping through the list of all actors
bool StudentWorld::coordIsBlocked(int x, int y, bool canBonk, bool canDamage, Actor* caller) {
    bool isBlocked = false;
    for (auto const& actor : m_actors) {
        if(actor == caller) {
            continue;
        }
        //check to see if actor is still alive since it may have died earlier in the loop
        if(!actor->getIsAlive()) {
            continue;
        }
        //main hitbox detection calculation
        if(x + SPRITE_WIDTH -1 >= actor->getX() && x <= actor->getX() + SPRITE_WIDTH - 1 && y + SPRITE_HEIGHT - 1 >= actor->getY() && y <= actor->getY() + SPRITE_HEIGHT -1 ) {
            if(actor->block()) {
                isBlocked = true;
            }
            //depending on input parameters, bonk/damage accordingly
            if(canBonk) {
                actor->bonk();
            }
            if(canDamage) {
                if(actor->isDamageable()) {
                    actor->damage();
                    //need in order to achieve proper effect
                    isBlocked = true;
                    return isBlocked;
                }
            }
        }
    }
    return isBlocked;
}

void StudentWorld::addActor(Actor* newActor) {
    m_actors.push_back(newActor);
}

//check for overlap with peach and also bonk peach if overlapping
bool StudentWorld::overlappingPeach(int x, int y, bool canBonk) {
    if(x + SPRITE_WIDTH -1 >= m_peach->getX() && x <= m_peach->getX() + SPRITE_WIDTH - 1 && y + SPRITE_HEIGHT - 1 >= m_peach->getY() && y <= m_peach->getY() + SPRITE_HEIGHT -1 ) {
        if(canBonk) {
            m_peach->bonk();
        }
        return true;
    }
    return false;
}
//setters and getters
void StudentWorld::setLevelFinished() {
    m_levelFinished = true;
}
void StudentWorld::setGameWon() {
    m_gameWon = true;
}
void StudentWorld::giveFlowerBuff() {
    m_peach->setShootPower();
}
void StudentWorld::giveMushroomBuff() {
    m_peach->setJumpPower();
}
void StudentWorld::giveInvincibility(int frames) {
    m_peach->setStarPower(frames);
}
void StudentWorld::setPeachHP(int hp) {
    m_peach->setHP(hp);
}
bool StudentWorld::getPeachIsInvincible() const {
    return m_peach->getStarPower();
}
int StudentWorld::getPeachX() {
    return m_peach->getX();
}
int StudentWorld::getPeachY() {
    return m_peach->getY();
}
