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
ImageAnalyzer::diffFilterWithBase(void)
{
    if (m_baseImage.data == 0) {
        debugERROR("No base image set?\n");
        return false;
    }
    ASSERT(m_cacheFrame.depth() != sizeof(uchar));

    const int channels = m_cacheFrame.channels();
    int nRows = m_cacheFrame.rows;
    int nCols = m_cacheFrame.cols * channels;

    if (m_cacheFrame.isContinuous()) {
        nCols *= nRows;
        nRows = 1;
    }
    // what we need to do now is check for the difference in values with the
    // base image and apply a threshold
    if (m_cacheFrame.size == m_baseImage.size) {
        uchar* pc = 0, *pp = 0;
        for (int i = 0; i < nRows; ++i) {
            // pointer to the current and previous
            pc = m_cacheFrame.ptr<uchar>(i);
            pp = m_baseImage.ptr<uchar>(i);
            for (int j = 0; j < nCols; ++j) {
                // check the difference
                const int diff = std::abs(int(pc[j]) - int(pp[j]));
                if (diff < m_diffThreshold) {
                    // reset to color 0
                    pc[j] = 0;
                }
            }
        }
    }

    return true;
}

////////////////////////////////////////////////////////////////////////////////
bool
ImageAnalyzer::retrieveContours(cv::Mat& capturedData)
{
    cv::cvtColor(capturedData, m_cacheFrame, cv::COLOR_BGR2GRAY);

    ASSERT(m_cacheFrame.depth() != sizeof(uchar));

    const int channels = m_cacheFrame.channels();
    int nRows = m_cacheFrame.rows;
    int nCols = m_cacheFrame.cols * channels;

    if (m_cacheFrame.isContinuous()) {
        nCols *= nRows;
        nRows = 1;
    }
    // what we need to do now is check for the difference in values from the
    // previous image and apply a threshold
    if (m_cacheFrame.size == m_baseImage.size) {
        uchar* pc = 0, *pp = 0;
        for (int i = 0; i < nRows; ++i) {
            // pointer to the current and previous
            pc = m_cacheFrame.ptr<uchar>(i);
            pp = m_baseImage.ptr<uchar>(i);
            for (int j = 0; j < nCols; ++j) {
                // check the difference
                const int diff = std::abs(int(pc[j]) - int(pp[j]));
                if (diff < m_diffThreshold) {
                    // reset to color 0
                    pc[j] = 0;
                }
            }
        }
    }



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
bool
ImageAnalyzer::testMethod(cv::Mat& capturedData)
{
    cv::cvtColor(capturedData, m_cacheFrame, cv::COLOR_BGR2GRAY);

    // now apply the diff against the base image
    diffFilterWithBase();

    //cv::blur(m_cacheFrame, m_cacheFrame, cv::Size(3,3));
    cv::GaussianBlur(m_cacheFrame, m_cacheFrame, cv::Size(7,7), 1.5, 1.5);
    cv::threshold(m_cacheFrame, m_cacheFrame, m_threshold, m_maxValue, cv::THRESH_BINARY);
    //cv::Canny(m_cacheFrame, m_cacheFrame, m_threshold, m_diffThreshold, 3, 1.0);


    return true;
}

////////////////////////////////////////////////////////////////////////////////
ImageAnalyzer::ImageAnalyzer() :
    m_diffThreshold(0)
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
    m_baseImage = initData.baseImage;
    if (m_baseImage.data == 0) {
        debugERROR("Setting a base image null?\n");
        return false;
    }
    cv::cvtColor(m_baseImage, m_baseImage, cv::COLOR_BGR2GRAY);

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
    cv::createTrackbar("Diff Threshold", "ConfigAnalyzer", &m_diffThreshold, 255);
}

////////////////////////////////////////////////////////////////////////////////
bool
ImageAnalyzer::updateConfigFrame(cv::Mat& frame)
{
    // here we need to process the frame to get the contours
    /*if (!retrieveContours(frame)) {
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
    */

    // TODO remove this
    testMethod(frame);
    cv::imshow("ConfigAnalyzer", m_cacheFrame);


    // check if the user press escape to finish the config
    const int keyPressed = cv::waitKey(100);
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
