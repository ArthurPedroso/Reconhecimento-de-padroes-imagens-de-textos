#include "tcpserver.hpp"
#include <stdexcept>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <iostream>

void TcpServer::createSocket(int _port)
{
    char buffer[256];
    int n;

    m_serverSocketFD =  socket(AF_INET, SOCK_STREAM, 0);
    if (m_serverSocketFD < 0) throw std::exception();

    bzero((char *) &m_serverAddress, sizeof(m_serverAddress));

    m_serverAddress.sin_family = AF_INET;
    m_serverAddress.sin_addr.s_addr = INADDR_ANY;
    m_serverAddress.sin_port = htons(_port);

    if (bind(m_serverSocketFD, (struct sockaddr *) &m_serverAddress, sizeof(m_serverAddress)) < 0) 
        throw std::exception();
}

void TcpServer::waitForConnection()
{
    socklen_t clientLength;
    listen(m_serverSocketFD, 1);

    clientLength = sizeof(m_clientAddress);

    m_connectionSocketFD = accept(m_serverSocketFD, (struct sockaddr *) &m_clientAddress, &clientLength);
    if (m_connectionSocketFD < 0) throw std::exception();
}

void TcpServer::sendMsg(const char* _msg, int _msgSize)
{
    send(m_connectionSocketFD, _msg, _msgSize, 0);
}

TcpServer::TcpServer(int _port)
{
    createSocket(_port);
}

TcpServer::~TcpServer()
{
    if(m_serverSocketFD >= 0) close(m_serverSocketFD);
    if(m_connectionSocketFD >= 0) close(m_connectionSocketFD);
}