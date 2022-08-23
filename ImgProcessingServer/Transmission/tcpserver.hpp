#pragma once

#include <netinet/in.h>
#include <string>

class TcpServer
{
private:
    struct sockaddr_in m_serverAddress;
    struct sockaddr_in m_clientAddress;
    int m_serverSocketFD;
    int m_connectionSocketFD;

    void createSocket(int _port);
public:
    void waitForConnection();
    void sendMsg(const char* _msg, int _msgSize);

    TcpServer(int _port);
    ~TcpServer();
};