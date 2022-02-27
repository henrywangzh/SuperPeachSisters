#include "Actor.h"
#include "StudentWorld.h"

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp

void Peach::doSomething() {
    int choice;
    if(getWorld()->getKey(choice)) {
        switch(choice) {
            case KEY_PRESS_LEFT:
                setDirection(180);
                if(!getWorld()->coordIsBlocked(getX()-4, getY())) {
                    
                    moveTo(getX()-4, getY());
                }
                break;
            case KEY_PRESS_RIGHT:
                setDirection(0);
                if(!getWorld()->coordIsBlocked(getX()+4, getY())) {
                    
                    moveTo(getX()+4, getY());
                }
                break;
            case KEY_PRESS_UP:
                if(!getWorld()->coordIsBlocked(getX(), getY()+4)) {
                    moveTo(getX(), getY()+4);
                }
                break;
            case KEY_PRESS_DOWN:
                if(!getWorld()->coordIsBlocked(getX(), getY()-4)) {
                    moveTo(getX(), getY()-4);
                }
                break;
                
        }
    }
}
