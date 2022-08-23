#include "transmission.hpp"

#include <exception>

using namespace cv;
using namespace std;

bool Transmission::getFrameFromStream(VideoCapture& _stream)
{
    lock_guard<mutex> lock(m_mutex);
    return _stream.read(m_lastReceivedFrame);
}

void Transmission::startToReceiveVideo()
{
    VideoCapture stream(m_streamAddr);
    if (!stream.isOpened()) { m_threadCtrl--; return; }

    while(m_threadCtrl == 0)
    {
        if (!getFrameFromStream(stream))
        {
            m_threadCtrl--;
            return;
        }        
        this_thread::sleep_for(chrono::milliseconds(10));
    }
}

void Transmission::sendCommand(char _command)
{
    m_tcpServer.sendMsg(string(1, _command).c_str(), 2);
}

void Transmission::connectToRaspberryPi()
{
    cout << "waiting for connection...." << endl;
    m_tcpServer.waitForConnection();
    cout << "connected!" << endl;

    m_streamThread = thread(&Transmission::startToReceiveVideo, this);
}

Mat Transmission::getLastReceivedFrame()
{
    if(m_threadCtrl < 0) throw exception();

    lock_guard<mutex> lock(m_mutex);
    return m_lastReceivedFrame;
}

Transmission::Transmission(std::string _videoStreamAddr, int _port) : m_streamAddr(_videoStreamAddr), m_tcpServer(_port)
{

}

Transmission::~Transmission()
{
    m_threadCtrl++;
    m_streamThread.join();    
}
