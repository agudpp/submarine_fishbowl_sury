/*
 * Configurer.cpp
 *
 *  Created on: Nov 16, 2014
 *      Author: agustin
 */

#include "Configurer.h"

#include <cmath>
#include <algorithm>
#include <vector>

#include <common/debug/DebugUtil.h>

namespace {



// Internal structure to be filled by the callback
//
struct InternalData {
    int numPoints;
    cv::Point points[4];

    InternalData() : numPoints(0) {}
};


// @brief Helper method that will sort the points in the correct places
//
void
sortPoints(const cv::Point points[4],
           cv::Point& tl,
           cv::Point& tr,
           cv::Point& bl,
           cv::Point& br)
{
    std::vector<cv::Point> sortedPoints;
    for (unsigned int i = 0; i < 4; ++i) sortedPoints.push_back(points[i]);
    std::sort(sortedPoints.begin(), sortedPoints.end(),
              [](const cv::Point& p1, const cv::Point& p2)
              {
                return p1.x < p2.x;
              }
              );

    // now have 2 left (first two elements) and two right, so we need to
    // detect up and down now for each case
    if (sortedPoints[0].y > sortedPoints[1].y) {
        tl = sortedPoints[0];
        bl = sortedPoints[1];
    } else {
        tl = sortedPoints[1];
        bl = sortedPoints[0];
    }

    if (sortedPoints[2].y > sortedPoints[3].y) {
        tr = sortedPoints[2];
        br = sortedPoints[3];
    } else {
        tr = sortedPoints[3];
        br = sortedPoints[2];
    }
}


// @brief Callback for mouse position detection
//
void
callBackFunc(int event, int x, int y, int flags, void* userdata)
{
    InternalData* data = static_cast<InternalData*>(userdata);

    if (event == cv::EVENT_RBUTTONDOWN) {
        // if we press right button
        debugGREEN("...%d\n", data->numPoints);
        if (data->numPoints > 0) {
            --(data->numPoints);
        }
        debugGREEN("...%d\n", data->numPoints);
        return;
    }

    // if it is not left do nothing
    if (event != cv::EVENT_LBUTTONDOWN) {
        return;
    }

    // we will add a point selected by the user if we don't have already all of
    // them
    if (data->numPoints >= 4) {
        return;
    }
    data->points[(data->numPoints)++] = cv::Point(x,y);
}


// @brief Helper method to draw the points / lines into the frame
//
void
drawDataIntoImg(cv::Mat& img, const InternalData& data)
{
    static const cv::Scalar_<int> color(0, 0, 255, 0);
    static const int circleThickness = 5;
    static const int lineThickness = 3;


    // draw the points
    for (int i = 0; i < data.numPoints; ++i) {
        cv::circle(img, data.points[i], 1, color, circleThickness);
    }

    // if we have 4 points then draw the lines
    if (data.numPoints == 4) {
        cv::Point tl,tr,bl,br;
        sortPoints(data.points, tl, tr, bl, br);

        // draw the for lines
        cv::line(img, tl, tr, color, lineThickness);
        cv::line(img, tl, bl, color, lineThickness);
        cv::line(img, bl, br, color, lineThickness);
        cv::line(img, tr, br, color, lineThickness);
    }

}

}

namespace img {


////////////////////////////////////////////////////////////////////////////////
Configurer::Configurer()
{

}

////////////////////////////////////////////////////////////////////////////////
Configurer::~Configurer()
{
}

////////////////////////////////////////////////////////////////////////////////
bool
Configurer::configureRect(const cv::Mat& img)
{
    // we will wait until the user press enter or something
    //          left click -> adds a point
    //          right click -> removes the last point.
    //          enter -> confirm the selection (if we have the 4 points)
    //          escape -> close the window aborting the selection

    cv::Mat currentImg = img;
    InternalData data;
    int currentNumPoints = 0;

    cv::namedWindow("Select Rectangle Game Area", cv::WINDOW_AUTOSIZE);
    cv::setMouseCallback("Select Rectangle Game Area", callBackFunc, &data);

    while (1) {

        // process the points if we need to add it in the img
        if (currentNumPoints != data.numPoints) {
            // we need to update the img
            currentImg = img.clone();
            drawDataIntoImg(currentImg, data);
            currentNumPoints = data.numPoints;
        }

        // show the image
        cv::imshow("Select Rectangle Game Area", currentImg);

        // check user input
        const int keyPressed = cv::waitKey(30);
        if (keyPressed == 27) {
            break;
        }
    }

    cv::destroyWindow("Select Rectangle Game Area");

    // now check if we have all the points selected
    const bool exitStatus = data.numPoints == 4;
    // if we have 4 points then draw the lines
    if (exitStatus) {
        sortPoints(data.points, m_tl, m_tr, m_bl, m_br);

        // TODO: remove this
        cv::Rect r = minimumSelRect();
        cv::rectangle(currentImg, r, cv::Scalar(0,255,0,0), 2);
        cv::imshow("Select Rectangle Game Area", currentImg);
        cv::waitKey(0);
    }
    return exitStatus;
}

////////////////////////////////////////////////////////////////////////////////
cv::Rect
Configurer::minimumSelRect(void) const
{
    // get the maximum and minimum
    cv::Point max, min;
    max.x = std::max(m_tr.x, m_br.x);
    max.y = std::max(m_tr.y, m_tl.y);
    min.x = std::min(m_tl.x, m_bl.x);
    min.y = std::min(m_bl.y, m_br.y);


    return cv::Rect(min.x, min.y, max.x - min.x, max.y - min.y);
}


} /* namespace img */
