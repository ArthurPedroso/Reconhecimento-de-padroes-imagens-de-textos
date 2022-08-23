#pragma once 

#include "rpicommands.hpp"

class MovementCtrl
{
private:
    void moveForward();
    void moveBackwards();
    void moveLeft();
    void moveRight();
    void stop();
public:
    void executeMoveOrder(RpiCommand _rpiCommand);

    MovementCtrl();
};
