/*
 * ImageAnalyzer.cpp
 *
 *  Created on: Sep 7, 2014
 *      Author: agustin
 */

#include "ImageAnalyzer.h"

#include <opencv2/highgui/highgui.hpp>

#include <common/debug/DebugUtil.h>


namespace {
cv::RNG g_rng(12345);
}


namespace img {

////////////////////////////////////////////////////////////////////////////////
bool
ImageAnalyzer::retrieveContours(cv::Mat& capturedData)
{

    cv::cvtColor(capturedData, m_cacheFrame, cv::COLOR_BGR2GRAY);
    cv::blur(m_cacheFrame, m_cacheFrame, cv::Size(3,3));
    cv::threshold(m_cacheFrame, m_cacheFrame, m_threshold, m_maxValue, cv::THRESH_BINARY);

    // now we will get the contours
    m_contours.clear();
    m_cHierarchy.clear();
    cv::findContours(m_cacheFrame,
                     m_contours,
                     m_cHierarchy,
                     cv::RETR_TREE,
                     cv::CHAIN_APPROX_SIMPLE,
                     cv::Point(0, 0));

    // now find the contours
    m_contoursPoly.resize(m_contours.size());
    m_boundRect.resize(m_contours.size());
    m_center.resize(m_contours.size());
    m_radius.resize(m_contours.size());

    for (unsigned int i = 0; i < m_contours.size(); ++i) {
        cv::approxPolyDP(cv::Mat(m_contours[i]), m_contoursPoly[i], 3.0, true);
        m_boundRect[i] = cv::boundingRect(cv::Mat(m_contoursPoly[i]));
        cv::minEnclosingCircle((cv::Mat) m_contoursPoly[i], m_center[i], m_radius[i]);
    }

    return true;
}

////////////////////////////////////////////////////////////////////////////////
ImageAnalyzer::ImageAnalyzer()
{
}

////////////////////////////////////////////////////////////////////////////////
ImageAnalyzer::~ImageAnalyzer()
{
}

////////////////////////////////////////////////////////////////////////////////
bool
ImageAnalyzer::init(const InitData& initData)
{
    m_maxValue = initData.maxValue;
    m_threshold = initData.threshold;
    return true;
}


////////////////////////////////////////////////////////////////////////////////
void
ImageAnalyzer::uninit(void)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////
void
ImageAnalyzer::startConfiguration(void)
{
    // create the window
    cv::namedWindow("ConfigAnalyzer", cv::WINDOW_AUTOSIZE);
    cv::createTrackbar("Threshold", "ConfigAnalyzer", &m_threshold, 255);
}

////////////////////////////////////////////////////////////////////////////////
bool
ImageAnalyzer::updateConfigFrame(cv::Mat& frame)
{
    // here we need to process the frame to get the contours
    if (!retrieveContours(frame)) {
        debugERROR("Couldn't get the contours!\n");
        return false;
    }

    // paint the  frame
    cv::Mat drawing = cv::Mat::zeros(m_cacheFrame.size(), CV_8UC3);
    for (int i = 0; i < m_contours.size(); i++) {
        cv::Scalar color = cv::Scalar(g_rng.uniform(0, 255),
                                      g_rng.uniform(0, 255),
                                      g_rng.uniform(0, 255));
        cv::drawContours(drawing,
                         m_contoursPoly,
                         i,
                         color,
                         1,
                         8,
                         std::vector<cv::Vec4i>(),
                         0,
                         cv::Point());
        cv::rectangle(drawing,
                      m_boundRect[i].tl(),
                      m_boundRect[i].br(),
                      color,
                      2,
                      8,
                      0);
        cv::circle(drawing, m_center[i], (int) m_radius[i], color, 2, 8, 0);
    }
    cv::imshow("ConfigAnalyzer", drawing);

    // check if the user press escape to finish the config
    const int keyPressed = cv::waitKey(30);
    if (keyPressed == 27) {
        return false;
    }

    return true;
}

////////////////////////////////////////////////////////////////////////////////
void
ImageAnalyzer::finishConfiguration(void)
{
    cv::destroyWindow("ConfigAnalyzer");
}

////////////////////////////////////////////////////////////////////////////////
bool
ImageAnalyzer::process(ImgCapturedData& capturedData, ResultData& result)
{
    // for now we will do the most basic approach of all. Just assume that
    // we will only detect the submarine and there will be not other things
    // around there
    if (!retrieveContours(capturedData.frame)) {
        debugERROR("Couldn't get the contours!\n");
        return false;
    }

    // for now we will just return the first contour we find
    // TODO!

    return true;

}



} /* namespace img */
