/*
 * ImageReader.cpp
 *
 *  Created on: Sep 7, 2014
 *      Author: agustin
 */

#include "ImageReader.h"

#include <common/debug/DebugUtil.h>

namespace img {

ImageReader::ImageReader()
{

}

ImageReader::~ImageReader()
{
}

////////////////////////////////////////////////////////////////////////////////
bool
ImageReader::init(const InitData& initData)
{
    // uninit
    uninit();

    if (initData.isDevice()) {
        if (!m_capturer.open(initData.deviceID)) {
            debugERROR("Error trying to open the device %d\n", initData.deviceID);
            return false;
        }
        // else was good
        return true;
    }
    // check if it is file
    if (!initData.isVideo()) {
        debugERROR("The information to initialize the reader is wrong, it is not"
                   " a device nor file...\n");
        return false;
    }

    if (!m_capturer.open(initData.filePath)) {
        debugERROR("Error trying to open the file %s\n", initData.filePath.c_str());
        return false;
    }

    // alles gut
    return true;
}

////////////////////////////////////////////////////////////////////////////////
void
ImageReader::uninit(void)
{
    if (m_capturer.isOpened()) {
        m_capturer.release();
    }
}

////////////////////////////////////////////////////////////////////////////////
bool
ImageReader::captureFrame(ImgCapturedData& frameData)
{
    // from now we will only just capture one by one, we can create a queue
    // of frames captured later, not now
    // first reset the time when we are capturing the frame
    m_frameCache.timestamp.reset();
    if (!m_capturer.read(m_frameCache.frame)) {
        debugERROR("Error trying to read a frame from the capturer\n");
        return false;
    }

    // now we can copy the matrix, since this is a shallow copy there will be
    // no problems
    frameData = m_frameCache;
    return true;
}

////////////////////////////////////////////////////////////////////////////////
void
ImageReader::releaseFrame(ImgCapturedData& frameData)
{
    // we don't need this method now, we will just skip
}

} /* namespace img */
