/*
 * ImageAnalyzer.h
 *
 *  Created on: Sep 7, 2014
 *      Author: agustin
 */

#ifndef IMAGEANALYZER_H_
#define IMAGEANALYZER_H_


#include <string>
#include <vector>

#include <opencv2/imgproc/imgproc.hpp>

#include <img_reader/ImgCapturedData.h>
#include <common/math/Vector2.h>

namespace img {

// @brief This class will be in charge of analyze the image and retrieve the
//        information we need, in this case will be a few info, it will be not
//        generic class since we don't care this now :).
//        We basically need to get:
//          - Submarine information (position / rotation / bounding box).
//          - Rectangle point positions in the image.
//          - Transformation matrix to transform an image into the plane of the
//            fishbowl axes.
//

class ImageAnalyzer
{
public:

    // Init data
    //
    struct InitData {
        // data used for the contours algorithm
        double maxValue;
        double threshold;

        // the base image to be used when comparing with the current frame
        cv::Mat baseImage;
    };

    // The resulting data obtaindes by this class.
    struct ResultData {
        // for now it will be the bounding box and the 4 vertices in the image
        // representing the corners of the fishbowl (we can always return the same
        // positions and assume that the camera will not be moved, for simplicity)
        //

        // submarine info
        Vec2I submPos;  // the center position of the bounding box
        Vec2I submSize; // the size of the bounding box (width / height)

        // float submRotation; // probably we can track rotation, not for now

        // fishbowl info (4 corners) (note used)
        Vec2I topLeft;
        Vec2I topRight;
        Vec2I bottomLeft;
        Vec2I bottomRight;
    };

public:
    ImageAnalyzer();
    ~ImageAnalyzer();

    // @brief Initialize the analyzer with the information we need. Here for
    //        now (to save some time) we can give the coordinates of the 4
    //        corners points in the image defining the fishbowl content we will
    //        use for the game.
    // @param initData      The initialization data
    // @return true on success | false otherwise
    //
    bool
    init(const InitData& initData);

    // @brief Uninit the module.
    //
    void
    uninit(void);

    // @brief Methods used to start the configuration process if needed for this
    //        algorithm
    //        The updateconfigFrame will return true until the user finish the
    //        configuration
    //
    void
    startConfiguration(void);
    bool
    updateConfigFrame(cv::Mat& frame);
    void
    finishConfiguration(void);

    // @brief This is the main method that should process (analyze) the image
    //        and extract the information we need from the frame.
    //        For this particular project we will only extract the bounding box
    //        information of the submarine. This should contain the position
    //        and size of the bounding box.
    // @param capturedData      The captured data we will process.
    // @param result            The resulting processed data
    // @return true on success | false otherwise
    //
    bool
    process(ImgCapturedData& capturedData, ResultData& result);


private:

    // @brief Apply the difference against the base image and save the result
    //        into the m_cache image. Assumes the cache image contains the
    //        image we want to compare already
    //
    bool
    diffFilterWithBase(void);

    // @brief Method that will process a frame and retrieve all the contours
    //        and put them into the cache structures
    //
    bool
    retrieveContours(cv::Mat& capturedData);

    // @brief testing method
    bool
    testMethod(cv::Mat& capturedData);

private:
    double m_maxValue;
    int m_threshold;
    int m_diffThreshold;
    cv::Mat m_cacheFrame;
    cv::Mat m_prevCacheFrame;
    cv::Mat m_baseImage;

    // cached vecs
    std::vector<std::vector<cv::Point> > m_contours;
    std::vector<cv::Vec4i> m_cHierarchy;
    std::vector<std::vector<cv::Point> > m_contoursPoly;
    std::vector<cv::Rect> m_boundRect;
    std::vector<cv::Point2f> m_center;
    std::vector<float> m_radius;

};

} /* namespace img */

#endif /* IMAGEANALYZER_H_ */
