#include "tcpclient.hpp"

#include <stdexcept>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>

#include <iostream>

#define BUFFER_SIZE 3

using namespace std;

void TcpClient::createSocket(int _port, string _ip)
{
    m_clientSocketFD = socket(AF_INET, SOCK_STREAM, 0);

    if (m_clientSocketFD < 0) 
        throw exception();


    bzero((char *) &m_serverAddress, sizeof(m_serverAddress));

    m_serverAddress.sin_family = AF_INET;
    m_serverAddress.sin_port = htons(_port);

    if (inet_pton(AF_INET, _ip.c_str(), &m_serverAddress.sin_addr) <= 0) 
    {
        throw exception();
    }
}

void TcpClient::connectToServer()
{
    if (connect(m_clientSocketFD, (struct sockaddr *) &m_serverAddress, sizeof(m_serverAddress)) < 0) 
        throw exception();    

    m_tcpReceiveThread = thread(&TcpClient::receiveMsg, this);
}

bool TcpClient::readTopBufferMsg(std::vector<char>& _outMsg)
{   
    if(m_threadCtrl < 0) throw std::exception();

    lock_guard<mutex> lock(m_msgBufferMutex);
    if(m_msgBuffer.size() <= 0)
        return false;
    else
    {
        _outMsg = m_msgBuffer.front();
        m_msgBuffer.pop();
        return true;
    }
}


void TcpClient::insertMsgOnBuffer(const std::vector<char>& _msg)
{    
    lock_guard<mutex> lock(m_msgBufferMutex);
    
    m_msgBuffer.push(_msg);
    if(m_msgBuffer.size() > 5) m_msgBuffer.pop();
}

void TcpClient::receiveMsg()
{
    int msgLen;
    char msg[256];

    while (m_threadCtrl == 0)
    {
        bzero(msg,256);
        msgLen = read(m_clientSocketFD, msg, 255);

        if (msgLen < 0) 
        {
            cout << "Msg too small" << endl;
            m_threadCtrl--;
            throw exception();
        }

        cout << "reading buffer" << endl;
        
        insertMsgOnBuffer(std::vector<char>(msg, msg + msgLen));
    }
}

TcpClient::TcpClient(int _port, string _ip)
{
    createSocket(_port, _ip);
}

TcpClient::~TcpClient()
{
    if(m_clientSocketFD >= 0) close(m_clientSocketFD);  
    m_threadCtrl++;
    m_tcpReceiveThread.join();
}
