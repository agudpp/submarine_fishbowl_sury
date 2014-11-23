#include <opencv2/highgui/highgui.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <iostream>

#include <common/debug/DebugUtil.h>
#include <img_reader/ImageReader.h>
#include <img_reader/ImgCapturedData.h>
#include <img_configurer/Configurer.h>
#include <img_analyzer/ImageAnalyzer.h>


using namespace std;
using namespace cv;

Mat src; Mat src_gray;
int thresh = 100;
int max_thresh = 255;
RNG rng(12345);

/** @function thresh_callback */
void thresh_callback(int, void* )
{
  Mat threshold_output;
  vector<vector<Point> > contours;
  vector<Vec4i> hierarchy;

  /// Detect edges using Threshold
  threshold( src_gray, threshold_output, thresh, 255, THRESH_BINARY );
  /// Find contours
  findContours( threshold_output, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0) );

  /// Approximate contours to polygons + get bounding rects and circles
  vector<vector<Point> > contours_poly( contours.size() );
  vector<Rect> boundRect( contours.size() );
  vector<Point2f>center( contours.size() );
  vector<float>radius( contours.size() );

  for( int i = 0; i < contours.size(); i++ )
     { approxPolyDP( Mat(contours[i]), contours_poly[i], 3, true );
       boundRect[i] = boundingRect( Mat(contours_poly[i]) );
       minEnclosingCircle( (Mat)contours_poly[i], center[i], radius[i] );
     }


  /// Draw polygonal contour + bonding rects + circles
  Mat drawing = Mat::zeros( threshold_output.size(), CV_8UC3 );
  for( int i = 0; i< contours.size(); i++ )
     {
       Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
       drawContours( drawing, contours_poly, i, color, 1, 8, vector<Vec4i>(), 0, Point() );
       rectangle( drawing, boundRect[i].tl(), boundRect[i].br(), color, 2, 8, 0 );
       circle( drawing, center[i], (int)radius[i], color, 2, 8, 0 );
     }

  /// Show in a window
  namedWindow( "Contours", WINDOW_AUTOSIZE );
  imshow( "Contours", drawing );
}


void treshChanger(int val,void*)
{
    thresh = val;
}


static int
real_main(int argc, char** argv)
{
    img::ImageReader imgReader;
    img::ImgCapturedData frameData;
    img::Configurer configurer;
    img::ImageAnalyzer analyzer;

    // open the device
    img::ImageReader::InitData initData;
    if (argc > 1) {
        initData.filePath = argv[1];
    } else {
        initData.filePath = "./media/referencia.wmv";
    }

    if (!imgReader.init(initData)) {
        debugERROR("Cannot init, abort\n");
        return -1;
    }

    // read one img and configure it
    if (!imgReader.captureFrame(frameData)) {
        debugERROR("mmmm something is not ok, or maybe there are nothing to capture?\n");
        return -1;
    }
    if (!configurer.configureRect(frameData.frame)) {
        debugERROR("Error configuring the frame\n");
        return -1;
    }

    // everything was ok? print the data
    debugBLUE("tl: %d, %d\ttr: %d, %d\tbl: %d, %d\tbr: %d, %d\n",
              configurer.topLeft().x, configurer.topLeft().y,
              configurer.topRight().x, configurer.topRight().y,
              configurer.bottomLeft().x, configurer.bottomLeft().y,
              configurer.bottomRight().x, configurer.bottomRight().y);

    // now we need to select the base image
    bool baseSelected = false;
    while (imgReader.captureFrame(frameData) && !baseSelected) {
        baseSelected = configurer.selectBaseImage(frameData.frame);
    }

    if (!baseSelected) {
        debugERROR("We wasn't able to select a base image before reading all the frames?\n");
        return -1;
    }

    // now in frame data we have the base frame we will use to compare
    // configure the analyzer and while we capture frames
    img::ImageAnalyzer::InitData iaInitData;
    iaInitData.maxValue = 255;
    iaInitData.threshold = 80;
    iaInitData.baseImage = frameData.frame(configurer.minimumSelRect());
    if (!analyzer.init(iaInitData)) {
        debugERROR("Error initializing the analyzer\n");
        return -3;
    }

    // configure it
    analyzer.startConfiguration();

#ifdef DEBUG
    // debug window showing  the captured frame
    cv::namedWindow("DEBUG_LAST_FRAME", cv::WINDOW_AUTOSIZE);
#endif
    while (imgReader.captureFrame(frameData)) {
#ifdef DEBUG
        // debug
        cv::imshow("DEBUG_LAST_FRAME", frameData.frame);
#endif

        // here we need to only use a certain rectangle of the image
        frameData.frame = frameData.frame(configurer.minimumSelRect());

        if (!analyzer.updateConfigFrame(frameData.frame)) {
            // user exit
            break;
        }
    }
    analyzer.finishConfiguration();
    return 0;
}

int main(int argc, char** argv)
{
     /*// Read image from file
     Mat img = imread("./media/sample_img.png");

      //if fail to read the image
     if ( img.empty() )
     {
          cout << "Error loading the image" << endl;
          return -1;
     }

      //Create a window
     namedWindow("My Window", 1);

      //set the callback function for any mouse event
     //setMouseCallback("My Window", CallBackFunc, NULL);

      //show the image
     imshow("My Window", img);

      // Wait until user press some key
     waitKey(0);

      return 0;*/


    // real main stuff
    return real_main(argc, argv);

    VideoCapture cap("./media/sample_video.wmv"); // open the video camera no. 0

       if (!cap.isOpened())  // if not success, exit program
       {
           cout << "Cannot open the video cam" << endl;
           return -1;
       }

      double dWidth = cap.get(CAP_PROP_FRAME_WIDTH); //get the width of frames of the video
      double dHeight = cap.get(CAP_PROP_FRAME_HEIGHT); //get the height of frames of the video

       cout << "Frame size : " << dWidth << " x " << dHeight << endl;

       namedWindow("MyVideo",WINDOW_AUTOSIZE); //create a window called "MyVideo"
       namedWindow("Blur",WINDOW_AUTOSIZE); //create a window called "MyVideo"

       // create the slider
       createTrackbar("TreshModifier", "MyVideo", &thresh, 255);

       while (1)
       {
           Mat frame;

           bool bSuccess = cap.read(frame); // read a new frame from video

            if (!bSuccess) //if not success, break loop
           {
                cout << "Cannot read a frame from video stream" << endl;
                break;
           }

           imshow("MyVideo", frame); //show the frame in "MyVideo" window

           /// Convert image to gray and blur it
         cvtColor( frame, src_gray, COLOR_BGR2GRAY );
         blur( src_gray, src_gray, Size(3,3) );
         threshold( src_gray, src, thresh, 255, THRESH_BINARY );
         imshow( "Blur", src );


         /*createTrackbar( " Threshold:", "Source", &thresh, max_thresh, thresh_callback );
         thresh_callback( 0, 0 );*/
           if (waitKey(30) == 27) //wait for 'esc' key press for 30ms. If 'esc' key is pressed, break loop
          {
               cout << "esc key is pressed by user" << endl;
               break;
          }
       }
       return 0;

}
