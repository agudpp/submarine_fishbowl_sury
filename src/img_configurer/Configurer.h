/*
 * Configurer.h
 *
 *  Created on: Nov 16, 2014
 *      Author: agustin
 */

#ifndef CONFIGURER_H_
#define CONFIGURER_H_

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

namespace img {

// @brief This class will take a image and will let the user configure
//        the area that we need to analyze of the image and the other parameters
//


class Configurer
{
public:
    Configurer();
    ~Configurer();

    // @brief Show the UI to the user to let him configure the area where we
    //        want to analyze.
    //        User interface:
    //          left click -> adds a point
    //          right click -> removes the last point.
    //          escape -> close the window with the current selection
    // @param img       The image where we have to select the area from.
    // @return true on success | false if the user didn't select the area or error
    // @note call getArea() later to retrieve the information
    //
    bool
    configureRect(const cv::Mat& img);

    // @brief Select the background base image to be used to compare (diff)
    //        with the others.
    // @param frame         The image to show the user if we want to use as default
    // @return true if the user select the current frame as base image
    //         false if we need to call again this method because the user discard
    //         the img
    //
    bool
    selectBaseImage(const cv::Mat& img);

    // @brief Return the four points selected by the user.
    //
    inline const cv::Point&
    topLeft(void) const;
    inline const cv::Point&
    topRight(void) const;
    inline const cv::Point&
    bottomLeft(void) const;
    inline const cv::Point&
    bottomRight(void) const;

    // @brief Get the minimum rectangle containing the current user selection
    //
    cv::Rect
    minimumSelRect(void) const;


private:
    cv::Point m_tl;
    cv::Point m_tr;
    cv::Point m_br;
    cv::Point m_bl;
};








////////////////////////////////////////////////////////////////////////////////

inline const cv::Point&
Configurer::topLeft(void) const
{
    return m_tl;
}
inline const cv::Point&
Configurer::topRight(void) const
{
    return m_tr;
}
inline const cv::Point&
Configurer::bottomLeft(void) const
{
    return m_bl;
}
inline const cv::Point&
Configurer::bottomRight(void) const
{
    return m_br;
}



} /* namespace img */

#endif /* IMAGECONFIGURER_H_ */
