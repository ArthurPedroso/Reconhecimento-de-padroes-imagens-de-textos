#pragma once

#include <string>
#include <netinet/in.h>
#include <vector>
#include <mutex>
#include <queue>
#include <thread>
#include <atomic>

class TcpClient
{
private:
    struct sockaddr_in m_serverAddress;
    int m_clientSocketFD;


    std::atomic<int> m_threadCtrl {0};
    std::queue<std::vector<char>> m_msgBuffer;
    std::thread m_tcpReceiveThread;
    std::mutex m_msgBufferMutex;

    void createSocket(int _port, std::string _ip);
    void receiveMsg();
    void insertMsgOnBuffer(const std::vector<char>& _msg);
public:
    void connectToServer();
    bool readTopBufferMsg(std::vector<char>& _outMsg);
    

    TcpClient(int _port, std::string _ip);
    ~TcpClient();
};