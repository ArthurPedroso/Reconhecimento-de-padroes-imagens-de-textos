#include "imgprocessingserver.hpp"
#include "ImgProcessing/imgprocessing.hpp"

#include <iostream>
#include <opencv2/opencv.hpp>
#include <tesseract/baseapi.h>
#include <regex>

#define DEFAULT_ADDR "rtsp://192.168.15.120:8555/unicast"
#define DEFAULT_PORT 8554

using namespace cv;
using namespace std;

char ImageProcessingServer::tryToStrToRpiCommand(std::string _string, RpiCommand& _outCommand)
{
    regex stop("STOP[A-Z]");
    regex left("LEFT[A-Z]");
    regex right("RIGHT[A-Z]");
    regex back("BACK[A-Z]");
    regex front("FRONT[A-Z]");
    smatch match;
    
    regex_search(_string, match, front);
    if(match.size() > 0)
    {
        _outCommand = RpiCommand::FRONT;
        return match[0].str()[match[0].str().size() - 1];
    }

    regex_search(_string, match, back);
    if(match.size() > 0)
    {
        _outCommand = RpiCommand::BACK;
        return match[0].str()[match[0].str().size() - 1];
    }

    regex_search(_string, match, left);
    if(match.size() > 0)
    {
        _outCommand = RpiCommand::LEFT;
        return match[0].str()[match[0].str().size() - 1];
    }

    regex_search(_string, match, right);
    if(match.size() > 0)
    {
        _outCommand = RpiCommand::RIGHT;
        return match[0].str()[match[0].str().size() - 1];
    }

    regex_search(_string, match, stop);
    if(match.size() > 0)
    {
        _outCommand = RpiCommand::STOP;
        return match[0].str()[match[0].str().size() - 1];
    }

    return 0;
}
int ImageProcessingServer::run()
{
    ImgProcessing imgProcessing;
    Mat streamFrame;
    vector<string> foundTexts;
    RpiCommand parsedCommand;
    char commandID = 0;
    char lastCommandID = 'A' - 1;;

    //Inicia a thread de leitura de video e o socket TCP
    m_transmission.connectToRaspberryPi();

    int i = 0;
    bool stop = false;
    do
    {
        streamFrame = m_transmission.getLastReceivedFrame();
        if(!streamFrame.empty())
        {
            cout << "Lendo frame " << ++i << "..." << endl;
            foundTexts = imgProcessing.applyOCR(streamFrame);
            cout << "Frame lido..." << endl;
            for(int j = 0; j < foundTexts.size(); j++)
            {
                commandID = tryToStrToRpiCommand(foundTexts[j], parsedCommand);
                cout << "Texto " << j << ":" << foundTexts[j] << endl;
                if(commandID == lastCommandID + 1)
                {
                    if(commandID != 'Z')
                        lastCommandID = commandID;
                    else
                        lastCommandID = 'A' - 1;
                    
                    m_transmission.sendCommand(parsedCommand);
                    cout << "Comando Enviado: " << lastCommandID << endl;

                    if(parsedCommand == RpiCommand::STOP) stop = true; 
                    break;
                }
            }
        }
    } while (!stop);
    return 0;
}
ImageProcessingServer::ImageProcessingServer() : m_transmission(DEFAULT_ADDR, DEFAULT_PORT)
{
}

ImageProcessingServer::ImageProcessingServer(std::string _videoStreamAddr, int _port) : m_transmission(_videoStreamAddr, _port)
{
    
}
