/*
 * ImageAnalyzer.h
 *
 *  Created on: Sep 7, 2014
 *      Author: agustin
 */

#ifndef IMAGEANALYZER_H_
#define IMAGEANALYZER_H_


#include <string>



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


};

} /* namespace img */

#endif /* IMAGEANALYZER_H_ */
