#pragma once

#include <string>
#include <opencv2/core/core.hpp>
#include <opencv2/dnn.hpp>
#include <memory>
#include <tesseract/baseapi.h>

class ImgProcessing
{
private:
    std::unique_ptr<tesseract::TessBaseAPI> api;
    cv::dnn::TextDetectionModel_EAST m_textDetector;

    void loadEASTModel();
    void fourPointsTransform(const cv::Mat& _frame, const cv::Point2f _vertices[], cv::Mat& _outResult);
    void scaleQuadrangle(std::vector<cv::Point2f>& _outQuad, const cv::Point2f& _scale, float _maxWidth, float _maxHeight);

    void drawContourOnImg(const cv::Mat& _img, std::vector<cv::Point>& _contour);
    
    void applyPreProcessing(const cv::Mat& _img, cv::Mat& _outProcessedImg);
    void detectText(const cv::Mat& _img, std::vector<cv::Mat>& _outTextImgs, float _eastWidth, float _eastHeight);
    std::string recognizeText(const cv::Mat& _img);
public:
    std::vector<std::string> applyOCR(const cv::Mat& _img);

    ImgProcessing();
};