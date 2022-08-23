#include "rpicontroller.hpp"
#include "rpicommands.hpp"

#define DEFAULT_SERVER_IP "192.168.0.18"
#define DEFAULT_SERVER_PORT 8554


int RpiController::run()
{
    RpiCommand receivedCommand;
    m_transmission.establishConnection();

    do
    {
        receivedCommand = (RpiCommand)m_transmission.receiveMsgFromServer();

        m_moveController.executeMoveOrder(receivedCommand);

    } while (receivedCommand != RpiCommand::STOP);  

    return 0;
}

RpiController::RpiController() : m_transmission(DEFAULT_SERVER_IP, DEFAULT_SERVER_PORT)
{
    
}

RpiController::RpiController(std::string _serverAddr, int _serverTcpPort) : m_transmission(_serverAddr, _serverTcpPort)
{
    
}

