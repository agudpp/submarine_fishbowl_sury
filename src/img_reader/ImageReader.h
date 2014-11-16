/*
 * ImageReader.h
 *
 *  Created on: Sep 7, 2014
 *      Author: agustin
 */

#ifndef IMAGEREADER_H_
#define IMAGEREADER_H_

#include <string>

#include <opencv2/videoio.hpp>

#include "ImgCapturedData.h"

namespace img {

// @brief This class will be used to read and capture the images from the
//        device (camera) or any other source.
//

class ImageReader
{
public:
    // Init data structure
    //
    struct InitData {
        std::string filePath;
        int deviceID;

        // here we need to set the values we want to configure the camera
        //
        int propWidth;
        int propHeight;


        InitData() : deviceID(-1) {}

        bool isDevice(void) const
        {
            return deviceID >= 0 && filePath.empty();
        }
        bool isVideo(void) const
        {
            return !isDevice();
        }
    };

public:
    ImageReader();
    ~ImageReader();

    // @brief This method will initialize the image reader module and prepare
    //        all the internal stuff to be ready to capture frames.
    // @param initData      The information needed to initialize the module
    // @return true on success | false otherwise
    //
    bool
    init(const InitData& initData);

    // @brief Uninit the ImageReader.
    //
    void
    uninit(void);

    // @brief This is the main method that we will be calling every time we need
    //        a new frame.
    // @param frameData     The frame data to be filled by this class.
    // @return true on success | false on error.
    //
    bool
    captureFrame(ImgCapturedData& frameData);

    // @brief This method will be called every time we finish using a frame
    //        so this class knows about it (to use a queue of frames if we need).
    // @param frameData     The frame to be released (not used anymore)
    //
    void
    releaseFrame(ImgCapturedData& frameData);


private:
    cv::VideoCapture m_capturer;
    ImgCapturedData m_frameCache;
};

} /* namespace img */

#endif /* IMAGEREADER_H_ */
