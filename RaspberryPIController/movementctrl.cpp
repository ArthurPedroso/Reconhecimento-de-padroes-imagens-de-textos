#include "movementctrl.hpp"
#include <wiringPi.h>
#include <iostream>
#include <chrono>
#include <thread>

#define LEFT_MOTOR_FRONT 27
#define LEFT_MOTOR_BACK 26
#define RIGHT_MOTOR_FRONT 29
#define RIGHT_MOTOR_BACK 28
#define LINEAR_MOVE_DURATION 1
#define ROTATION_DURATION 1333

using namespace std;

void MovementCtrl::moveForward()
{
    digitalWrite(RIGHT_MOTOR_FRONT, HIGH);
    digitalWrite(LEFT_MOTOR_FRONT, HIGH);

    this_thread::sleep_for(chrono::seconds(LINEAR_MOVE_DURATION));

    digitalWrite(RIGHT_MOTOR_FRONT, LOW);
    digitalWrite(LEFT_MOTOR_FRONT, LOW);
}

void MovementCtrl::moveBackwards()
{
    cout << "Back!" << endl;     

    digitalWrite(RIGHT_MOTOR_BACK, HIGH);
    digitalWrite(LEFT_MOTOR_BACK, HIGH);
    this_thread::sleep_for(chrono::seconds(LINEAR_MOVE_DURATION));
    digitalWrite(RIGHT_MOTOR_BACK, LOW);
    digitalWrite(LEFT_MOTOR_BACK, LOW);
}

void MovementCtrl::moveLeft()
{    
    cout << "Left!" << endl; 
    
    digitalWrite(RIGHT_MOTOR_FRONT, HIGH);
    digitalWrite(LEFT_MOTOR_BACK, HIGH);

    this_thread::sleep_for(chrono::milliseconds(ROTATION_DURATION));
    
    digitalWrite(RIGHT_MOTOR_FRONT, LOW);
    digitalWrite(LEFT_MOTOR_BACK, LOW);
}

void MovementCtrl::moveRight()
{
    cout << "Right!" << endl; 
    
    digitalWrite(RIGHT_MOTOR_BACK, HIGH);
    digitalWrite(LEFT_MOTOR_FRONT, HIGH);

    this_thread::sleep_for(chrono::milliseconds(ROTATION_DURATION));
    
    digitalWrite(RIGHT_MOTOR_BACK, LOW);
    digitalWrite(LEFT_MOTOR_FRONT, LOW);
}

void MovementCtrl::stop()
{
    cout << "Stop!" << endl;     
}
    
void MovementCtrl::executeMoveOrder(RpiCommand _rpiCommand)
{
    switch (_rpiCommand)
    {
    case RpiCommand::FRONT:
        moveForward();
        break;
    case RpiCommand::BACK:
        moveBackwards();
        break;
    case RpiCommand::RIGHT:
        moveRight();
        break;
    case RpiCommand::LEFT:
        moveLeft();
        break;
    case RpiCommand::STOP:
        stop();
        break;    
    default:
        cout << "Comando nao conhecido" << endl;
        throw exception();
        break;
    }
}

MovementCtrl::MovementCtrl()
{

    if (wiringPiSetup() < 0) 
    {
        cout << "Wiring PI setup Failed!!!" << endl;
        throw exception();
    }

    pinMode(LEFT_MOTOR_FRONT, OUTPUT);
    pinMode(LEFT_MOTOR_BACK, OUTPUT);
    pinMode(RIGHT_MOTOR_FRONT, OUTPUT);
    pinMode(RIGHT_MOTOR_BACK, OUTPUT);
}

