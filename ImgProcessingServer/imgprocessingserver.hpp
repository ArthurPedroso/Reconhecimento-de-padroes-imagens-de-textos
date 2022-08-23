#pragma once

#include "Transmission/transmission.hpp"
#include "rpicommands.hpp"

#include <string>

class ImageProcessingServer
{
private:
    Transmission m_transmission;

    char tryToStrToRpiCommand(std::string _string, RpiCommand& _outCommand);
public:
    int run();

    ImageProcessingServer();
    ImageProcessingServer(std::string _videoStreamAddr, int _port);
};