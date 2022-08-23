#pragma once

#include "tcpclient.hpp"

class Transmission
{
private:
    TcpClient m_tcpClient;

public:
    void establishConnection();
    char receiveMsgFromServer();

    Transmission(std::string _serverIP, int _port);
};