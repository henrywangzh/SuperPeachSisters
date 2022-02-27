#include "Actor.h"
#include "StudentWorld.h"

using namespace std;
#include <iostream>
// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp

void Peach::bonk() {
    cerr << "Peach bonked" << endl;
    if(m_starPower || m_temporaryInvincibility) {
        return;
    }
    m_hp--;
    m_temporaryInvincibilityFramesLeft = 10;
    m_temporaryInvincibility = true;
    if(m_shootPower) {
        m_shootPower = false;
    }
    if(m_jumpPower) {
        m_jumpPower = false;
    }
    if(m_hp > 0) {
        getWorld()->playSound(SOUND_PLAYER_HURT);
    }
    if(m_hp <= 0) {
        setIsAlive(false);
    }
}
void Peach::doSomething() {
    //cerr << getX() << " " << getY() << endl;
    if(!getIsAlive()) {
        return;
    }
    if(getStarPower()) {
        if(m_invincibilityFramesLeft == 0) {
            m_starPower = false;
        } else {
            m_invincibilityFramesLeft--;
        }
    }
    if(m_temporaryInvincibility) {
        m_temporaryInvincibilityFramesLeft--;
        if(m_temporaryInvincibilityFramesLeft == 0) {
            m_temporaryInvincibility = false;
        }
    }
    if(m_recharging) {
        m_rechargeFramesLeft--;
        if(m_rechargeFramesLeft == 0) {
            m_recharging = false;
        }
    }
    getWorld()->coordIsBlocked(getX(), getY(), true, false, nullptr);
    int choice;
    if(m_remainingJumpDistance > 0) {
        if(getWorld()->coordIsBlocked(getX(), getY()+4, true, false, nullptr)) {
            m_remainingJumpDistance = 0;
        } else {
            moveTo(getX(), getY()+4);
            m_remainingJumpDistance--;
        }
    } else if(!getWorld()->coordIsBlocked(getX(), getY(), false, false, nullptr) && !getWorld()->coordIsBlocked(getX(), getY()-3, false, false, nullptr)) {
            moveTo(getX(), getY()-4);
    }
    if(getWorld()->getKey(choice)) {
        switch(choice) {
            case KEY_PRESS_LEFT:
                setDirection(180);
                if(!getWorld()->coordIsBlocked(getX()-4, getY(), true, false, nullptr)) {
                    
                    moveTo(getX()-4, getY());
                }
                break;
            case KEY_PRESS_RIGHT:
                setDirection(0);
                if(!getWorld()->coordIsBlocked(getX()+4, getY(), true, false, nullptr)) {
                    
                    moveTo(getX()+4, getY());
                }
                break;
            case KEY_PRESS_UP:
                if(getWorld()->coordIsBlocked(getX(), getY()-2, false, false, nullptr)) {
                    if(!m_jumpPower) {
                        m_remainingJumpDistance = 16;
                    } else {
                        m_remainingJumpDistance = 12;
                    }
                    getWorld()->playSound(SOUND_PLAYER_JUMP);
                }
                break;
            case KEY_PRESS_SPACE:
                if(!m_shootPower || m_recharging) {
                    break;
                }
                m_recharging = true;
                getWorld()->playSound(SOUND_PLAYER_FIRE);
                m_rechargeFramesLeft = 8;
                int x = getX() + (getDirection() == 180 ? -4 : 4);
                int y = getY();
                getWorld()->addActor(new PeachFireball(getWorld(), x, y, getDirection()));
        }
    }
}

void Block::bonk() {
    cerr << "Block bonked" << endl;
    if(m_hasBeenBonked) {
        getWorld()->playSound(SOUND_PLAYER_BONK);
        return;
    }
    getWorld()->playSound(SOUND_POWERUP_APPEARS);
    if(m_goodie == 1) {
        getWorld()->addActor(new Mushroom(getWorld(), getX(), getY()+8, getDirection()));
    }
    else if(m_goodie == 2) {
        getWorld()->addActor(new Flower(getWorld(), getX(), getY()+8, getDirection()));
    }
    else {
        //m_goodie should be equal to 3
        getWorld()->addActor(new Star(getWorld(), getX(), getY()+8, getDirection()));
    }
    m_hasBeenBonked = true;
}

void Collider::doSomething() {
    
}
void Goomba::bonk() {
    cerr << "Goomba bonked" << endl;
}
void Koopa::bonk() {
    cerr << "Koopa bonked" << endl;
}
void Flag::doSomething() {
    if(!getIsAlive()) {
        return;
    }
    if(getWorld()->overlappingPeach(getX(), getY(), false)) {
        getWorld()->increaseScore(1000);
        setIsAlive(false);
        cerr << "level completed!" << endl;
        getWorld()->setLevelFinished();
    }
}
void Mario::doSomething() {
    if(!getIsAlive()) {
        return;
    }
    if(getWorld()->overlappingPeach(getX(), getY(), false)) {
        getWorld()->increaseScore(1000);
        setIsAlive(false);
        cerr << "game completed!" << endl;
        getWorld()->setGameWon();
    }
}

void Goodie::doSomething() {
    if(getWorld()->overlappingPeach(getX(), getY(), false)) {
        customBuffs();
        setIsAlive(false);
        getWorld()->playSound(SOUND_PLAYER_POWERUP);
        return;
    } else {
        if(!getWorld()->coordIsBlocked(getX(), getY()-1, false, false, this)) {
            moveTo(getX(), getY()-2);
        }
        int x = getX() + (getDirection() == 180 ? -2 : 2);
        int y = getY();
        if(getWorld()->coordIsBlocked(x, y, false, false, this)) {
            setDirection((getDirection() == 180 ? 0 : 180));
            return;
        }
        moveTo(x, y);
    }
}
void Flower::customBuffs() {
    getWorld()->increaseScore(50);
    getWorld()->giveFlowerBuff();
}
void Mushroom::customBuffs() {
    getWorld()->increaseScore(75);
    getWorld()->giveMushroomBuff();
}
void Star::customBuffs() {
    getWorld()->increaseScore(100);
    getWorld()->giveInvincibility(150);
}

void PeachFireball::doSomething() {
    if(getWorld()->coordIsBlocked(getX(), getY(), false, true, this)) {
        setIsAlive(false);
        return;
    } else {
        if(!getWorld()->coordIsBlocked(getX(), getY()-2, false, false, this)) {
            moveTo(getX(), getY()-2);
        }
        int x = getX() + (getDirection() == 180 ? -2 : 2);
        int y = getY();
        if(getWorld()->coordIsBlocked(x, y, false, false, this)) {
            setIsAlive(false);
            return;
        }
        moveTo(x, y);
    }
    
}

void Goomba::doSomething() {
    if(!getIsAlive()) {
        return;
    }
    if(getWorld()->overlappingPeach(getX(), getY(), true)) {
        return;
    }
    if(getWorld()->coordIsBlocked(getX() + (getDirection() == 0 ? 1 : -1), getY(), false, false, this)) {
        setDirection((getDirection() == 180 ? 0 : 180));
    }
    else if(!getWorld()->coordIsBlocked(getX() + (getDirection() == 0 ? SPRITE_WIDTH : -SPRITE_WIDTH), getY()-1, false, false, this)) {
        setDirection((getDirection() == 180 ? 0 : 180));
    }
    int x = getX() + (getDirection() == 180 ? -1 : 1);
    //cerr << x << endl;
    int y = getY();
    if(getWorld()->coordIsBlocked(x, y, false, false, this)) {
        return;
    } else {
        moveTo(x, y);
    }
    
}
