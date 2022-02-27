#include "Actor.h"
#include "StudentWorld.h"

using namespace std;
#include <iostream>
// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp

void Peach::bonk() {
    //check for invincibilities
    if(m_starPower || m_temporaryInvincibility) {
        return;
    }
    //apply all effects from bonking
    m_hp--;
    m_temporaryInvincibilityFramesLeft = 10;
    m_temporaryInvincibility = true;
    //remove powerups
    if(m_shootPower) {
        m_shootPower = false;
    }
    if(m_jumpPower) {
        m_jumpPower = false;
    }
    //check hp
    if(m_hp > 0) {
        getWorld()->playSound(SOUND_PLAYER_HURT);
    }
    if(m_hp <= 0) {
        setIsAlive(false);
    }
}
void Peach::doSomething() {
    if(!getIsAlive()) {
        return;
    }
    //do invincibility calculations
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
    //recharging calculations
    if(m_recharging) {
        m_rechargeFramesLeft--;
        if(m_rechargeFramesLeft == 0) {
            m_recharging = false;
        }
    }
    //attempt to bonk
    getWorld()->coordIsBlocked(getX(), getY(), true, false, nullptr);
    int choice;
    //jump movement
    if(m_remainingJumpDistance > 0) {
        if(getWorld()->coordIsBlocked(getX(), getY()+4, true, false, nullptr)) {
            m_remainingJumpDistance = 0;
        } else {
            moveTo(getX(), getY()+4);
            m_remainingJumpDistance--;
        }
    // fall movement
    } else if(!getWorld()->coordIsBlocked(getX(), getY(), false, false, nullptr) && !getWorld()->coordIsBlocked(getX(), getY()-3, false, false, nullptr)) {
            moveTo(getX(), getY()-4);
    }
    //directional user input calculations
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
                if(getWorld()->coordIsBlocked(getX(), getY()-1, false, false, nullptr)) {
                    if(!m_jumpPower) {
                        m_remainingJumpDistance = 8;
                    } else {
                        m_remainingJumpDistance = 12;
                    }
                    getWorld()->playSound(SOUND_PLAYER_JUMP);
                }
                break;
            case KEY_PRESS_SPACE:
                //check for powerups and generate peachfireball if possible
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

//release goodie if available, play bonk sound
void Block::bonk() {
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

//base doSomething for collider objects
void Collider::doSomething() {
    
}

//custom reaction for koopa on bonk/damage
void Koopa::onHit() {
    getWorld()->addActor(new Shell(getWorld(), getX(), getY(), getDirection()));
}

//kill enemy, increase score, play sound if enemy bonked
void Enemy::bonk() {
    //will only get run if peach bonks
    if(getWorld()->getPeachIsInvincible()) {
        getWorld()->playSound(SOUND_PLAYER_KICK);
        getWorld()->increaseScore(100);
        setIsAlive(false);
    }
    onHit();
}

//base doSomething() for level completion objects (flag and mario), checks for peach overlap to perform ending operations
void Goal::doSomething() {
    if(!getIsAlive()) {
        return;
    }
    if(getWorld()->overlappingPeach(getX(), getY(), false)) {
        getWorld()->increaseScore(1000);
        setIsAlive(false);
        endAction();
    }
}

//different results depending on flag or mario
void Flag::endAction() {
    getWorld()->setLevelFinished();
}
void Mario::endAction() {
    getWorld()->setGameWon();
}

//move goodies appropriately, allow for peach to collect powerup
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
//custom powerups for each goodie
void Flower::customBuffs() {
    getWorld()->increaseScore(50);
    getWorld()->setPeachHP(2);
    getWorld()->giveFlowerBuff();
}
void Mushroom::customBuffs() {
    getWorld()->increaseScore(75);
    getWorld()->setPeachHP(2);
    getWorld()->giveMushroomBuff();
}
void Star::customBuffs() {
    getWorld()->increaseScore(100);
    getWorld()->giveInvincibility(150);
}

//checks for collision with everything other than peach
void FriendlyProjectile::collision() {
    if(getWorld()->coordIsBlocked(getX(), getY(), false, true, this)) {
        setIsAlive(false);
        return;
    }
}
//checks for only peach
void PiranhaFireball::collision() {
    if(getWorld()->overlappingPeach(getX(), getY(), true)) {
        setIsAlive(false);
        return;
    }
}
//move projectiles
void Projectile::doSomething() {
    collision();
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
//move enemies, applies to all moving enemies
void Enemy::doSomething() {
    if(!getIsAlive()) {
        return;
    }
    if(getWorld()->overlappingPeach(getX(), getY(), true)) {
        return;
    }
    //change direction as needed
    if(getWorld()->coordIsBlocked(getX() + (getDirection() == 0 ? 1 : -1), getY(), false, false, this)) {
        setDirection((getDirection() == 180 ? 0 : 180));
    }
    else if(!getWorld()->coordIsBlocked(getX() + (getDirection() == 0 ? SPRITE_WIDTH : -SPRITE_WIDTH), getY()-1, false, false, this)) {
        setDirection((getDirection() == 180 ? 0 : 180));
    }
    
    int x = getX() + (getDirection() == 180 ? -1 : 1);
    int y = getY();
    
    if(getWorld()->coordIsBlocked(x, y, false, false, this)) {
        return;
    } else {
        moveTo(x, y);
    }
}

//override doSomething() for piranha since it does not move
void Piranha::doSomething() {
    if(!getIsAlive()) {
        return;
    }
    increaseAnimationNumber();
    if(getWorld()->overlappingPeach(getX(), getY(), true)) {
        return;
    }
    if(!(getWorld()->getPeachY() < getY()+1.5*SPRITE_HEIGHT && getWorld()->getPeachY() > getY()-1.5*SPRITE_HEIGHT)) {
        
        return;
    }
    if(getWorld()->getPeachX() < getX()) {
        setDirection(180);
    } else {
        setDirection(0);
    }
    if(m_firingDelay > 0) {
        m_firingDelay--;
        return;
    } else {
        if(getWorld()->getPeachX() < getX()+8*SPRITE_WIDTH && getWorld()->getPeachX() > getX()-8*SPRITE_WIDTH) {
            getWorld()->addActor(new PiranhaFireball(getWorld(), getX(), getY(), getDirection()));
            getWorld()->playSound(SOUND_PIRANHA_FIRE);
            m_firingDelay = 40;
        }
    }
}

//damage an enemy
void Enemy::damage() {
    getWorld()->increaseScore(100);
    setIsAlive(false);
    onHit();
}
