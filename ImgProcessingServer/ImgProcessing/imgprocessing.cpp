#include "imgprocessing.hpp"
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/dnn/all_layers.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <memory>
#include <iostream>

using namespace cv;
using namespace std;
using namespace dnn;

//-----PRIVATE-----//
void ImgProcessing::loadEASTModel()
{
    m_textDetector = TextDetectionModel_EAST("Model/frozen_east_text_detection.pb");
    m_textDetector.setConfidenceThreshold(0.99f);
    m_textDetector.setNMSThreshold(0.4f);
}

void ImgProcessing::fourPointsTransform(const Mat& _frame, const Point2f _vertices[], Mat& _outResult)
{    
    const Size outputSize = Size(200, 64);
    Point2f targetVertices[4] = {
        Point(0, outputSize.height - 1),
        Point(0, 0), Point(outputSize.width - 1, 0),
        Point(outputSize.width - 1, outputSize.height - 1)
    };
    Mat rotationMatrix = getPerspectiveTransform(_vertices, targetVertices);
    warpPerspective(_frame, _outResult, rotationMatrix, outputSize);
}

void ImgProcessing::scaleQuadrangle(vector<Point2f>& _outQuad, const Point2f& _scale, float _maxWidth, float _maxHeight)
{
    _outQuad[0].x = (_outQuad[0].x * 2) - (_outQuad[0].x * _scale.x);
    _outQuad[0].y = (_outQuad[0].y * _scale.y);

    _outQuad[1].x = (_outQuad[1].x * 2) - (_outQuad[1].x * _scale.x);
    _outQuad[1].y = (_outQuad[1].y * 2) - (_outQuad[1].y * _scale.y);

    _outQuad[2].x = _outQuad[2].x * _scale.x;
    _outQuad[2].y = (_outQuad[2].y * 2) - (_outQuad[2].y * _scale.y);

    _outQuad[3].x = _outQuad[3].x * _scale.x;
    _outQuad[3].y = _outQuad[3].y * _scale.y;

    for(int i = 0; i < 4; i++)
    {
        if(_outQuad[i].x < 0) _outQuad[i].x = 0;
        if(_outQuad[i].y < 0) _outQuad[i].y = 0;
        if(_outQuad[i].x > _maxWidth) _outQuad[i].x = _maxWidth;
        if(_outQuad[i].y > _maxHeight) _outQuad[i].y = _maxHeight;
    }
}

void ImgProcessing::drawContourOnImg(const Mat& _img, vector<Point>& _contour)
{
    if (_contour.size() == 0) return;
    Scalar color = Scalar(0, 255, 0);
    drawContours(_img, vector<vector<Point>>(1, _contour), -1, color, 1, LINE_8);    
}

void ImgProcessing::applyPreProcessing(const Mat& _img, Mat& _outProcessedImg)
{
    const int morph_size = 1;
    const Mat element = getStructuringElement(
        MORPH_ELLIPSE,
        Size(2, 2),
        Point(1, 1));

    cvtColor(_img, _outProcessedImg, COLOR_BGR2GRAY);
    threshold(_outProcessedImg, _outProcessedImg, 0, 255, THRESH_BINARY | THRESH_OTSU);
    morphologyEx(_outProcessedImg, _outProcessedImg, MORPH_OPEN, element);
    morphologyEx(_outProcessedImg, _outProcessedImg, MORPH_CLOSE, element);
}

void ImgProcessing::detectText(const Mat& _img, std::vector<cv::Mat>& _outTextImgs, float _eastWidth = 640, float _eastHeight = 640)
{
    double detScale = 1.0;
    Size detInputSize = Size(_eastWidth, _eastHeight);
    Scalar detMean = Scalar(123.68, 116.78, 103.94);
    bool swapRB = true;
    m_textDetector.setInputParams(detScale, detInputSize, detMean, swapRB);

    // Detection
    vector<vector<Point>> detResults;
    Mat detectImage;
    cvtColor(_img, detectImage, COLOR_GRAY2BGR);
    m_textDetector.detect(detectImage, detResults);

    Mat cropped = _img;
    if (detResults.size() > 0) 
    {
        Mat recInput = _img;

        vector<vector<Point>> contours;
        for (uint i = 0; i < detResults.size(); i++)
        {
            vector<Point>& quadrangle = detResults[i];
            CV_CheckEQ(quadrangle.size(), (size_t)4, "");

            contours.emplace_back(quadrangle);
            vector<Point2f> quadrangle_2f;
            for (int j = 0; j < 4; j++)
            {
                quadrangle_2f.emplace_back(quadrangle[j]);
            }           

            scaleQuadrangle(quadrangle_2f, Point2f(1.016f, 1.016f), _img.cols, _img.rows);

            fourPointsTransform(recInput, &quadrangle_2f[0], cropped);

            _outTextImgs.push_back(cropped.clone());
        }
    }
}

string ImgProcessing::recognizeText(const Mat& _img)
{
    string outText;

    api->Init("Model/", "eng", tesseract::OEM_TESSERACT_LSTM_COMBINED);
    api->SetPageSegMode(tesseract::PSM_SINGLE_LINE);
    api->SetVariable("debug_file", "debugLog.log");

    api->SetImage(_img.data, _img.cols, _img.rows, 3, _img.step);
    outText = string(api->GetUTF8Text());

    api->End();
    return outText;
}
//-----PRIVATE-----//

//-----PUBLIC-----//
vector<string> ImgProcessing::applyOCR(const Mat& _img)
{
    vector<string> texts;
    vector<Mat> detectedTexts;
    Mat textImg;
    string text;

    applyPreProcessing(_img, textImg);    
    detectText(textImg, detectedTexts);

    for(int i = 0; i < detectedTexts.size(); i++)
    {
        text = recognizeText(detectedTexts[i]);
        if(text != "")
        {
            texts.push_back(text);
        }
    }

    return texts;
}

ImgProcessing::ImgProcessing()
{
    api = unique_ptr<tesseract::TessBaseAPI>(new tesseract::TessBaseAPI());

    loadEASTModel();
}
//-----PUBLIC-----//
