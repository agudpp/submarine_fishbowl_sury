/*
 * ImgCapturedData.h
 *
 *  Created on: Sep 7, 2014
 *      Author: agustin
 */

#ifndef IMGCAPTUREDDATA_H_
#define IMGCAPTUREDDATA_H_

#include <common/timestamp/Timestamp.h>
#include <opencv2/core/core.hpp>

namespace img {


// This structure will define the data we will get every time we capture a frame
// from the device / any source.
//
struct ImgCapturedData {
    // the timestamp of the image captured.
    Timestamp timestamp;
    // the frame captured info
    cv::Mat frame;

};


}



#endif /* IMGCAPTUREDDATA_H_ */
