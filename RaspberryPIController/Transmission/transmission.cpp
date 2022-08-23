#include "transmission.hpp"
#include <iostream>
#include <chrono>
#include <thread>

using namespace std;


void Transmission::establishConnection()
{
    m_tcpClient.connectToServer();    
}

char Transmission::receiveMsgFromServer()
{
    std::vector<char> msg;
    
    while(!m_tcpClient.readTopBufferMsg(msg)) 
    {
        cout << "No messages to read!" << endl;
        this_thread::sleep_for(chrono::seconds(1));
    }
    
    cout << "Msg size: " << msg.size();
    cout << " Msg: " << (int)msg[0] << endl;

    if(msg.size() != 2) {  throw exception(); }

    return msg[0];
}

Transmission::Transmission(string _serverIP, int _port) : m_tcpClient(_port, _serverIP)
{
    
}
