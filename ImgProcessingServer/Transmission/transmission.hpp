#pragma once

#include <opencv2/opencv.hpp>
#include <mutex>
#include <atomic>
#include <thread>
#include <string>
#include "tcpserver.hpp"

class Transmission
{
private:
    std::atomic<int> m_threadCtrl {0};
    cv::Mat m_lastReceivedFrame;
    std::mutex m_mutex;
    std::thread m_streamThread;
    std::string m_streamAddr;

    TcpServer m_tcpServer;
    
    bool getFrameFromStream(cv::VideoCapture& _stream);
    void startToReceiveVideo();

public:
    void sendCommand(char _command);
    void connectToRaspberryPi();
    cv::Mat getLastReceivedFrame();


    Transmission(std::string _videoStreamAddr, int _port);
    ~Transmission();
};