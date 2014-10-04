/*
 * ImageAnalyzer.h
 *
 *  Created on: Sep 7, 2014
 *      Author: agustin
 */

#ifndef IMAGEANALYZER_H_
#define IMAGEANALYZER_H_


#include <string>

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
        std::string configFilePath;
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

        // fishbowl info (4 corners)
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

};

} /* namespace img */

#endif /* IMAGEANALYZER_H_ */
