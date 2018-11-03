//
// Created by cuizhou on 18-7-24.
//

#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/video/background_segm.hpp>


int main()
{
    // Open the video file
//    cv::VideoCapture capture("/home/cuizhou/AAAA/AndroidFACE/data/0/spofing-atack.mp4");
//    cv::VideoCapture capture("/home/cuizhou/AAAA/AndroidFACE/data/0/real.mp4");
//    cv::VideoCapture capture("/home/cuizhou/AAAA/AndroidFACE/data/0/other-real.mp4");

    //  shake
//    cv::VideoCapture capture("/home/cuizhou/AAAA/AndroidFACE/data/1/real.mp4");
//    cv::VideoCapture capture("/home/cuizhou/AAAA/AndroidFACE/data/1/spof-atacking.mp4");
//    cv::VideoCapture capture("/home/cuizhou/AAAA/AndroidFACE/data/1/other.mp4");

    //  2
//    cv::VideoCapture capture("/home/cuizhou/AAAA/AndroidFACE/data/2/real.mp4");
//    cv::VideoCapture capture("/home/cuizhou/AAAA/AndroidFACE/data/2/fake.mp4");

    //  3
//    cv::VideoCapture capture("/home/cuizhou/AAAA/AndroidFACE/data/3/real.mp4");
    cv::VideoCapture capture("/home/cuizhou/AAAA/AndroidFACE/data/3/fake.mp4");

    // check if video successfully opened
    if (!capture.isOpened()){
        std::cout<<"failed to open video!"<<std::endl;
        return 0;
    }

    // current video frame
    cv::Mat frame, frameGray;
    // foreground binary image
    cv::Mat foreground;

    cv::namedWindow("Extracted Foreground");

    // The Mixture of Gaussian object
    // used with all default parameters
    cv::Ptr<cv::BackgroundSubtractorMOG2> pMOG2 = cv::createBackgroundSubtractorMOG2();

    bool stop(false);
    // for all frames in video
    while (!stop) {

        // read next frame if any
        if (!capture.read(frame))
            break;

        cv::resize(frame,frame,cv::Size(0,0),0.5,0.5);

        // update the background
        // and return the foreground
        cvtColor(frame, frameGray, CV_BGR2GRAY);


        float learningRate = 0.001; // or whatever
        cv::Mat foreground;
        pMOG2->apply(frameGray, foreground, learningRate);

        // show foreground
        cv::imshow("Extracted Foreground",foreground);
        cv::imshow("image",frame);

//        std::cout<<foreground<<std::endl;

        // introduce a delay
        // or press key to stop
        if (cv::waitKey(10)>=0)
            stop= true;
    }

    return 0;
}