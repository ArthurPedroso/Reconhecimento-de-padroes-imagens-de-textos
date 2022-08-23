#pragma once

#include "Transmission/transmission.hpp"
#include "movementctrl.hpp"

#include <string>

class RpiController
{
private:
    Transmission m_transmission;
    MovementCtrl m_moveController;
public:
    int run();
    RpiController();
    RpiController(std::string _serverAddr, int _serverTcpPort);
};